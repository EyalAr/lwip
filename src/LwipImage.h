#ifndef LWIP_IMAGE_H
#define LWIP_IMAGE_H

#define cimg_display 0

#include <string>
#include <node.h>
#include <node_buffer.h>
#include <v8.h>
#include "jpeglib.h"
#include "CImg.h"

using namespace cimg_library;
using namespace v8;

typedef CImg<unsigned char> * lwip_data_t;

class LwipImage : public node::ObjectWrap {
public:
    static void Init();
    static Handle<Value> NewInstance();
    explicit LwipImage(): _data(NULL){};
    ~LwipImage();
    lwip_data_t _data;

private:
    static Handle<Value> New(const Arguments& args);
    // static Handle<Value> resize(const Arguments& args);
    // static Handle<Value> rotate(const Arguments& args);
    // static Handle<Value> crop(const Arguments& args);
    static Handle<Value> width(const Arguments& args);
    static Handle<Value> height(const Arguments& args);
    static Handle<Value> toBuffer(const Arguments& args);
    static Persistent<Function> constructor;
};

void toBufferAsync(uv_work_t * request);
void toBufferAsyncDone(uv_work_t * request, int status);

struct ToBufferBaton {
    uv_work_t request;
    v8::Persistent<Function> cb;
    LwipImage * img;
    unsigned char * buffer;
    unsigned long bufferSize;
    std::string format;
    bool err;
    std::string errMsg;
};

#endif
