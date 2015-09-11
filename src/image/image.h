#ifndef LWIP_IMAGE_H
#define LWIP_IMAGE_H

#define cimg_display 0
#define cimg_verbosity 0

#define N_CHANNELS 4

#include <string>
#include <cmath>
#include <node.h>
#include <node_buffer.h>
#include <nan.h>
#include <v8.h>
#include "CImg.h"

using namespace cimg_library;
using namespace v8;
using namespace node;

/*
 * LwipImage object is a wrapper for a CImg object.
 * It is constructed from a memory biffer of RGB pixels.
 * The memory buffer is of type 'unsigned char' such that for an image of size
 * W * H the buffer has (W*H) R(ed) values, then (W*H) G(reen) values and then
 * (W*H) B(lue) values.
 */
class LwipImage : public node::ObjectWrap {
public:
    static void Init(Handle<Object> exports);
    Handle<Value> NewInstance();
    static NAN_METHOD(New);
    static NAN_METHOD(resize);
    static NAN_METHOD(rotate);
    static NAN_METHOD(blur);
    static NAN_METHOD(crop);
    static NAN_METHOD(mirror);
    static NAN_METHOD(pad);
    static NAN_METHOD(sharpen);
    static NAN_METHOD(hslaAdj);
    static NAN_METHOD(opacify);
    static NAN_METHOD(paste);
    static NAN_METHOD(width);
    static NAN_METHOD(height);
    static NAN_METHOD(getPixel);
    static NAN_METHOD(buffer);
    static NAN_METHOD(setPixel);
    LwipImage(unsigned char * data, size_t width, size_t height);
    ~LwipImage();
private:
    static Nan::Persistent<v8::FunctionTemplate> constructor;
    CImg<unsigned char> * _cimg;
};

class ResizeWorker : public Nan::AsyncWorker {
public:
    ResizeWorker(
        size_t width,
        size_t height,
        int inter,
        CImg<unsigned char> * cimg,
        Nan::Callback * callback
    );
    ~ResizeWorker();
    void Execute ();
    void HandleOKCallback ();
private:
    size_t _width;
    size_t _height;
    int _inter;
    CImg<unsigned char> * _cimg;
};

class RotateWorker : public Nan::AsyncWorker {
public:
    RotateWorker(
        float degs,
        unsigned char r,
        unsigned char g,
        unsigned char b,
        unsigned char a,
        CImg<unsigned char> * cimg,
        Nan::Callback * callback
    );
    ~RotateWorker();
    void Execute ();
    void HandleOKCallback ();
private:
    float _degs;
    unsigned char _r;
    unsigned char _g;
    unsigned char _b;
    unsigned char _a;
    CImg<unsigned char> * _cimg;
};

class BlurWorker : public Nan::AsyncWorker {
public:
    BlurWorker(
        float sigma,
        CImg<unsigned char> * cimg,
        Nan::Callback * callback
    );
    ~BlurWorker();
    void Execute ();
    void HandleOKCallback ();
private:
    float _sigma;
    CImg<unsigned char> * _cimg;
};

class CropWorker : public Nan::AsyncWorker {
public:
    CropWorker(
        size_t left,
        size_t top,
        size_t right,
        size_t bottom,
        CImg<unsigned char> * cimg,
        Nan::Callback * callback
    );
    ~CropWorker();
    void Execute ();
    void HandleOKCallback ();
private:
    size_t _left;
    size_t _top;
    size_t _right;
    size_t _bottom;
    CImg<unsigned char> * _cimg;
};

class MirrorWorker : public Nan::AsyncWorker {
public:
    MirrorWorker(
        bool xaxis,
        bool yaxis,
        CImg<unsigned char> * cimg,
        Nan::Callback * callback
    );
    ~MirrorWorker();
    void Execute ();
    void HandleOKCallback ();
private:
    bool _xaxis;
    bool _yaxis;
    CImg<unsigned char> * _cimg;
};

class PadWorker : public Nan::AsyncWorker {
public:
    PadWorker(
        size_t left,
        size_t top,
        size_t right,
        size_t bottom,
        unsigned char r,
        unsigned char g,
        unsigned char b,
        unsigned char a,
        CImg<unsigned char> * cimg,
        Nan::Callback * callback
    );
    ~PadWorker();
    void Execute ();
    void HandleOKCallback ();
private:
    size_t _left;
    size_t _top;
    size_t _right;
    size_t _bottom;
    unsigned char _r;
    unsigned char _g;
    unsigned char _b;
    unsigned char _a;
    CImg<unsigned char> * _cimg;
};

class SharpenWorker : public Nan::AsyncWorker {
public:
    SharpenWorker(
        float amp,
        CImg<unsigned char> * cimg,
        Nan::Callback * callback
    );
    ~SharpenWorker();
    void Execute ();
    void HandleOKCallback ();
private:
    float _amp;
    CImg<unsigned char> * _cimg;
};

class HSLAWorker : public Nan::AsyncWorker {
public:
    HSLAWorker(
        float hs,
        float sd,
        float ld,
        float ad,
        CImg<unsigned char> * cimg,
        Nan::Callback * callback
    );
    ~HSLAWorker();
    void Execute ();
    void HandleOKCallback ();
private:
    float _hs;
    float _sd;
    float _ld;
    float _ad;
    CImg<unsigned char> * _cimg;
};

class OpacifyWorker : public Nan::AsyncWorker {
public:
    OpacifyWorker(
        CImg<unsigned char> * cimg,
        Nan::Callback * callback
    );
    ~OpacifyWorker();
    void Execute ();
    void HandleOKCallback ();
private:
    CImg<unsigned char> * _cimg;
};

class PasteWorker : public Nan::AsyncWorker {
public:
    PasteWorker(
        size_t left,
        size_t top,
        Local<Object> & pixbuf,
        size_t width,
        size_t height,
        CImg<unsigned char> * cimg,
        Nan::Callback * callback
    );
    ~PasteWorker();
    void Execute ();
    void HandleOKCallback ();
private:
    size_t _left;
    size_t _top;
    size_t _width;
    size_t _height;
    unsigned char * _pixels;
    CImg<unsigned char> * _cimg;
};

class SetPixelWorker : public Nan::AsyncWorker {
public:
    SetPixelWorker(
        size_t left,
        size_t top,
        unsigned char r,
        unsigned char g,
        unsigned char b,
        unsigned char a,
        CImg<unsigned char> * cimg,
        Nan::Callback * callback
    );
    ~SetPixelWorker();
    void Execute ();
    void HandleOKCallback ();
private:
    size_t _left;
    size_t _top;
    unsigned char _r;
    unsigned char _g;
    unsigned char _b;
    unsigned char _a;
    CImg<unsigned char> * _cimg;
};

void rgb_to_hsl(unsigned char r, unsigned char g, unsigned char b, float * h,
                float * s, float * l);
void hsl_to_rgb(float h, float s, float l, unsigned char * r, unsigned char * g,
                unsigned char * b);

#endif
