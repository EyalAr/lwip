#include "encoder.h"

// encoder.jpeg(pixbuf, width, height, quality, callback)
NAN_METHOD(encodeToJpegBuffer) {
    Nan::HandleScope();

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
}

// encoder.png(pixbuf, width, height, compression, interlaced, trans, metadata, callback)
NAN_METHOD(encodeToPngBuffer) {
    Nan::HandleScope();

    Local<Object> buff = info[0].As<Object>();
    size_t width = info[1].As<Integer>()->Value();
    size_t height = info[2].As<Integer>()->Value();
    int compression = info[3].As<Integer>()->Value();
    bool interlaced = info[4].As<Boolean>()->Value();
    bool trans = info[5].As<Boolean>()->Value();

    char * metadata;

    if (info[6]->IsNull() || info[6]->IsUndefined()) {
        metadata = NULL;
    } else {
        if (!info[6]->IsString())
        {
            Nan::ThrowError("Metadata should be a string");
            return;
        }

        int metadata_len = Nan::DecodeBytes(info[6], Nan::Encoding::UTF8);
        metadata = (char *)malloc((metadata_len + 1) * sizeof(char));
        memset(metadata, 0, (metadata_len + 1) * sizeof(char));
        Nan::DecodeWrite(metadata, metadata_len, info[6], Nan::Encoding::UTF8);
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
}

// encoder.gif(pixbuf, width, height, cmapSize, colors, interlaced, trans, threshold, callback)
NAN_METHOD(encodeToGifBuffer) {
    Nan::HandleScope();

    Local<Object> buff = info[0].As<Object>();
    size_t width = info[1].As<Integer>()->Value();
    size_t height = info[2].As<Integer>()->Value();
    int cmapSize = info[3].As<Integer>()->Value();
    int colors = info[4].As<Integer>()->Value();
    bool interlaced = info[5].As<Boolean>()->Value();
    bool trans = info[6].As<Boolean>()->Value();
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
}

// create an init function for our node module
NAN_MODULE_INIT(InitAll) {
    Nan::Export(target, "jpeg", encodeToJpegBuffer);
    Nan::Export(target, "png", encodeToPngBuffer);
    Nan::Export(target, "gif", encodeToGifBuffer);
}

// use NODE_MODULE macro to register our module:
NODE_MODULE(lwip_encoder, InitAll)
