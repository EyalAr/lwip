#include "image.h"

OpacifyWorker::OpacifyWorker(
    CImg<unsigned char> * cimg,
    NanCallback * callback
): NanAsyncWorker(callback), _cimg(cimg) {}

OpacifyWorker::~OpacifyWorker() {}

void OpacifyWorker::Execute () {
    try {
        cimg_forXY(*_cimg, x, y) {
            unsigned char r = (*_cimg)(x, y, 0, 0),
                          g = (*_cimg)(x, y, 0, 1),
                          b = (*_cimg)(x, y, 0, 2);
            _cimg->fillC(x, y, 0, r, g, b, 100);
        }
    } catch (CImgException e) {
        SetErrorMessage("Unable to modify HSLA");
        return;
    }
    return;
}

void OpacifyWorker::HandleOKCallback () {
    NanScope();
    Local<Value> argv[] = {
        NanNull()
    };
    callback->Call(1, argv);
}
