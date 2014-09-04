#ifndef LWIP_IMAGE_H
#define LWIP_IMAGE_H

#define cimg_display 0
#define cimg_use_jpeg
#define cimg_verbosity 0

#include <string>
#include <cmath>
#include <setjmp.h>
#include <node.h>
#include <node_buffer.h>
#include <v8.h>
extern "C" {
#include "jpeglib.h"
}
#include <png.h>
#include <zlib.h>
#include "CImg.h"

using namespace cimg_library;
using namespace v8;

class LwipImage : public node::ObjectWrap {
public:
    static void Init();
    static Handle<Value> NewInstance();
    explicit LwipImage(): _data(NULL) {};
    ~LwipImage();
    CImg<unsigned char> * _data;

private:
    static Handle<Value> New(const Arguments & args);
    static Handle<Value> resize(const Arguments & args);
    static Handle<Value> rotate(const Arguments & args);
    static Handle<Value> blur(const Arguments & args);
    static Handle<Value> crop(const Arguments & args);
    static Handle<Value> mirror(const Arguments & args);
    static Handle<Value> pad(const Arguments & args);
    static Handle<Value> width(const Arguments & args);
    static Handle<Value> height(const Arguments & args);
    static Handle<Value> toJpegBuffer(const Arguments & args);
    static Handle<Value> toPngBuffer(const Arguments & args);
    static Persistent<Function> constructor;
};

struct lwip_jpeg_error_mgr {
    struct jpeg_error_mgr pub;
    jmp_buf setjmp_buffer;
};

inline void lwip_jpeg_error_exit (j_common_ptr cinfo) {
    lwip_jpeg_error_mgr * lwip_jpeg_err = (lwip_jpeg_error_mgr *) cinfo->err;
    longjmp(lwip_jpeg_err->setjmp_buffer, 1);
}

void toJpegBufferAsync(uv_work_t * request);
void toPngBufferAsync(uv_work_t * request);
void pngWriteCB(png_structp png_ptr, png_bytep data, png_size_t length);
void toBufferAsyncDone(uv_work_t * request, int status);
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

struct ToBufferBaton {
    uv_work_t request;
    v8::Persistent<Function> cb;
    LwipImage * img;
    unsigned char * buffer;
    unsigned long bufferSize;
    int jpegQuality;
    int pngCompLevel;
    bool pngInterlaced;
    bool err;
    std::string errMsg;
};

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
