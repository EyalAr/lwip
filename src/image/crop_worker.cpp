#include "image.h"

CropWorker::CropWorker(
    size_t left,
    size_t top,
    size_t right,
    size_t bottom,
    CImg<unsigned char> * cimg,
    Nan::Callback * callback
): Nan::AsyncWorker(callback), _left(left), _top(top), _right(right),
    _bottom(bottom), _cimg(cimg) {}

CropWorker::~CropWorker() {}

void CropWorker::Execute () {
    try {
        int channels = _cimg->spectrum();
        _cimg->crop(_left, _top, 0, 0, _right, _bottom, 0, channels - 1);
    } catch (CImgException e) {
        SetErrorMessage("Unable to crop image");
        return;
    }
    return;
}

void CropWorker::HandleOKCallback () {
    Nan::HandleScope();
    Local<Value> argv[] = {
        Nan::Null()
    };
    callback->Call(1, argv);
}
