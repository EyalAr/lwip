#ifndef LWIP_ENCODER_H
#define LWIP_ENCODER_H

#define cimg_display 0
#define cimg_verbosity 0

#include <string>
#include <cstring>
#include <node.h>
#include <node_buffer.h>
#include <nan.h>
#include <v8.h>
extern "C" {
#include "jpeglib.h"
}
#include <png.h>
#include <zlib.h>
#include "CImg.h"

using namespace cimg_library;
using namespace v8;
using namespace node;
using namespace std;

class EncodeToJpegBufferWorker : public NanAsyncWorker {
public:
    EncodeToJpegBufferWorker(
        Local<Object> & buff,
        size_t width,
        size_t height,
        int quality,
        NanCallback * callback
    );
    ~EncodeToJpegBufferWorker();
    void Execute ();
    void HandleOKCallback ();
private:
    unsigned char * _pixbuf;
    size_t _width;
    size_t _height;
    int _quality;
    char * _jpegbuf;
    size_t _jpegbufsize;
};

class EncodeToPngBufferWorker : public NanAsyncWorker {
public:
    EncodeToPngBufferWorker(
        Local<Object> & buff,
        size_t width,
        size_t height,
        int compression,
        bool interlaced,
        bool trans,
        NanCallback * callback
    );
    ~EncodeToPngBufferWorker();
    void Execute ();
    void HandleOKCallback ();
private:
    unsigned char * _pixbuf;
    size_t _width;
    size_t _height;
    int _compression;
    bool _interlaced;
    bool _trans;
    char * _pngbuf;
    size_t _pngbufsize;
};

typedef struct {
    unsigned char * buff;
    size_t buffsize;
} pngWriteCbData;

struct lwip_jpeg_error_mgr {
    struct jpeg_error_mgr pub;
    jmp_buf setjmp_buffer;
};

inline void lwip_jpeg_error_exit (j_common_ptr cinfo) {
    lwip_jpeg_error_mgr * lwip_jpeg_err = (lwip_jpeg_error_mgr *) cinfo->err;
    longjmp(lwip_jpeg_err->setjmp_buffer, 1);
}

NAN_METHOD(encodeToJpegBuffer);
NAN_METHOD(encodeToPngBuffer);
void pngWriteCB(png_structp png_ptr, png_bytep data, png_size_t length);
void initAll(Handle<Object> exports);

#endif
