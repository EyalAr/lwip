#include "decoder.h"

DecodeBufferWorker::DecodeBufferWorker(
    Nan::Callback * callback,
    Local<Object> & buff,
    buf_dec_f_t decoder
): Nan::AsyncWorker(callback), _decoder(decoder), _pixbuf(NULL), _width(0),
    _height(0), _channels(0), _trans(false), _metadata("") {
    SaveToPersistent("buff", buff); // make sure buff isn't GC'ed
    _buffer = Buffer::Data(buff);
    _buffsize = Buffer::Length(buff);
}

DecodeBufferWorker::~DecodeBufferWorker() {}

void DecodeBufferWorker::Execute () {
    CImg<unsigned char> * img = NULL;
    string err;
    char * metadata = NULL;

    err = _decoder(_buffer, _buffsize, &img, &metadata);

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
    _metadata = metadata;

    delete img;
    return;
}

void DecodeBufferWorker::HandleOKCallback () {
    Nan::HandleScope();

    Local<v8::Primitive> metadata;
    if (_metadata == NULL) {
        metadata = Nan::Null();
    } else {
        metadata = Nan::New<String>(_metadata).ToLocalChecked();
    }

    Local<Value> argv[] = {
        Nan::Null(),
        Nan::NewBuffer(
            (char *) _pixbuf,
            _width * _height * _channels
        ).ToLocalChecked(),
        Nan::New<Number>(_width),
        Nan::New<Number>(_height),
        Nan::New<Number>(_channels),
        Nan::New<Boolean>(_trans),
        metadata
    };

    callback->Call(7, argv);
}
