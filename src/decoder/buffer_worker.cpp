#include "decoder.h"

DecodeBufferWorker::DecodeBufferWorker(
    NanCallback * callback,
    char * buffer,
    size_t buffsize,
    buf_dec_f_t decoder
): NanAsyncWorker(callback), _buffsize(buffsize),
    _decoder(decoder), _pixbuf(NULL), _width(0), _height(0), _channels(0),
    _trans(false) {
    // buffer needs to be copied, because the buffer may be gc'ed by
    // V8 at any time.
    // !!! _buffer still needs to be freed by us when no longer needed (see Execute)
    _buffer = (char *) malloc(_buffsize);
    if (_buffer == NULL) {
        // TODO: check - can I use SetErrorMessage here?
        SetErrorMessage("Out of memory");
        return;
    }
    memcpy(_buffer, buffer, _buffsize);
}

DecodeBufferWorker::~DecodeBufferWorker() {}

void DecodeBufferWorker::Execute () {
    CImg<unsigned char> * img = NULL;
    string err;
    err = _decoder(_buffer, _buffsize, &img);
    free(_buffer);
    if (img == NULL) {
        SetErrorMessage(err.c_str());
        return;
    }
    err = to3Channels(&img);
    if (err != "") {
        if (img) delete img;
        SetErrorMessage(err.c_str());
        return;
    }
    img->_is_shared = true; // don't free image data. need it for callback.
    _pixbuf = img->data();
    _width = img->width();
    _height = img->height();
    // TODO: support transparency:
    _channels = 3;
    _trans = false;
    delete img;
    return;
}

void DecodeBufferWorker::HandleOKCallback () {
    NanScope();
    Local<Value> argv[] = {
        NanNull(),
        NanBufferUse(
            (char *) _pixbuf,
            _width * _height * _channels
        ),
        NanNew<Number>(_width),
        NanNew<Number>(_height),
        NanNew<Number>(_channels),
        NanNew<Boolean>(_trans)
    };
    callback->Call(6, argv);
}
