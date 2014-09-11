#include "decoder.h"

NAN_METHOD(decodeJpegFile) {
    NanScope();

    NanUtf8String path = NanUtf8String(args[0]);
    NanCallback * callback = new NanCallback(args[1].As<Function>());

    NanAsyncQueueWorker(new DecodeFileWorker(callback, string(*path), "jpeg"));
    NanReturnUndefined();
}

NAN_METHOD(decodePngFile) {
    NanScope();

    NanUtf8String path = NanUtf8String(args[0]);
    NanCallback * callback = new NanCallback(args[1].As<Function>());

    NanAsyncQueueWorker(new DecodeFileWorker(callback, string(*path), "png"));
    NanReturnUndefined();
}

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
