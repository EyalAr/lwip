#include "image.h"

SharpenWorker::SharpenWorker(
    float amp,
    CImg<unsigned char> * cimg,
    NanCallback * callback
): NanAsyncWorker(callback), _amp(amp), _cimg(cimg) {}

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
    NanScope();
    Local<Value> argv[] = {
        NanNull()
    };
    callback->Call(1, argv);
}
