#include "image.h"

NAN_METHOD(buildImage) {
    NanScope();

    Local<Object> buff = args[0].As<Object>();
    unsigned char * pixbuf = (unsigned char *) Buffer::Data(buff);
    NanCallback * callback = new NanCallback(args[1].As<Function>());

    // NanAsyncQueueWorker(new DecodeBufferWorker(callback, buffer, "png"));
    NanReturnUndefined();
}

// create an init function for our node module
void InitAll(Handle<Object> exports) {
    exports->Set(NanNew("build"),
                 NanNew<FunctionTemplate>(buildImage)->GetFunction());
}

// use NODE_MODULE macro to register our module:
NODE_MODULE(lwip_image_builder, InitAll)
