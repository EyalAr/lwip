#include "decoder.h"

NAN_METHOD(decodeJpegBuffer) {
    Nan::HandleScope scope;

    Local<Object> jpegBuff = info[0].As<Object>();
    Nan::Callback * callback = new Nan::Callback(info[1].As<Function>());

    Nan::AsyncQueueWorker(new DecodeBufferWorker(callback, jpegBuff, decode_jpeg_buffer));
    return;;
}

NAN_METHOD(decodePngBuffer) {
    Nan::HandleScope scope;

    Local<Object> pngBuff = info[0].As<Object>();
    Nan::Callback * callback = new Nan::Callback(info[1].As<Function>());

    Nan::AsyncQueueWorker(new DecodeBufferWorker(callback, pngBuff, decode_png_buffer));
    return;;
}

NAN_METHOD(decodeGifBuffer) {
    Nan::HandleScope scope;

    Local<Object> gifBuff = info[0].As<Object>();
    Nan::Callback * callback = new Nan::Callback(info[1].As<Function>());

    Nan::AsyncQueueWorker(new DecodeBufferWorker(callback, gifBuff, decode_gif_buffer));
    return;;
}

// create an init function for our node module
void InitAll(Handle<Object> exports) {
    exports->Set(Nan::New("jpeg").ToLocalChecked(),
                 Nan::New<FunctionTemplate>(decodeJpegBuffer)->GetFunction());
    exports->Set(Nan::New("png").ToLocalChecked(),
                 Nan::New<FunctionTemplate>(decodePngBuffer)->GetFunction());
    exports->Set(Nan::New("gif").ToLocalChecked(),
                 Nan::New<FunctionTemplate>(decodeGifBuffer)->GetFunction());
}

// use NODE_MODULE macro to register our module:
NODE_MODULE(lwip_decoder, InitAll)
