#include "image.h"

BlurWorker::BlurWorker(
    float sigma,
    CImg<unsigned char> * cimg,
    Nan::Callback * callback
): Nan::AsyncWorker(callback), _sigma(sigma), _cimg(cimg) {}

BlurWorker::~BlurWorker() {}

void BlurWorker::Execute () {
    try {
        // filter order = 0 (on the image itself, not derivatives)
        _cimg->vanvliet(_sigma, 0, 'x');
        _cimg->vanvliet(_sigma, 0, 'y');
    } catch (CImgException e) {
        SetErrorMessage("Unable to blur image");
        return;
    }
    return;
}

void BlurWorker::HandleOKCallback () {
    Nan::HandleScope();
    Local<Value> argv[] = {
        Nan::Null()
    };
    callback->Call(1, argv);
}
