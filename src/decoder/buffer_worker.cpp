#include "decoder.h"

DecodeBufferWorker::DecodeBufferWorker(
    NanCallback * callback,
    Local<Object> & buff,
    buf_dec_f_t decoder
): NanAsyncWorker(callback), _decoder(decoder), _pixbuf(NULL), _width(0),
    _height(0), _channels(0), _trans(false) {
    SaveToPersistent("buff", buff); // make sure buff isn't GC'ed
    _buffer = Buffer::Data(buff);
    _buffsize = Buffer::Length(buff);
}

DecodeBufferWorker::~DecodeBufferWorker() {}

void DecodeBufferWorker::Execute () {
    CImg<unsigned char> * img = NULL;
    string err;
    err = _decoder(_buffer, _buffsize, &img);
    if (img == NULL) {
        SetErrorMessage(err.c_str());
        return;
    }
    _trans = img->spectrum() == 2 || img->spectrum() == 4;
    err = toRGBA(&img);
    if (err != "") {
        if (img) delete img;
        SetErrorMessage(err.c_str());
        return;
    }
    img->_is_shared = true; // don't free image data. need it for callback.
    _pixbuf = img->data();
    _width = img->width();
    _height = img->height();
    _channels = 4;
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
