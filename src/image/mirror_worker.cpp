#include "image.h"

MirrorWorker::MirrorWorker(
        bool xaxis,
        bool yaxis,
    CImg<unsigned char> * cimg,
    Nan::Callback * callback
): Nan::AsyncWorker(callback), _xaxis(xaxis), _yaxis(yaxis), _cimg(cimg) {}

MirrorWorker::~MirrorWorker() {}

void MirrorWorker::Execute () {
    std::string axes = "";
    if (_xaxis) {
        axes += "x";
    }
    if (_yaxis) {
        axes += "y";
    }
    try {
        _cimg->mirror(axes.c_str());
    } catch (CImgException e) {
        SetErrorMessage("Unable to mirror image");
        return;
    }
    return;
}

void MirrorWorker::HandleOKCallback () {
    Nan::HandleScope();
    Local<Value> argv[] = {
        Nan::Null()
    };
    callback->Call(1, argv);
}
