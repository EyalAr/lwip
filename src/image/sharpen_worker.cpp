#include "image.h"

SharpenWorker::SharpenWorker(
    float amp,
    CImg<unsigned char> * cimg,
    Nan::Callback * callback
): Nan::AsyncWorker(callback), _amp(amp), _cimg(cimg) {}

SharpenWorker::~SharpenWorker() {}

void SharpenWorker::Execute () {
    try {
        // sharpen_type = false - inverse diffusion
        _cimg->sharpen(_amp, false);
    } catch (CImgException e) {
        SetErrorMessage("Unable to sharpen image");
        return;
    }
    return;
}

void SharpenWorker::HandleOKCallback () {
    Nan::HandleScope();
    Local<Value> argv[] = {
        Nan::Null()
    };
    callback->Call(1, argv);
}
