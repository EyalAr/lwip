#define BUILDING_NODE_EXTENSION

#include "LwipImage.h"
#include <node.h>
#include <v8.h>

using namespace v8;

Persistent<Function> LwipImage::constructor;

LwipImage::~LwipImage(){
    // free the binary data in memory
    if (_data != NULL) free(_data);
}

void LwipImage::Init() {
    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
    tpl->SetClassName(String::NewSymbol("LwipImage"));
    tpl->InstanceTemplate()->SetInternalFieldCount(2);
    node::SetPrototypeMethod(tpl, "width", width);
    node::SetPrototypeMethod(tpl, "height", height);
    constructor = Persistent<Function>::New(tpl->GetFunction());
}

Handle<Value> LwipImage::NewInstance() {
    HandleScope scope;
    Handle<Value> argv[0] = {};
    Local<Object> instance = constructor->NewInstance(0, argv);
    return scope.Close(instance);
}

Handle<Value> LwipImage::New(const Arguments& args) {
    HandleScope scope;
    LwipImage * obj = new LwipImage();
    obj->Wrap(args.This());
    return args.This();
}

void LwipImage::setData(const char * data) {
    _data = (char *) data;
}

void LwipImage::setWidth(const unsigned int width) {
    _width = (unsigned int) width;
}

void LwipImage::setHeight(const unsigned int height) {
    _height = (unsigned int) height;
}

Handle<Value> LwipImage::width(const Arguments& args) {
    HandleScope scope;
    LwipImage* obj = ObjectWrap::Unwrap<LwipImage>(args.This());
    return scope.Close(Number::New(obj->_width));
}

Handle<Value> LwipImage::height(const Arguments& args) {
    HandleScope scope;
    LwipImage* obj = ObjectWrap::Unwrap<LwipImage>(args.This());
    return scope.Close(Number::New(obj->_height));
}
