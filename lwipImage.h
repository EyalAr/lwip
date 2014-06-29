#ifndef LWIP_IMAGE_H
#define LWIP_IMAGE_H

#define cimg_display 0

#include <node.h>
#include <v8.h>
#include "lib/cimg/CImg.h"

using namespace cimg_library;
using namespace v8;

typedef CImg<unsigned char> * lwip_data_t;

class LwipImage : public node::ObjectWrap {
public:
    static void Init();
    static Handle<Value> NewInstance();
    explicit LwipImage(): _data(NULL){};
    ~LwipImage();
    void setData(lwip_data_t data);

private:
    static Handle<Value> New(const Arguments& args);
    // static Handle<Value> resize(const Arguments& args);
    // static Handle<Value> rotate(const Arguments& args);
    // static Handle<Value> crop(const Arguments& args);
    static Handle<Value> width(const Arguments& args);
    static Handle<Value> height(const Arguments& args);
    // static Handle<Value> toBuffer(const Arguments& args);
    static Persistent<Function> constructor;
    lwip_data_t _data;
};

#endif
