#include "image.h"

RotateWorker::RotateWorker(
    float degs,
    unsigned char r,
    unsigned char g,
    unsigned char b,
    CImg<unsigned char> * cimg,
    NanCallback * callback
): NanAsyncWorker(callback), _degs(degs), _r(r), _g(g), _b(b), _cimg(cimg) {}

RotateWorker::~RotateWorker() {}

void RotateWorker::Execute () {
    const float nangle = cimg::mod(_degs, 360.0f);
    if (cimg::mod(nangle, 90.0f) != 0) {
        CImg<unsigned char> * res;
        size_t oldwidth = _cimg->width(),
               oldheight = _cimg->height();
        try {
            // 2 pixels wider and taller
            res = new CImg<unsigned char>(oldwidth + 2, oldheight + 2, 1, 3);
        } catch (CImgException e) {
            SetErrorMessage("Out of memory");
            return;
        }
        cimg_forX(*res, x) {
            res->fillC(x, 0, 0, _r, _g, _b);
            res->fillC(x, oldheight + 1, 0, _r, _g, _b);
        }
        cimg_forY(*res, y) {
            res->fillC(0, y, 0, _r, _g, _b);
            res->fillC(oldwidth + 1, y, 0, _r, _g, _b);
        }
        cimg_forXY(*_cimg, x, y) {
            unsigned char r = (*_cimg)(x, y, 0, 0),
                          g = (*_cimg)(x, y, 0, 1),
                          b = (*_cimg)(x, y, 0, 2);
            res->fillC(x + 1, y + 1, 0, r, g, b);
        }
        res->move_to(*_cimg);
        delete res;
    }
    try {
        // linear interpolations = 1
        // neumann boundary condition = 1
        _cimg->rotate(nangle, 1, 1);
    } catch (CImgException e) {
        SetErrorMessage("Unable to rotate image");
        return;
    }
    return;
}

void RotateWorker::HandleOKCallback () {
    NanScope();
    Local<Value> argv[] = {
        NanNull()
    };
    callback->Call(1, argv);
}
