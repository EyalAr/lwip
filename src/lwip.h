#ifndef LWIP_H
#define LWIP_H

#define cimg_display 0
#define cimg_use_jpeg
#define cimg_use_png
#define cimg_verbosity 0

#include <string>
#include <cstring>
#include <node.h>
#include <v8.h>
#include "CImg.h"
#include "LwipImage.h"

using namespace cimg_library;
using namespace v8;
using namespace node;

/**
 * Contains information regarding the image to be opened
 */
struct ImageOpenBaton {
    uv_work_t request;
    v8::Persistent<Function> cb;
    CImg<unsigned char> * imgData;
    std::string imgPath;
    bool err;
    std::string errMsg;
};

/**
 * Decode JPEG image and prepare the image data as a CIMG object.
 */
void openJpegAsync(uv_work_t * request);

/**
 * Decode PNG image and prepare the image data as a CIMG object.
 */
void openPngAsync(uv_work_t * request);

/**
 * Utility function to take a CIMG object (*tmp), convert it to 3 channels
 * (if not 3 channels already) and have iob->imgData point to it.
 */
void to3Channels(CImg<unsigned char> * tmp, ImageOpenBaton * iob);

/**
 * Construct an LwipImage object from the opened image data and bring it
 * back to Javascript land.
 */
void openAsyncDone(uv_work_t * request, int status);

/**
 * Queue the async image opening task with libuv and register the appropriate
 * callback.
 */
Handle<Value> open(const Arguments & args, void asyncOpener(uv_work_t *));

Handle<Value> openJpeg(const Arguments & args);
Handle<Value> openPng(const Arguments & args);
void init(Handle<Object> exports, Handle<Object> module);

#endif
