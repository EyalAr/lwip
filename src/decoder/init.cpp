#include "decoder.h"

NAN_METHOD(decodeJpegBuffer) {
    NanScope();

    Local<Object> jpegBuff = args[0].As<Object>();
    NanCallback * callback = new NanCallback(args[1].As<Function>());

    NanAsyncQueueWorker(new DecodeBufferWorker(callback, jpegBuff, decode_jpeg_buffer));
    NanReturnUndefined();
}

NAN_METHOD(decodePngBuffer) {
    NanScope();

    Local<Object> pngBuff = args[0].As<Object>();
    NanCallback * callback = new NanCallback(args[1].As<Function>());

    NanAsyncQueueWorker(new DecodeBufferWorker(callback, pngBuff, decode_png_buffer));
    NanReturnUndefined();
}

NAN_METHOD(decodeGifBuffer) {
    NanScope();

    Local<Object> gifBuff = args[0].As<Object>();
    NanCallback * callback = new NanCallback(args[1].As<Function>());

    NanAsyncQueueWorker(new DecodeBufferWorker(callback, gifBuff, decode_gif_buffer));
    NanReturnUndefined();
}

// create an init function for our node module
void InitAll(Handle<Object> exports) {
    exports->Set(NanNew("jpeg"),
                 NanNew<FunctionTemplate>(decodeJpegBuffer)->GetFunction());
    exports->Set(NanNew("png"),
                 NanNew<FunctionTemplate>(decodePngBuffer)->GetFunction());
    exports->Set(NanNew("gif"),
                 NanNew<FunctionTemplate>(decodeGifBuffer)->GetFunction());
}

// use NODE_MODULE macro to register our module:
NODE_MODULE(lwip_decoder, InitAll)
