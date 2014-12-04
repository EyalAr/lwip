#include "image.h"

SetPixelWorker::SetPixelWorker(
    size_t left,
    size_t top,
    unsigned char r,
    unsigned char g,
    unsigned char b,
    unsigned char a,
    CImg<unsigned char> * cimg,
    NanCallback * callback
): NanAsyncWorker(callback), _left(left), _top(top), _r(r), _g(g), _b(b), _a(a),
    _cimg(cimg) {}

SetPixelWorker::~SetPixelWorker() {}

void SetPixelWorker::Execute () {
    try {
        _cimg->fillC(_left, _top, 0, _r, _g, _b, _a);
    } catch (CImgException e) {
        SetErrorMessage("Unable to set pixel");
        return;
    }
    return;
}

void SetPixelWorker::HandleOKCallback () {
    NanScope();
    Local<Value> argv[] = {
        NanNull()
    };
    callback->Call(1, argv);
}
