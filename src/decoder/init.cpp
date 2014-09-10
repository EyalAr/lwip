#include "decoder.h"

// create an init function for our node module
void InitAll(Handle<Object> exports) {
    exports->Set(NanNew("jpegFile"),
                 NanNew<FunctionTemplate>(decodeJpegFile)->GetFunction());
    exports->Set(NanNew("pngFile"),
                 NanNew<FunctionTemplate>(decodePngFile)->GetFunction());
    exports->Set(NanNew("jpegBuffer"),
                 NanNew<FunctionTemplate>(decodeJpegBuffer)->GetFunction());
    exports->Set(NanNew("pngBuffer"),
                 NanNew<FunctionTemplate>(decodePngBuffer)->GetFunction());
}

// use NODE_MODULE macro to register our module:
NODE_MODULE(lwip_decoder, InitAll)
