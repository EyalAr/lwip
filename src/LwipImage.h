#ifndef LWIP_IMAGE_H
#define LWIP_IMAGE_H

#define cimg_display 0
#define cimg_use_jpeg
#define cimg_verbosity 0

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
    static void Init();
    static Handle<Value> NewInstance();
    static NAN_METHOD(New);
    static NAN_METHOD(resize);
    static NAN_METHOD(rotate);
    static NAN_METHOD(blur);
    static NAN_METHOD(crop);
    static NAN_METHOD(mirror);
    static NAN_METHOD(pad);
    static NAN_METHOD(width);
    static NAN_METHOD(height);
    LwipImage(unsigned char * data, size_t width, size_t height);
    ~LwipImage();
private:
    CImg<unsigned char> * _cimg;
    static Persistent<FunctionTemplate> constructor;
};

void resizeAsync(uv_work_t * request);
void resizeAsyncDone(uv_work_t * request, int status);
void rotateAsync(uv_work_t * request);
void rotateAsyncDone(uv_work_t * request, int status);
void blurAsync(uv_work_t * request);
void blurAsyncDone(uv_work_t * request, int status);
void cropAsync(uv_work_t * request);
void cropAsyncDone(uv_work_t * request, int status);
void mirrorAsync(uv_work_t * request);
void mirrorAsyncDone(uv_work_t * request, int status);
void padAsync(uv_work_t * request);
void padAsyncDone(uv_work_t * request, int status);

struct resizeBaton {
    uv_work_t request;
    v8::Persistent<Function> cb;
    LwipImage * img;
    int width;
    int height;
    int inter;
    bool err;
    std::string errMsg;
};

struct rotateBaton {
    uv_work_t request;
    v8::Persistent<Function> cb;
    LwipImage * img;
    float degs;
    unsigned char color[3];
    bool err;
    std::string errMsg;
};

struct blurBaton {
    uv_work_t request;
    v8::Persistent<Function> cb;
    LwipImage * img;
    float sigma;
    bool err;
    std::string errMsg;
};

struct cropBaton {
    uv_work_t request;
    v8::Persistent<Function> cb;
    LwipImage * img;
    unsigned int left, top, right, bottom;
    bool err;
    std::string errMsg;
};

struct mirrorBaton {
    uv_work_t request;
    v8::Persistent<Function> cb;
    LwipImage * img;
    bool xaxis;
    bool yaxis;
    bool err;
    std::string errMsg;
};

struct padBaton {
    uv_work_t request;
    v8::Persistent<Function> cb;
    LwipImage * img;
    unsigned int left;
    unsigned int top;
    unsigned int right;
    unsigned int bottom;
    unsigned char color[3];
    bool err;
    std::string errMsg;
};

#endif
