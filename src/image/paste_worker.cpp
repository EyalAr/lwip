#include "image.h"

PasteWorker::PasteWorker(
    size_t left,
    size_t top,
    Local<Object> & pixbuf,
    size_t width,
    size_t height,
    CImg<unsigned char> * cimg,
    Nan::Callback * callback
): Nan::AsyncWorker(callback), _left(left), _top(top), _width(width),
   _height(height), _cimg(cimg) {
    SaveToPersistent("pixbuf", pixbuf);
    _pixels = (unsigned char *) Buffer::Data(pixbuf);
}

PasteWorker::~PasteWorker() {}

void PasteWorker::Execute () {
    try {
        CImg<unsigned char> pst(_pixels, _width, _height, 1, 4, true);
        for (size_t x = _left ; x < _left + pst.width() ; x++) {
            for (size_t y = _top ; y < _top + pst.height() ; y++) {
                float a = ((float) (*_cimg)(x, y, 0, 3)) / 100.0,
                      pa = ((float) pst(x - _left, y - _top, 0, 3)) / 100.0,
                      acpa = a * (1 - pa),
                      na = pa + acpa;
                unsigned char r = (*_cimg)(x, y, 0, 0),
                              g = (*_cimg)(x, y, 0, 1),
                              b = (*_cimg)(x, y, 0, 2),
                              pr = pst(x - _left, y - _top, 0, 0),
                              pg = pst(x - _left, y - _top, 0, 1),
                              pb = pst(x - _left, y - _top, 0, 2),
                              nr = 0, ng = 0, nb = 0;
                if (na != 0) {
                    nr = (unsigned char) ((((float) pr) * pa + ((float) r) * acpa) / na);
                    ng = (unsigned char) ((((float) pg) * pa + ((float) g) * acpa) / na);
                    nb = (unsigned char) ((((float) pb) * pa + ((float) b) * acpa) / na);
                }
                _cimg->fillC(x, y, 0, nr, ng, nb, (unsigned char) (na * 100.0));
            }
        }
    } catch (CImgException e) {
        SetErrorMessage("Unable to paste image");
        return;
    }
}

void PasteWorker::HandleOKCallback () {
    Nan::HandleScope();
    Local<Value> argv[] = {
        Nan::Null()
    };
    callback->Call(1, argv);
}
