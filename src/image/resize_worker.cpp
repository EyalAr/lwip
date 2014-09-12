#include "image.h"

ResizeWorker::ResizeWorker(
    size_t width,
    size_t height,
    int inter,
    CImg<unsigned char> * cimg,
    NanCallback * callback
): NanAsyncWorker(callback), _width(width), _height(height), _inter(inter),
    _cimg(cimg) {}

ResizeWorker::~ResizeWorker() {}

void ResizeWorker::Execute () {
    try {
        _cimg->resize(_width, _height, -100, -100, _inter);
    } catch (CImgException e) {
        SetErrorMessage("Unable to resize image");
        return;
    }
    return;
}

void ResizeWorker::HandleOKCallback () {
    NanScope();
    Local<Value> argv[] = {
        NanNull()
    };
    callback->Call(1, argv);
}
