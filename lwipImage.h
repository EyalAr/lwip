#ifndef LWIP_IMAGE_H
#define LWIP_IMAGE_H

#include <node.h>
#include <v8.h>

class LwipImage : public node::ObjectWrap {
public:
    static void Init(v8::Handle<v8::Object> exports);

private:
    explicit LwipImage(char * data_ = NULL);
    ~LwipImage();
    static v8::Handle<v8::Value> New(const v8::Arguments&);
    static v8::Handle<v8::Value> setData(const char *);
    static v8::Persistent<v8::Function> constructor;
    char * data_;
};

#endif
