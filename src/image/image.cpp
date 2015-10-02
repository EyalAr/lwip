#include "image.h"

Nan::Persistent<FunctionTemplate> LwipImage::constructor;

void LwipImage::Init(Handle<Object> exports) {
    // Prepare constructor template
    Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    tpl->SetClassName(Nan::New("LwipImage").ToLocalChecked());
    Nan::SetPrototypeMethod(tpl, "width", width);
    Nan::SetPrototypeMethod(tpl, "height", height);
    Nan::SetPrototypeMethod(tpl, "getPixel", getPixel);
    Nan::SetPrototypeMethod(tpl, "buffer", buffer);
    Nan::SetPrototypeMethod(tpl, "resize", resize);
    Nan::SetPrototypeMethod(tpl, "rotate", rotate);
    Nan::SetPrototypeMethod(tpl, "blur", blur);
    Nan::SetPrototypeMethod(tpl, "crop", crop);
    Nan::SetPrototypeMethod(tpl, "mirror", mirror);
    Nan::SetPrototypeMethod(tpl, "pad", pad);
    Nan::SetPrototypeMethod(tpl, "sharpen", sharpen);
    Nan::SetPrototypeMethod(tpl, "hslaAdj", hslaAdj);
    Nan::SetPrototypeMethod(tpl, "opacify", opacify);
    Nan::SetPrototypeMethod(tpl, "paste", paste);
    Nan::SetPrototypeMethod(tpl, "setPixel", setPixel);
    constructor.Reset(tpl);
    exports->Set(
        Nan::New("LwipImage").ToLocalChecked(),
        Nan::New<FunctionTemplate>(constructor)->GetFunction()
    );
}

LwipImage::LwipImage(unsigned char * data, size_t width, size_t height) {
    // TODO: CImg constructor may throw an exception. handle it in LwipImage::New.
    _cimg = new CImg<unsigned char>(data, width, height, 1, N_CHANNELS, false);
}

LwipImage::~LwipImage() {
    delete _cimg;
};

Handle<Value> LwipImage::NewInstance() {
    Nan::EscapableHandleScope scope;
    Local<FunctionTemplate> constructorHandle = Nan::New<FunctionTemplate>(constructor);
    Local<Object> instance = constructorHandle->GetFunction()->NewInstance();
    return scope.Escape(instance);
}

NAN_METHOD(LwipImage::New) {
    Nan::HandleScope();
    // info[0] - pixels buffer
    // info[1,2] - width and height
    Local<Object> pixBuff = info[0].As<Object>();
    size_t width = info[1]->NumberValue();
    size_t height = info[2]->NumberValue();
    unsigned char * pixels = (unsigned char *)Buffer::Data(pixBuff);
    // TODO: handle CImg exception
    LwipImage * obj = new LwipImage(pixels, width, height);
    obj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
}

// IMAGE JS OBJECT METHODS:
// ========================

// image.width():
// --------------
NAN_METHOD(LwipImage::width) {
    Nan::HandleScope();
    LwipImage * obj = ObjectWrap::Unwrap<LwipImage>(info.Holder());
    info.GetReturnValue().Set(Nan::New<Number>(obj->_cimg->width()));
}

// image.height():
// ---------------
NAN_METHOD(LwipImage::height) {
    Nan::HandleScope();
    LwipImage * obj = ObjectWrap::Unwrap<LwipImage>(info.Holder());
    info.GetReturnValue().Set(Nan::New<Number>(obj->_cimg->height()));
}

// image.getPixel(left, top):
// ---------------
NAN_METHOD(LwipImage::getPixel) {
    Nan::HandleScope();
    size_t left = (size_t) info[0].As<Number>()->Value();
    size_t top = (size_t) info[1].As<Number>()->Value();
    LwipImage * obj = ObjectWrap::Unwrap<LwipImage>(info.Holder());
    Local<Array> rgba = Nan::New<Array>(4);
    rgba->Set(0, Nan::New((*(obj->_cimg))(left, top, 0, 0))); // red
    rgba->Set(1, Nan::New((*(obj->_cimg))(left, top, 0, 1))); // green
    rgba->Set(2, Nan::New((*(obj->_cimg))(left, top, 0, 2))); // blue
    rgba->Set(3, Nan::New((*(obj->_cimg))(left, top, 0, 3))); // alpha
    info.GetReturnValue().Set(rgba);
}

// image.buffer():
// ---------------
NAN_METHOD(LwipImage::buffer) {
    Nan::HandleScope();
    LwipImage * obj = ObjectWrap::Unwrap<LwipImage>(info.Holder());
    // return a new buffer. don't use same memory an image. make a copy.
    // image object may be gc'ed, but buffer needs to stay alive.
    info.GetReturnValue().Set(Nan::CopyBuffer((char *)obj->_cimg->data(), obj->_cimg->size()).ToLocalChecked());
}

// image.resize(width, height, inter, callback):
// ---------------------------------------------

// info[0] - width
// info[1] - height
// info[2] - inter(polation)
// info[3] - callback
NAN_METHOD(LwipImage::resize) {
    Nan::HandleScope();

    size_t width = info[0].As<Integer>()->Value();
    size_t height = info[1].As<Integer>()->Value();
    int inter = info[2].As<Integer>()->Value();
    Nan::Callback * callback = new Nan::Callback(info[3].As<Function>());
    CImg<unsigned char> * cimg = ObjectWrap::Unwrap<LwipImage>(info.This())->_cimg;

    Nan::AsyncQueueWorker(
        new ResizeWorker(
            width,
            height,
            inter,
            cimg,
            callback
        )
    );

    return;
}

// image.rotate(degs, inter, callback):
// ------------------------------------

// info[0] - degs
// info[1] - R
// info[2] - G
// info[3] - B
// info[4] - A
// info[5] - callback
NAN_METHOD(LwipImage::rotate) {
    Nan::HandleScope();

    float degs = (float) info[0].As<Number>()->Value();
    unsigned char r = (unsigned char) info[1].As<Integer>()->Value();
    unsigned char g = (unsigned char) info[2].As<Integer>()->Value();
    unsigned char b = (unsigned char) info[3].As<Integer>()->Value();
    unsigned char a = (unsigned char) info[4].As<Integer>()->Value();
    Nan::Callback * callback = new Nan::Callback(info[5].As<Function>());
    CImg<unsigned char> * cimg = ObjectWrap::Unwrap<LwipImage>(info.This())->_cimg;

    Nan::AsyncQueueWorker(
        new RotateWorker(
            degs,
            r,
            g,
            b,
            a,
            cimg,
            callback
        )
    );

    return;
}

// image.blur(sigma, callback):
// ----------------------------

// info[0] - sigma
// info[1] - callback
NAN_METHOD(LwipImage::blur) {
    Nan::HandleScope();

    float sigma = (float) info[0].As<Number>()->Value();
    Nan::Callback * callback = new Nan::Callback(info[1].As<Function>());
    CImg<unsigned char> * cimg = ObjectWrap::Unwrap<LwipImage>(info.This())->_cimg;

    Nan::AsyncQueueWorker(
        new BlurWorker(
            sigma,
            cimg,
            callback
        )
    );

    return;
}

// image.crop(left, top, right, bottom, callback):
// -----------------------------------------------

// info[0] - left
// info[1] - top
// info[2] - right
// info[3] - bottom
// info[4] - callback
NAN_METHOD(LwipImage::crop) {
    Nan::HandleScope();

    size_t left = (size_t) info[0].As<Number>()->Value();
    size_t top = (size_t) info[1].As<Number>()->Value();
    size_t right = (size_t) info[2].As<Number>()->Value();
    size_t bottom = (size_t) info[3].As<Number>()->Value();
    Nan::Callback * callback = new Nan::Callback(info[4].As<Function>());
    CImg<unsigned char> * cimg = ObjectWrap::Unwrap<LwipImage>(info.This())->_cimg;

    Nan::AsyncQueueWorker(
        new CropWorker(
            left,
            top,
            right,
            bottom,
            cimg,
            callback
        )
    );

    return;
}

// image.mirror(xaxis, yaxis, callback):
// -----------------------------------------------

// info[0] - xaxis (boolean)
// info[1] - yaxis (boolean)
// info[2] - callback
NAN_METHOD(LwipImage::mirror) {
    Nan::HandleScope();

    bool xaxis = info[0]->BooleanValue();
    bool yaxis = info[1]->BooleanValue();
    Nan::Callback * callback = new Nan::Callback(info[2].As<Function>());
    CImg<unsigned char> * cimg = ObjectWrap::Unwrap<LwipImage>(info.This())->_cimg;

    Nan::AsyncQueueWorker(
        new MirrorWorker(
            xaxis,
            yaxis,
            cimg,
            callback
        )
    );

    return;
}

// image.pad(left, top, right, bottom, color, callback):
// -----------------------------------------------------

// info[0] - left
// info[1] - top
// info[2] - right
// info[3] - bottom
// info[4] - r
// info[5] - g
// info[6] - b
// info[7] - a
// info[8] - callback
NAN_METHOD(LwipImage::pad) {
    Nan::HandleScope();

    size_t left = (size_t) info[0].As<Number>()->Value();
    size_t top = (size_t) info[1].As<Number>()->Value();
    size_t right = (size_t) info[2].As<Number>()->Value();
    size_t bottom = (size_t) info[3].As<Number>()->Value();
    unsigned char r = (unsigned char) info[4].As<Integer>()->Value(),
                  g = (unsigned char) info[5].As<Integer>()->Value(),
                  b = (unsigned char) info[6].As<Integer>()->Value(),
                  a = (unsigned char) info[7].As<Integer>()->Value();
    Nan::Callback * callback = new Nan::Callback(info[8].As<Function>());
    CImg<unsigned char> * cimg = ObjectWrap::Unwrap<LwipImage>(info.This())->_cimg;

    Nan::AsyncQueueWorker(
        new PadWorker(
            left,
            top,
            right,
            bottom,
            r,
            g,
            b,
            a,
            cimg,
            callback
        )
    );

    return;
}

// image.sharpen(amplitude, callback):
// -----------------------------------

// info[0] - amplitude
// info[1] - callback
NAN_METHOD(LwipImage::sharpen) {
    Nan::HandleScope();

    float amp = (float) info[0].As<Number>()->Value();
    Nan::Callback * callback = new Nan::Callback(info[1].As<Function>());
    CImg<unsigned char> * cimg = ObjectWrap::Unwrap<LwipImage>(info.This())->_cimg;

    Nan::AsyncQueueWorker(
        new SharpenWorker(
            amp,
            cimg,
            callback
        )
    );

    return;
}

// image.hslaAdj(hd, sd, ld, callback):
// ------------------------------------

// info[0] - hue delta
// info[1] - saturation delta
// info[2] - lightness delta
// info[3] - alpha delta
// info[4] - callback
NAN_METHOD(LwipImage::hslaAdj) {
    Nan::HandleScope();

    float hd = (float) info[0].As<Number>()->Value();
    float sd = (float) info[1].As<Number>()->Value();
    float ld = (float) info[2].As<Number>()->Value();
    float ad = (float) info[3].As<Number>()->Value();
    Nan::Callback * callback = new Nan::Callback(info[4].As<Function>());
    CImg<unsigned char> * cimg = ObjectWrap::Unwrap<LwipImage>(info.This())->_cimg;

    Nan::AsyncQueueWorker(
        new HSLAWorker(
            hd,
            sd,
            ld,
            ad,
            cimg,
            callback
        )
    );

    return;
}

// image.opacify(callback):
// ------------------------------------

// info[0] - callback
NAN_METHOD(LwipImage::opacify) {
    Nan::HandleScope();

    Nan::Callback * callback = new Nan::Callback(info[0].As<Function>());
    CImg<unsigned char> * cimg = ObjectWrap::Unwrap<LwipImage>(info.This())->_cimg;

    Nan::AsyncQueueWorker(
        new OpacifyWorker(
            cimg,
            callback
        )
    );

    return;
}

// image.paste(callback):
// ------------------------------------

// info[0] - left
// info[1] - top
// info[2] - buffer
// info[3] - width
// info[4] - height
// info[5] - callback
NAN_METHOD(LwipImage::paste) {
    Nan::HandleScope();

    size_t left = (size_t) info[0].As<Number>()->Value();
    size_t top = (size_t) info[1].As<Number>()->Value();
    Local<Object> pixBuff = info[2].As<Object>();
    size_t width = (size_t) info[3].As<Number>()->Value();
    size_t height = (size_t) info[4].As<Number>()->Value();
    Nan::Callback * callback = new Nan::Callback(info[5].As<Function>());
    CImg<unsigned char> * cimg = ObjectWrap::Unwrap<LwipImage>(info.This())->_cimg;

    Nan::AsyncQueueWorker(
        new PasteWorker(
            left,
            top,
            pixBuff,
            width,
            height,
            cimg,
            callback
        )
    );

    return;
}

// image.setPixel(left, top, color, callback):
// -------------------------------------------

// info[0] - left
// info[1] - top
// info[2] - red
// info[3] - green
// info[4] - blue
// info[5] - alpha
// info[6] - callback
NAN_METHOD(LwipImage::setPixel) {
    Nan::HandleScope();

    size_t left = (size_t) info[0].As<Number>()->Value();
    size_t top = (size_t) info[1].As<Number>()->Value();
    unsigned char r = (unsigned char) info[2].As<Integer>()->Value();
    unsigned char g = (unsigned char) info[3].As<Integer>()->Value();
    unsigned char b = (unsigned char) info[4].As<Integer>()->Value();
    unsigned char a = (unsigned char) info[5].As<Integer>()->Value();
    Nan::Callback * callback = new Nan::Callback(info[6].As<Function>());
    CImg<unsigned char> * cimg = ObjectWrap::Unwrap<LwipImage>(info.This())->_cimg;

    Nan::AsyncQueueWorker(
        new SetPixelWorker(
            left,
            top,
            r,
            g,
            b,
            a,
            cimg,
            callback
        )
    );

    return;
}
