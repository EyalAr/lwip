#include "image.h"

HSLAWorker::HSLAWorker(
    float hs,
    float sd,
    float ld,
    float ad,
    CImg<unsigned char> * cimg,
    Nan::Callback * callback
): Nan::AsyncWorker(callback), _hs(hs), _sd(sd), _ld(ld), _ad(ad), _cimg(cimg) {}

HSLAWorker::~HSLAWorker() {}

void HSLAWorker::Execute () {
    if (_hs == 0 && _sd == 0 && _ld == 0 && _ad == 0) return;
    try {
        cimg_forXY(*_cimg, x, y) {
            unsigned char r = (*_cimg)(x, y, 0, 0),
                          g = (*_cimg)(x, y, 0, 1),
                          b = (*_cimg)(x, y, 0, 2),
                          a = (*_cimg)(x, y, 0, 3);
            float h, s, l, af = (float) a;
            rgb_to_hsl(r, g, b, &h, &s, &l);

            if (_hs != 0) {
                h += _hs;
                h = cimg::mod(h, 360.0f);
            }

            if (_sd != 0) {
                s *= 1.0 + _sd;
                if (s > 1) s = 1;
                else if (s < 0) s = 0;
            }

            if (_ld != 0) {
                l *= 1.0 + _ld;
                if (l > 1) l = 1;
                else if (l < 0) l = 0;
            }

            if (_ad != 0) {
                af *= 1.0 + _ad;
                if (af > 100) af = 100;
                else if (af < 0) af = 0;
            }

            hsl_to_rgb(h, s, l, &r, &g, &b);
            a = (unsigned char) af;

            _cimg->fillC(x, y, 0, r, g, b, a);
        }
    } catch (CImgException e) {
        SetErrorMessage("Unable to modify HSLA");
        return;
    }
    return;
}

void HSLAWorker::HandleOKCallback () {
    Nan::HandleScope();
    Local<Value> argv[] = {
        Nan::Null()
    };
    callback->Call(1, argv);
}

void rgb_to_hsl(unsigned char r, unsigned char g, unsigned char b, float * h,
                float * s, float * l) {
    float _r = ((float) r) / 255.0,
          _g = ((float) g) / 255.0,
          _b = ((float) b) / 255.0;
    float max = _r, min = _r;
    if (_g > max) max = _g;
    if (_b > max) max = _b;
    if (_g < min) min = _g;
    if (_b < min) min = _b;
    *h = 0;
    *s = 0;
    *l = (max + min) / 2;

    if (max != min) {
        float
        f = (_r == min) ? (_g - _b) : ((_g == min) ? (_b - _r) : (_r - _g)),
        i = (_r == min) ? 3.0f : ((_g == min) ? 5.0f : 1.0f);
        *h = (i - f / (max - min));
        if (*h >= 6) *h -= 6;
        *h *= 60;
        *s = (2 * *l <= 1) ? ((max - min) / (max + min)) : ((max - min) / (2 - max - min));
    }

    return;
};

void hsl_to_rgb(float h, float s, float l, unsigned char * r, unsigned char * g,
                unsigned char * b) {
    float
    q = 2 * l < 1 ? l * (1 + s) : (l + s - l * s),
    p = 2 * l - q,
    _h = h / 360,
    tr = _h + 1.0f / 3,
    tg = _h,
    tb = _h - 1.0f / 3,
    ntr = tr < 0 ? tr + 1 : (tr > 1 ? tr - 1 : tr),
    ntg = tg < 0 ? tg + 1 : (tg > 1 ? tg - 1 : tg),
    ntb = tb < 0 ? tb + 1 : (tb > 1 ? tb - 1 : tb),
    _r = 255 * (6 * ntr < 1 ? p + (q - p) * 6 * ntr : (2 * ntr < 1 ? q : (3 * ntr < 2 ? p + (q - p) * 6 * (2.0f / 3 - ntr) : p))),
    _g = 255 * (6 * ntg < 1 ? p + (q - p) * 6 * ntg : (2 * ntg < 1 ? q : (3 * ntg < 2 ? p + (q - p) * 6 * (2.0f / 3 - ntg) : p))),
    _b = 255 * (6 * ntb < 1 ? p + (q - p) * 6 * ntb : (2 * ntb < 1 ? q : (3 * ntb < 2 ? p + (q - p) * 6 * (2.0f / 3 - ntb) : p)));

    *r = (unsigned char) _r;
    *g = (unsigned char) _g;
    *b = (unsigned char) _b;
    return;
};
