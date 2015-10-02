#include "image.h"

// create an init function for our node module
void InitAll(Handle<Object> exports) {
    LwipImage::Init(exports);
}

// use NODE_MODULE macro to register our module:
NODE_MODULE(lwip_image, InitAll)
