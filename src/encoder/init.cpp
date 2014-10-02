#include "encoder.h"

// encoder.jpeg(pixbuf, width, height, quality, callback)
NAN_METHOD(encodeToJpegBuffer) {
    NanScope();

    Local<Object> buff = args[0].As<Object>();
    size_t width = args[1].As<Integer>()->Value();
    size_t height = args[2].As<Integer>()->Value();
    int quality = args[3].As<Integer>()->Value();
    NanCallback * callback = new NanCallback(args[4].As<Function>());

    NanAsyncQueueWorker(
        new EncodeToJpegBufferWorker(
            buff,
            width,
            height,
            quality,
            callback
        ));
    NanReturnUndefined();
}

// encoder.png(pixbuf, width, height, compression, interlaced, trans, callback)
NAN_METHOD(encodeToPngBuffer) {
    NanScope();

    Local<Object> buff = args[0].As<Object>();
    size_t width = args[1].As<Integer>()->Value();
    size_t height = args[2].As<Integer>()->Value();
    int compression = args[3].As<Integer>()->Value();
    bool interlaced = args[4]->BooleanValue();
    bool trans = args[5]->BooleanValue();
    NanCallback * callback = new NanCallback(args[6].As<Function>());

    NanAsyncQueueWorker(
        new EncodeToPngBufferWorker(
            buff,
            width,
            height,
            compression,
            interlaced,
            trans,
            callback
        )
    );
    NanReturnUndefined();
}

// create an init function for our node module
void InitAll(Handle<Object> exports) {
    exports->Set(NanNew("jpeg"),
                 NanNew<FunctionTemplate>(encodeToJpegBuffer)->GetFunction());
    exports->Set(NanNew("png"),
                 NanNew<FunctionTemplate>(encodeToPngBuffer)->GetFunction());
}

// use NODE_MODULE macro to register our module:
NODE_MODULE(lwip_encoder, InitAll)
