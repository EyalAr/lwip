#include "decoder.h"

NAN_METHOD(decodeJpegBuffer) {
    Nan::HandleScope();

    Local<Object> jpegBuff = info[0].As<Object>();
    Nan::Callback * callback = new Nan::Callback(info[1].As<Function>());

    Nan::AsyncQueueWorker(new DecodeBufferWorker(callback, jpegBuff, decode_jpeg_buffer));
}

NAN_METHOD(decodePngBuffer) {
    Nan::HandleScope();

    Local<Object> pngBuff = info[0].As<Object>();
    Nan::Callback * callback = new Nan::Callback(info[1].As<Function>());

    Nan::AsyncQueueWorker(new DecodeBufferWorker(callback, pngBuff, decode_png_buffer));
}

NAN_METHOD(decodeGifBuffer) {
    Nan::HandleScope();

    Local<Object> gifBuff = info[0].As<Object>();
    Nan::Callback * callback = new Nan::Callback(info[1].As<Function>());

    Nan::AsyncQueueWorker(new DecodeBufferWorker(callback, gifBuff, decode_gif_buffer));
}

// create an init function for our node module
NAN_MODULE_INIT(InitAll) {
    Nan::Export(target, "jpeg", decodeJpegBuffer);
    Nan::Export(target, "png", decodePngBuffer);
    Nan::Export(target, "gif", decodeGifBuffer);
}

// use NODE_MODULE macro to register our module:
NODE_MODULE(lwip_decoder, InitAll)
