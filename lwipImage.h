#define BUILDING_NODE_EXTENSION

#ifndef LWIP_IMAGE_H
#define LWIP_IMAGE_H

#include <node.h>
#include <v8.h>

class LwipImage : public node::ObjectWrap {
public:
    static void Init();
    static v8::Handle<v8::Value> NewInstance();
    explicit LwipImage(): _data(NULL), _width(0), _height(0){};
    ~LwipImage();
    void setData(const char * data);
    void setWidth(const unsigned int width);
    void setHeight(const unsigned int height);

private:
    static v8::Handle<v8::Value> New(const v8::Arguments& args);
    // static v8::Handle<v8::Value> resize(const v8::Arguments& args);
    // static v8::Handle<v8::Value> rotate(const v8::Arguments& args);
    // static v8::Handle<v8::Value> crop(const v8::Arguments& args);
    static v8::Handle<v8::Value> width(const v8::Arguments& args);
    static v8::Handle<v8::Value> height(const v8::Arguments& args);
    // static v8::Handle<v8::Value> toBuffer(const v8::Arguments& args);
    static v8::Persistent<v8::Function> constructor;
    char * _data;
    unsigned int _width, _height;
};

#endif
