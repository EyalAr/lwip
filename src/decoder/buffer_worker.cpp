#include "decoder.h"

DecodeBufferWorker::DecodeBufferWorker(NanCallback * callback, char * buffer, string type)
    : NanAsyncWorker(callback), _buffer(buffer), _type(type), _pixbuf(NULL),
      _width(0), _height(0), _channels(0), _trans(false) {}

DecodeBufferWorker::~DecodeBufferWorker() {}

void DecodeBufferWorker::Execute () {
    SetErrorMessage("Decoding buffers is not yet supported");
    return;
}

void DecodeBufferWorker::HandleOKCallback () {
    NanScope();
    Local<Value> argv[] = {
        NanNull(),
        NanNull()
    };
    callback->Call(2, argv);
}

NAN_METHOD(decodeJpegBuffer) {
    NanScope();

    Local<Object> jpegBuff = args[0].As<Object>();
    char * buffer = Buffer::Data(jpegBuff);
    NanCallback * callback = new NanCallback(args[1].As<Function>());

    NanAsyncQueueWorker(new DecodeBufferWorker(callback, buffer, "jpeg"));
    NanReturnUndefined();
}

NAN_METHOD(decodePngBuffer) {
    NanScope();

    Local<Object> pngBuff = args[0].As<Object>();
    char * buffer = Buffer::Data(pngBuff);
    NanCallback * callback = new NanCallback(args[1].As<Function>());

    NanAsyncQueueWorker(new DecodeBufferWorker(callback, buffer, "png"));
    NanReturnUndefined();
}
