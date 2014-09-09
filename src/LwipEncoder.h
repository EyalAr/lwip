#ifndef LWIP_ENCODER_H
#define LWIP_ENCODER_H

#include <string>
#include <cstring>
#include <node.h>
#include <node_buffer.h>
#include <nan.h>
#include <v8.h>

using namespace cimg_library;
using namespace v8;
using namespace node;
using namespace std;

class EncodeToJpegBufferWorker : public NanAsyncWorker {
public:
    EncodeToJpegBufferWorker(
        char * pixbuf,
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
        char * pixbuf,
        size_t width,
        size_t height,
        int compression,
        bool interlaced,
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
    char * _pngbuf;
    size_t _pngbufsize;
};

typedef pngWriteCbData struct {
    unsigned char * buff;
    size_t buffsize;
}

NAN_METHOD(encodeToJpegBuffer);
NAN_METHOD(encodeToPngBuffer);
void pngWriteCB(png_structp png_ptr, png_bytep data, png_size_t length);
void initAll(Handle<Object> exports);

#endif
