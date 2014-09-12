#include "decoder.h"

DecodeBufferWorker::DecodeBufferWorker(
    NanCallback * callback,
    char * buffer,
    size_t buffsize,
    string type
): NanAsyncWorker(callback), _buffer(buffer), _buffsize(buffsize),
    _type(type), _pixbuf(NULL), _width(0), _height(0), _channels(0),
    _trans(false) {}

DecodeBufferWorker::~DecodeBufferWorker() {}

void DecodeBufferWorker::Execute () {
    CImg<unsigned char> * img = NULL;
    if (_type == "jpeg") img = decode_jpeg_buffer(_buffer, _buffsize);
    // else if (_type == "png") img = decode_png_buffer(_buffer, _buffsize);
    // TODO: GIF support.
    // no other types for now
    if (img == NULL) {
        SetErrorMessage("Unable to decode buffer");
        return;
    }
    string err = to3Channels(&img);
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
