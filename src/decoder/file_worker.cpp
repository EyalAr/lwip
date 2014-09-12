#include "decoder.h"

DecodeFileWorker::DecodeFileWorker(NanCallback * callback, string path, string type)
    : NanAsyncWorker(callback), _path(path), _type(type), _pixbuf(NULL),
      _width(0), _height(0), _channels(0), _trans(false) {
}

DecodeFileWorker::~DecodeFileWorker() {
    // _pixbuf is not freed because it is given to JS as a Buffer object.
}

void DecodeFileWorker::Execute () {
    CImg<unsigned char> * img;
    try {
        img = new CImg<unsigned char>();
        if (_type == "jpeg") img->load_jpeg(_path.c_str());
        else if (_type == "png") img->load_png(_path.c_str());
        // TODO: GIF support.
        // no other types for now
    } catch (CImgException e) {
        if (img) delete img;
        SetErrorMessage("Unable to open file");
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

void DecodeFileWorker::HandleOKCallback () {
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
