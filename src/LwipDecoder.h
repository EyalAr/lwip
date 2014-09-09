#ifndef LWIP_DECODER_H
#define LWIP_DECODER_H

#define cimg_display 0
#define cimg_use_jpeg
#define cimg_use_png
#define cimg_verbosity 0

#include <string>
#include <cstring>
#include <node.h>
#include <node_buffer.h>
#include <nan.h>
#include <v8.h>
#include "CImg.h"

using namespace cimg_library;
using namespace v8;
using namespace node;
using namespace std;

class DecodeBufferWorker : public NanAsyncWorker {
public:
    DecodeBufferWorker(NanCallback * callback, char * buffer, string type);
    ~DecodeBufferWorker();
    void Execute ();
    void HandleOKCallback ();
private:
    char * _buffer;
    string _type;
    unsigned char * _pixbuf;
    size_t _width;
    size_t _height;
    int _channels;
    bool _trans; // transparency
};

class DecodeFileWorker : public NanAsyncWorker {
public:
    DecodeFileWorker(NanCallback * callback, string path, string type);
    ~DecodeFileWorker();
    void Execute ();
    void HandleOKCallback ();
private:
    string _path;
    string _type;
    unsigned char * _pixbuf;
    size_t _width;
    size_t _height;
    int _channels;
    bool _trans; // transparency
};

/**
 * Utility function to take a CIMG object (**tmp), and convert it to 3 channels
 * (if not 3 channels already)
 * Returns an error string if there was an error. Empty string otherwise.
 */
string to3Channels(CImg<unsigned char> ** img);

NAN_METHOD(decodeJpegFile);
NAN_METHOD(decodePngFile);
NAN_METHOD(decodeJpegBuffer);
NAN_METHOD(decodePngBuffer);
void initAll(Handle<Object> exports);

#endif
