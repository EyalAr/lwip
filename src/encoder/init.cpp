#include "encoder.h"

// encoder.jpeg(pixbuf, width, height, quality, callback)
NAN_METHOD(encodeToJpegBuffer) {
    Nan::HandleScope scope;

    Local<Object> buff = info[0].As<Object>();
    size_t width = info[1].As<Integer>()->Value();
    size_t height = info[2].As<Integer>()->Value();
    int quality = info[3].As<Integer>()->Value();
    Nan::Callback * callback = new Nan::Callback(info[4].As<Function>());

    Nan::AsyncQueueWorker(
        new EncodeToJpegBufferWorker(
            buff,
            width,
            height,
            quality,
            callback
        ));
    return;;
}

// encoder.png(pixbuf, width, height, compression, interlaced, trans, metadata, callback)
NAN_METHOD(encodeToPngBuffer) {
    Nan::HandleScope scope;

    Local<Object> buff = info[0].As<Object>();
    size_t width = info[1].As<Integer>()->Value();
    size_t height = info[2].As<Integer>()->Value();
    int compression = info[3].As<Integer>()->Value();
    bool interlaced = info[4]->BooleanValue();
    bool trans = info[5]->BooleanValue();

    char * metadata;

    if (info[6]->IsNull() || info[6]->IsUndefined()) {
        metadata = NULL;
    } else {
        int metadata_len = info[6].As<String>()->Utf8Length();
        metadata = (char *)malloc((metadata_len + 1) * sizeof(char));
        info[6].As<String>()->WriteUtf8(metadata);
    }

    Nan::Callback * callback = new Nan::Callback(info[7].As<Function>());

    Nan::AsyncQueueWorker(
        new EncodeToPngBufferWorker(
            buff,
            width,
            height,
            compression,
            interlaced,
            trans,
            metadata,
            callback
        )
    );
    return;;
}

// encoder.gif(pixbuf, width, height, cmapSize, colors, interlaced, trans, threshold, callback)
NAN_METHOD(encodeToGifBuffer) {
    Nan::HandleScope scope;

    Local<Object> buff = info[0].As<Object>();
    size_t width = info[1].As<Integer>()->Value();
    size_t height = info[2].As<Integer>()->Value();
    int cmapSize = info[3].As<Integer>()->Value();
    int colors = info[4].As<Integer>()->Value();
    bool interlaced = info[5]->BooleanValue();
    bool trans = info[6]->BooleanValue();
    int threshold = info[7].As<Integer>()->Value();
    Nan::Callback * callback = new Nan::Callback(info[8].As<Function>());

    Nan::AsyncQueueWorker(
        new EncodeToGifBufferWorker(
            buff,
            width,
            height,
            cmapSize,
            colors,
            interlaced,
            trans,
            threshold,
            callback
        )
    );
    return;;
}

// create an init function for our node module
void InitAll(Handle<Object> exports) {
    exports->Set(Nan::New("jpeg").ToLocalChecked(),
                 Nan::New<FunctionTemplate>(encodeToJpegBuffer)->GetFunction());
    exports->Set(Nan::New("png").ToLocalChecked(),
                 Nan::New<FunctionTemplate>(encodeToPngBuffer)->GetFunction());
    exports->Set(Nan::New("gif").ToLocalChecked(),
                 Nan::New<FunctionTemplate>(encodeToGifBuffer)->GetFunction());
}

// use NODE_MODULE macro to register our module:
NODE_MODULE(lwip_encoder, InitAll)
