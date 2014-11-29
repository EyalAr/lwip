#include "image.h"

Persistent<FunctionTemplate> LwipImage::constructor;

void LwipImage::Init(Handle<Object> exports) {
    // Prepare constructor template
    Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    tpl->SetClassName(NanNew("LwipImage"));
    NODE_SET_PROTOTYPE_METHOD(tpl, "width", width);
    NODE_SET_PROTOTYPE_METHOD(tpl, "height", height);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getPixel", getPixel);
    NODE_SET_PROTOTYPE_METHOD(tpl, "buffer", buffer);
    NODE_SET_PROTOTYPE_METHOD(tpl, "resize", resize);
    NODE_SET_PROTOTYPE_METHOD(tpl, "rotate", rotate);
    NODE_SET_PROTOTYPE_METHOD(tpl, "blur", blur);
    NODE_SET_PROTOTYPE_METHOD(tpl, "crop", crop);
    NODE_SET_PROTOTYPE_METHOD(tpl, "mirror", mirror);
    NODE_SET_PROTOTYPE_METHOD(tpl, "pad", pad);
    NODE_SET_PROTOTYPE_METHOD(tpl, "sharpen", sharpen);
    NODE_SET_PROTOTYPE_METHOD(tpl, "hslaAdj", hslaAdj);
    NODE_SET_PROTOTYPE_METHOD(tpl, "opacify", opacify);
    NODE_SET_PROTOTYPE_METHOD(tpl, "paste", paste);
    NODE_SET_PROTOTYPE_METHOD(tpl, "setPixel", setPixel);
    NanAssignPersistent(constructor, tpl);
    exports->Set(
        NanNew("LwipImage"),
        NanNew<FunctionTemplate>(constructor)->GetFunction()
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
    NanEscapableScope();
    Local<FunctionTemplate> constructorHandle = NanNew<FunctionTemplate>(constructor);
    Local<Object> instance = constructorHandle->GetFunction()->NewInstance();
    return NanEscapeScope(instance);
}

NAN_METHOD(LwipImage::New) {
    NanScope();
    // args[0] - pixels buffer
    // args[1,2] - width and height
    Local<Object> pixBuff = args[0].As<Object>();
    size_t width = args[1]->NumberValue();
    size_t height = args[2]->NumberValue();
    unsigned char * pixels = (unsigned char *)Buffer::Data(pixBuff);
    // TODO: handle CImg exception
    LwipImage * obj = new LwipImage(pixels, width, height);
    obj->Wrap(args.This());
    NanReturnValue(args.This());
}

// IMAGE JS OBJECT METHODS:
// ========================

// image.width():
// --------------
NAN_METHOD(LwipImage::width) {
    NanScope();
    LwipImage * obj = ObjectWrap::Unwrap<LwipImage>(args.Holder());
    NanReturnValue(NanNew<Number>(obj->_cimg->width()));
}

// image.height():
// ---------------
NAN_METHOD(LwipImage::height) {
    NanScope();
    LwipImage * obj = ObjectWrap::Unwrap<LwipImage>(args.Holder());
    NanReturnValue(NanNew<Number>(obj->_cimg->height()));
}

// image.getPixel(left, top):
// ---------------
NAN_METHOD(LwipImage::getPixel) {
    NanScope();
    size_t left = (size_t) args[0].As<Number>()->Value();
    size_t top = (size_t) args[1].As<Number>()->Value();
    LwipImage * obj = ObjectWrap::Unwrap<LwipImage>(args.Holder());
    Local<Array> rgba = NanNew<Array>(4);
    rgba->Set(0, NanNew((*(obj->_cimg))(left, top, 0, 0))); // red
    rgba->Set(1, NanNew((*(obj->_cimg))(left, top, 0, 1))); // green
    rgba->Set(2, NanNew((*(obj->_cimg))(left, top, 0, 2))); // blue
    rgba->Set(3, NanNew((*(obj->_cimg))(left, top, 0, 3))); // alpha
    NanReturnValue(rgba);
}

// image.buffer():
// ---------------
NAN_METHOD(LwipImage::buffer) {
    NanScope();
    LwipImage * obj = ObjectWrap::Unwrap<LwipImage>(args.Holder());
    // return a new buffer. don't use same memory an image. make a copy.
    // image object may be gc'ed, but buffer needs to stay alive.
    NanReturnValue(NanNewBufferHandle((char *)obj->_cimg->data(), obj->_cimg->size()));
}

// image.resize(width, height, inter, callback):
// ---------------------------------------------

// args[0] - width
// args[1] - height
// args[2] - inter(polation)
// args[3] - callback
NAN_METHOD(LwipImage::resize) {
    NanScope();

    size_t width = args[0].As<Integer>()->Value();
    size_t height = args[1].As<Integer>()->Value();
    int inter = args[2].As<Integer>()->Value();
    NanCallback * callback = new NanCallback(args[3].As<Function>());
    CImg<unsigned char> * cimg = ObjectWrap::Unwrap<LwipImage>(args.This())->_cimg;

    NanAsyncQueueWorker(
        new ResizeWorker(
            width,
            height,
            inter,
            cimg,
            callback
        )
    );

    NanReturnUndefined();
}

// image.rotate(degs, inter, callback):
// ------------------------------------

// args[0] - degs
// args[1] - R
// args[2] - G
// args[3] - B
// args[4] - A
// args[5] - callback
NAN_METHOD(LwipImage::rotate) {
    NanScope();

    float degs = (float) args[0].As<Number>()->Value();
    unsigned char r = (unsigned char) args[1].As<Integer>()->Value();
    unsigned char g = (unsigned char) args[2].As<Integer>()->Value();
    unsigned char b = (unsigned char) args[3].As<Integer>()->Value();
    unsigned char a = (unsigned char) args[4].As<Integer>()->Value();
    NanCallback * callback = new NanCallback(args[5].As<Function>());
    CImg<unsigned char> * cimg = ObjectWrap::Unwrap<LwipImage>(args.This())->_cimg;

    NanAsyncQueueWorker(
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

    NanReturnUndefined();
}

// image.blur(sigma, callback):
// ----------------------------

// args[0] - sigma
// args[1] - callback
NAN_METHOD(LwipImage::blur) {
    NanScope();

    float sigma = (float) args[0].As<Number>()->Value();
    NanCallback * callback = new NanCallback(args[1].As<Function>());
    CImg<unsigned char> * cimg = ObjectWrap::Unwrap<LwipImage>(args.This())->_cimg;

    NanAsyncQueueWorker(
        new BlurWorker(
            sigma,
            cimg,
            callback
        )
    );

    NanReturnUndefined();
}

// image.crop(left, top, right, bottom, callback):
// -----------------------------------------------

// args[0] - left
// args[1] - top
// args[2] - right
// args[3] - bottom
// args[4] - callback
NAN_METHOD(LwipImage::crop) {
    NanScope();

    size_t left = (size_t) args[0].As<Number>()->Value();
    size_t top = (size_t) args[1].As<Number>()->Value();
    size_t right = (size_t) args[2].As<Number>()->Value();
    size_t bottom = (size_t) args[3].As<Number>()->Value();
    NanCallback * callback = new NanCallback(args[4].As<Function>());
    CImg<unsigned char> * cimg = ObjectWrap::Unwrap<LwipImage>(args.This())->_cimg;

    NanAsyncQueueWorker(
        new CropWorker(
            left,
            top,
            right,
            bottom,
            cimg,
            callback
        )
    );

    NanReturnUndefined();
}

// image.mirror(xaxis, yaxis, callback):
// -----------------------------------------------

// args[0] - xaxis (boolean)
// args[1] - yaxis (boolean)
// args[2] - callback
NAN_METHOD(LwipImage::mirror) {
    NanScope();

    bool xaxis = args[0]->BooleanValue();
    bool yaxis = args[1]->BooleanValue();
    NanCallback * callback = new NanCallback(args[2].As<Function>());
    CImg<unsigned char> * cimg = ObjectWrap::Unwrap<LwipImage>(args.This())->_cimg;

    NanAsyncQueueWorker(
        new MirrorWorker(
            xaxis,
            yaxis,
            cimg,
            callback
        )
    );

    NanReturnUndefined();
}

// image.pad(left, top, right, bottom, color, callback):
// -----------------------------------------------------

// args[0] - left
// args[1] - top
// args[2] - right
// args[3] - bottom
// args[4] - r
// args[5] - g
// args[6] - b
// args[7] - a
// args[8] - callback
NAN_METHOD(LwipImage::pad) {
    NanScope();

    size_t left = (size_t) args[0].As<Number>()->Value();
    size_t top = (size_t) args[1].As<Number>()->Value();
    size_t right = (size_t) args[2].As<Number>()->Value();
    size_t bottom = (size_t) args[3].As<Number>()->Value();
    unsigned char r = (unsigned char) args[4].As<Integer>()->Value(),
                  g = (unsigned char) args[5].As<Integer>()->Value(),
                  b = (unsigned char) args[6].As<Integer>()->Value(),
                  a = (unsigned char) args[7].As<Integer>()->Value();
    NanCallback * callback = new NanCallback(args[8].As<Function>());
    CImg<unsigned char> * cimg = ObjectWrap::Unwrap<LwipImage>(args.This())->_cimg;

    NanAsyncQueueWorker(
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

    NanReturnUndefined();
}

// image.sharpen(amplitude, callback):
// -----------------------------------

// args[0] - amplitude
// args[1] - callback
NAN_METHOD(LwipImage::sharpen) {
    NanScope();

    float amp = (float) args[0].As<Number>()->Value();
    NanCallback * callback = new NanCallback(args[1].As<Function>());
    CImg<unsigned char> * cimg = ObjectWrap::Unwrap<LwipImage>(args.This())->_cimg;

    NanAsyncQueueWorker(
        new SharpenWorker(
            amp,
            cimg,
            callback
        )
    );

    NanReturnUndefined();
}

// image.hslaAdj(hd, sd, ld, callback):
// ------------------------------------

// args[0] - hue delta
// args[1] - saturation delta
// args[2] - lightness delta
// args[3] - alpha delta
// args[4] - callback
NAN_METHOD(LwipImage::hslaAdj) {
    NanScope();

    float hd = (float) args[0].As<Number>()->Value();
    float sd = (float) args[1].As<Number>()->Value();
    float ld = (float) args[2].As<Number>()->Value();
    float ad = (float) args[3].As<Number>()->Value();
    NanCallback * callback = new NanCallback(args[4].As<Function>());
    CImg<unsigned char> * cimg = ObjectWrap::Unwrap<LwipImage>(args.This())->_cimg;

    NanAsyncQueueWorker(
        new HSLAWorker(
            hd,
            sd,
            ld,
            ad,
            cimg,
            callback
        )
    );

    NanReturnUndefined();
}

// image.opacify(callback):
// ------------------------------------

// args[0] - callback
NAN_METHOD(LwipImage::opacify) {
    NanScope();

    NanCallback * callback = new NanCallback(args[0].As<Function>());
    CImg<unsigned char> * cimg = ObjectWrap::Unwrap<LwipImage>(args.This())->_cimg;

    NanAsyncQueueWorker(
        new OpacifyWorker(
            cimg,
            callback
        )
    );

    NanReturnUndefined();
}

// image.paste(callback):
// ------------------------------------

// args[0] - left
// args[1] - top
// args[2] - buffer
// args[3] - width
// args[4] - height
// args[5] - callback
NAN_METHOD(LwipImage::paste) {
    NanScope();

    size_t left = (size_t) args[0].As<Number>()->Value();
    size_t top = (size_t) args[1].As<Number>()->Value();
    Local<Object> pixBuff = args[2].As<Object>();
    size_t width = (size_t) args[3].As<Number>()->Value();
    size_t height = (size_t) args[4].As<Number>()->Value();
    NanCallback * callback = new NanCallback(args[5].As<Function>());
    CImg<unsigned char> * cimg = ObjectWrap::Unwrap<LwipImage>(args.This())->_cimg;

    NanAsyncQueueWorker(
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

    NanReturnUndefined();
}

// image.setPixel(left, top, color, callback):
// -------------------------------------------

// args[0] - left
// args[1] - top
// args[2] - red
// args[3] - green
// args[4] - blue
// args[5] - alpha
// args[6] - callback
NAN_METHOD(LwipImage::setPixel) {
    NanScope();

    size_t left = (size_t) args[0].As<Number>()->Value();
    size_t top = (size_t) args[1].As<Number>()->Value();
    unsigned char r = (unsigned char) args[2].As<Integer>()->Value();
    unsigned char g = (unsigned char) args[3].As<Integer>()->Value();
    unsigned char b = (unsigned char) args[4].As<Integer>()->Value();
    unsigned char a = (unsigned char) args[5].As<Integer>()->Value();
    NanCallback * callback = new NanCallback(args[6].As<Function>());
    CImg<unsigned char> * cimg = ObjectWrap::Unwrap<LwipImage>(args.This())->_cimg;

    NanAsyncQueueWorker(
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

    NanReturnUndefined();
}
