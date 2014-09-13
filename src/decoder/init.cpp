#include "decoder.h"

NAN_METHOD(decodeJpegBuffer) {
    NanScope();

    Local<Object> jpegBuff = args[0].As<Object>();
    char * buffer = Buffer::Data(jpegBuff);
    size_t buffsize = Buffer::Length(jpegBuff);
    NanCallback * callback = new NanCallback(args[1].As<Function>());

    NanAsyncQueueWorker(new DecodeBufferWorker(callback, buffer, buffsize, decode_jpeg_buffer));
    NanReturnUndefined();
}

NAN_METHOD(decodePngBuffer) {
    NanScope();

    Local<Object> pngBuff = args[0].As<Object>();
    char * buffer = Buffer::Data(pngBuff);
    size_t buffsize = Buffer::Length(pngBuff);
    NanCallback * callback = new NanCallback(args[1].As<Function>());

    NanAsyncQueueWorker(new DecodeBufferWorker(callback, buffer, buffsize, decode_png_buffer));
    NanReturnUndefined();
}

// create an init function for our node module
void InitAll(Handle<Object> exports) {
    exports->Set(NanNew("jpeg"),
                 NanNew<FunctionTemplate>(decodeJpegBuffer)->GetFunction());
    exports->Set(NanNew("png"),
                 NanNew<FunctionTemplate>(decodePngBuffer)->GetFunction());
}

// use NODE_MODULE macro to register our module:
NODE_MODULE(lwip_decoder, InitAll)
