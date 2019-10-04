#include "image.h"

// create an init function for our node module
void InitAll(v8::Local<v8::Object> exports) {
    LwipImage::Init(exports);
}

// use NODE_MODULE macro to register our module:
NODE_MODULE(lwip_image, InitAll)
