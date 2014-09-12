#include "image.h"

Persistent<FunctionTemplate> LwipImage::constructor;

void LwipImage::Init(Handle<Object> exports) {
    // Prepare constructor template
    Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    tpl->SetClassName(NanNew("LwipImage"));
    NODE_SET_PROTOTYPE_METHOD(tpl, "width", width);
    NODE_SET_PROTOTYPE_METHOD(tpl, "height", height);
    NODE_SET_PROTOTYPE_METHOD(tpl, "buffer", buffer);
    NODE_SET_PROTOTYPE_METHOD(tpl, "resize", resize);
    NODE_SET_PROTOTYPE_METHOD(tpl, "rotate", rotate);
    NODE_SET_PROTOTYPE_METHOD(tpl, "blur", blur);
    NODE_SET_PROTOTYPE_METHOD(tpl, "crop", crop);
    NODE_SET_PROTOTYPE_METHOD(tpl, "mirror", mirror);
    NODE_SET_PROTOTYPE_METHOD(tpl, "pad", pad);
    NanAssignPersistent(constructor, tpl);
    exports->Set(
        NanNew("LwipImage"),
        NanNew<FunctionTemplate>(constructor)->GetFunction()
    );
}

LwipImage::LwipImage(unsigned char * data, size_t width, size_t height) {
    // TODO: CImg constructor may throw an exception. handle it in LwipImage::New.
    _cimg = new CImg<unsigned char>(data, width, height, 1, 3, false);
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
Handle<Value> LwipImage::height(const Arguments & args) {
    NanScope();
    LwipImage * obj = ObjectWrap::Unwrap<LwipImage>(args.Holder());
    NanReturnValue(NanNew<Number>(obj->_cimg->height()));
}

// image.buffer():
// ---------------
Handle<Value> LwipImage::buffer(const Arguments & args) {
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
Handle<Value> LwipImage::resize(const Arguments & args) {
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
// args[4] - callback
Handle<Value> LwipImage::rotate(const Arguments & args) {
    HandleScope scope;
    // (arguments validation is done in JS land)
    rotateBaton * rb = new rotateBaton();
    if (rb == NULL) {
        ThrowException(Exception::TypeError(NanNew<String>("Out of memory")));
        return scope.Close(Undefined());
    }
    rb->request.data = rb;
    rb->cb = Persistent<Function>::New(Local<Function>::Cast(args[4]));
    rb->degs = (float) args[0]->NumberValue();
    rb->color[0] = (unsigned char) args[1]->NumberValue();
    rb->color[1] = (unsigned char) args[2]->NumberValue();
    rb->color[2] = (unsigned char) args[3]->NumberValue();
    rb->img = ObjectWrap::Unwrap<LwipImage>(args.This());
    rb->err = false;
    uv_queue_work(uv_default_loop(), &rb->request, rotateAsync, rotateAsyncDone);
    return scope.Close(Undefined());
}

void rotateAsync(uv_work_t * request) {
    rotateBaton * rb = static_cast<rotateBaton *>(request->data);
    const float nangle = cimg::mod(rb->degs, 360.0f);
    if (cimg::mod(nangle, 90.0f) != 0) {
        CImg<unsigned char> * res;
        unsigned int oldwidth = rb->img->_cimg->width(),
                     oldheight = rb->img->_cimg->height();
        try {
            // 2 pixels wider and taller
            res = new CImg<unsigned char>(oldwidth + 2, oldheight + 2, 1, 3);
        } catch (CImgException e) {
            rb->err = true;
            rb->errMsg = "Out of memory";
            return;
        }
        cimg_forX(*res, x) {
            res->fillC(x, 0, 0, rb->color[0], rb->color[1], rb->color[2]);
            res->fillC(x, oldheight + 1, 0, rb->color[0], rb->color[1], rb->color[2]);
        }
        cimg_forY(*res, y) {
            res->fillC(0, y, 0, rb->color[0], rb->color[1], rb->color[2]);
            res->fillC(oldwidth + 1, y, 0, rb->color[0], rb->color[1], rb->color[2]);
        }
        cimg_forXY(*(rb->img->_cimg), x, y) {
            unsigned char r = (*(rb->img->_cimg))(x, y, 0, 0),
                          g = (*(rb->img->_cimg))(x, y, 0, 1),
                          b = (*(rb->img->_cimg))(x, y, 0, 2);
            res->fillC(x + 1, y + 1, 0, r, g, b);
        }
        delete rb->img->_cimg;
        rb->img->_cimg = res;
    }
    try {
        // linear interpolations = 1
        // neumann boundary condition = 1
        rb->img->_cimg->rotate(nangle, 1, 1);
    } catch (CImgException e) {
        rb->err = true;
        rb->errMsg = "Unable to rotate image";
    }
    return;
}

void rotateAsyncDone(uv_work_t * request, int status) {
    // rotate completed. now we call the callback.
    rotateBaton * rb = static_cast<rotateBaton *>(request->data);
    if (rb->err) {
        const unsigned int argc = 1;
        Local<Value> argv[argc] = {
            Local<Value>::New(Exception::Error(NanNew<String>(rb->errMsg.c_str())))
        };
        rb->cb->Call(Context::GetCurrent()->Global(), argc, argv);
    } else {
        const unsigned int argc = 1;
        Handle<Value> argv[argc] = {
            Local<Value>::New(Null())
        };
        rb->cb->Call(Context::GetCurrent()->Global(), argc, argv);
    }
    // dispose of cb, because it's a persistent function
    rb->cb.Dispose();
    delete rb;
}

// image.blur(sigma, callback):
// ----------------------------

// args[0] - sigma
// args[1] - callback
Handle<Value> LwipImage::blur(const Arguments & args) {
    HandleScope scope;
    // (arguments validation is done in JS land)
    blurBaton * bb = new blurBaton();
    if (bb == NULL) {
        ThrowException(Exception::TypeError(NanNew<String>("Out of memory")));
        return scope.Close(Undefined());
    }
    bb->request.data = bb;
    bb->cb = Persistent<Function>::New(Local<Function>::Cast(args[1]));
    bb->sigma = (float) args[0]->NumberValue();
    bb->img = ObjectWrap::Unwrap<LwipImage>(args.This());
    bb->err = false;
    uv_queue_work(uv_default_loop(), &bb->request, blurAsync, blurAsyncDone);
    return scope.Close(Undefined());
}

void blurAsync(uv_work_t * request) {
    blurBaton * bb = static_cast<blurBaton *>(request->data);
    try {
        // filter order = 0 (on the image itself, not derivatives)
        bb->img->_cimg->vanvliet(bb->sigma, 0, 'x');
        bb->img->_cimg->vanvliet(bb->sigma, 0, 'y');
    } catch (CImgException e) {
        bb->err = true;
        bb->errMsg = "Unable to blur image";
    }
    return;
}

void blurAsyncDone(uv_work_t * request, int status) {
    // blur completed. now we call the callback.
    blurBaton * bb = static_cast<blurBaton *>(request->data);
    if (bb->err) {
        const unsigned int argc = 1;
        Local<Value> argv[argc] = {
            Local<Value>::New(Exception::Error(NanNew<String>(bb->errMsg.c_str())))
        };
        bb->cb->Call(Context::GetCurrent()->Global(), argc, argv);
    } else {
        const unsigned int argc = 1;
        Handle<Value> argv[argc] = {
            Local<Value>::New(Null())
        };
        bb->cb->Call(Context::GetCurrent()->Global(), argc, argv);
    }
    // dispose of cb, because it's a persistent function
    bb->cb.Dispose();
    delete bb;
}

// image.crop(left, top, right, bottom, callback):
// -----------------------------------------------

// args[0] - left
// args[1] - top
// args[2] - right
// args[3] - bottom
// args[4] - callback
Handle<Value> LwipImage::crop(const Arguments & args) {
    HandleScope scope;
    // (arguments validation is done in JS land)
    cropBaton * cb = new cropBaton();
    if (cb == NULL) {
        ThrowException(Exception::TypeError(NanNew<String>("Out of memory")));
        return scope.Close(Undefined());
    }
    cb->request.data = cb;
    cb->cb = Persistent<Function>::New(Local<Function>::Cast(args[4]));
    cb->left = (unsigned int) args[0]->NumberValue();
    cb->top = (unsigned int) args[1]->NumberValue();
    cb->right = (unsigned int) args[2]->NumberValue();
    cb->bottom = (unsigned int) args[3]->NumberValue();
    cb->img = ObjectWrap::Unwrap<LwipImage>(args.This());
    cb->err = false;
    uv_queue_work(uv_default_loop(), &cb->request, cropAsync, cropAsyncDone);
    return scope.Close(Undefined());
}

void cropAsync(uv_work_t * request) {
    cropBaton * cb = static_cast<cropBaton *>(request->data);
    try {
        cb->img->_cimg->crop(cb->left, cb->top, 0, 0, cb->right, cb->bottom, 0, 2);
    } catch (CImgException e) {
        cb->err = true;
        cb->errMsg = "Unable to crop image";
    }
    return;
}

void cropAsyncDone(uv_work_t * request, int status) {
    // crop completed. now we call the callback.
    cropBaton * cb = static_cast<cropBaton *>(request->data);
    if (cb->err) {
        const unsigned int argc = 1;
        Local<Value> argv[argc] = {
            Local<Value>::New(Exception::Error(NanNew<String>(cb->errMsg.c_str())))
        };
        cb->cb->Call(Context::GetCurrent()->Global(), argc, argv);
    } else {
        const unsigned int argc = 1;
        Handle<Value> argv[argc] = {
            Local<Value>::New(Null())
        };
        cb->cb->Call(Context::GetCurrent()->Global(), argc, argv);
    }
    // dispose of cb, because it's a persistent function
    cb->cb.Dispose();
    delete cb;
}

// image.mirror(xaxis, yaxis, callback):
// -----------------------------------------------

// args[0] - xaxis (boolean)
// args[1] - yaxis (boolean)
// args[2] - callback
Handle<Value> LwipImage::mirror(const Arguments & args) {
    HandleScope scope;
    // (arguments validation is done in JS land)
    mirrorBaton * mb = new mirrorBaton();
    if (mb == NULL) {
        ThrowException(Exception::TypeError(NanNew<String>("Out of memory")));
        return scope.Close(Undefined());
    }
    mb->request.data = mb;
    mb->cb = Persistent<Function>::New(Local<Function>::Cast(args[2]));
    mb->xaxis = args[0]->BooleanValue();
    mb->yaxis = args[1]->BooleanValue();
    mb->img = ObjectWrap::Unwrap<LwipImage>(args.This());
    mb->err = false;
    uv_queue_work(uv_default_loop(), &mb->request, mirrorAsync, mirrorAsyncDone);
    return scope.Close(Undefined());
}

void mirrorAsync(uv_work_t * request) {
    mirrorBaton * mb = static_cast<mirrorBaton *>(request->data);
    std::string axes = "";
    if (mb->xaxis) {
        axes += "x";
    }
    if (mb->yaxis) {
        axes += "y";
    }
    try {
        mb->img->_cimg->mirror(axes.c_str());
    } catch (CImgException e) {
        mb->err = true;
        mb->errMsg = "Unable to mirror image";
    }
    return;
}

void mirrorAsyncDone(uv_work_t * request, int status) {
    // mirror completed. now we call the callback.
    mirrorBaton * mb = static_cast<mirrorBaton *>(request->data);
    if (mb->err) {
        const unsigned int argc = 1;
        Local<Value> argv[argc] = {
            Local<Value>::New(Exception::Error(NanNew<String>(mb->errMsg.c_str())))
        };
        mb->cb->Call(Context::GetCurrent()->Global(), argc, argv);
    } else {
        const unsigned int argc = 1;
        Handle<Value> argv[argc] = {
            Local<Value>::New(Null())
        };
        mb->cb->Call(Context::GetCurrent()->Global(), argc, argv);
    }
    // dispose of cb, because it's a persistent function
    mb->cb.Dispose();
    delete mb;
}

// image.pad(left, top, right, bottom, color, callback):
// -----------------------------------------------------

// args[0] - left
// args[1] - right
// args[2] - top
// args[3] - bottom
// args[4] - red
// args[5] - green
// args[6] - blue
// args[7] - callback
Handle<Value> LwipImage::pad(const Arguments & args) {
    HandleScope scope;
    // (arguments validation is done in JS land)
    padBaton * pb = new padBaton();
    if (pb == NULL) {
        ThrowException(Exception::TypeError(NanNew<String>("Out of memory")));
        return scope.Close(Undefined());
    }
    pb->request.data = pb;
    pb->cb = Persistent<Function>::New(Local<Function>::Cast(args[7]));
    pb->left = (int) args[0]->NumberValue();
    pb->top = (int) args[1]->NumberValue();
    pb->right = (int) args[2]->NumberValue();
    pb->bottom = (int) args[3]->NumberValue();
    pb->color[0] = (unsigned char) args[4]->NumberValue();
    pb->color[1] = (unsigned char) args[5]->NumberValue();
    pb->color[2] = (unsigned char) args[6]->NumberValue();
    pb->img = ObjectWrap::Unwrap<LwipImage>(args.This());
    pb->err = false;
    uv_queue_work(uv_default_loop(), &pb->request, padAsync, padAsyncDone);
    return scope.Close(Undefined());
}

void padAsync(uv_work_t * request) {
    padBaton * pb = static_cast<padBaton *>(request->data);
    CImg<unsigned char> * res;
    unsigned int oldwidth = pb->img->_cimg->width(),
                 oldheight = pb->img->_cimg->height(),
                 newwidth = oldwidth + pb->left + pb->right,
                 newheight = oldheight + pb->top + pb->bottom;
    if (oldwidth != newwidth || oldheight != newheight) {
        try {
            res = new CImg<unsigned char>(newwidth, newheight, 1, 3);
        } catch (CImgException e) {
            pb->err = true;
            pb->errMsg = "Out of memory";
            return;
        }
        // fill left border:
        for (unsigned int x = 0; x < pb->left; x++) {
            for (unsigned int y = 0; y < newheight; y++) {
                res->fillC(x, y, 0, pb->color[0], pb->color[1], pb->color[2]);
            }
        }
        // fill right border:
        for (unsigned int x = oldwidth + pb->left; x < newwidth; x++) {
            for (unsigned int y = 0; y < newheight; y++) {
                res->fillC(x, y, 0, pb->color[0], pb->color[1], pb->color[2]);
            }
        }
        // fill top border:
        for (unsigned int x = pb->left; x < oldwidth + pb->left; x++) {
            for (unsigned int y = 0; y < pb->top; y++) {
                res->fillC(x, y, 0, pb->color[0], pb->color[1], pb->color[2]);
            }
        }
        // fill bottom border:
        for (unsigned int x = pb->left; x < oldwidth + pb->left; x++) {
            for (unsigned int y = pb->top + oldheight; y < newheight; y++) {
                res->fillC(x, y, 0, pb->color[0], pb->color[1], pb->color[2]);
            }
        }
        // paste original image:
        cimg_forXY(*(pb->img->_cimg), x, y) {
            unsigned char r = (*(pb->img->_cimg))(x, y, 0, 0),
                          g = (*(pb->img->_cimg))(x, y, 0, 1),
                          b = (*(pb->img->_cimg))(x, y, 0, 2);
            res->fillC(x + pb->left, y + pb->top, 0, r, g, b);
        }
        delete pb->img->_cimg;
        pb->img->_cimg = res;
    }
    return;
}

void padAsyncDone(uv_work_t * request, int status) {
    // pad completed. now we call the callback.
    padBaton * pb = static_cast<padBaton *>(request->data);
    if (pb->err) {
        const unsigned int argc = 1;
        Local<Value> argv[argc] = {
            Local<Value>::New(Exception::Error(NanNew<String>(pb->errMsg.c_str())))
        };
        pb->cb->Call(Context::GetCurrent()->Global(), argc, argv);
    } else {
        const unsigned int argc = 1;
        Handle<Value> argv[argc] = {
            Local<Value>::New(Null())
        };
        pb->cb->Call(Context::GetCurrent()->Global(), argc, argv);
    }
    // dispose of cb, because it's a persistent function
    pb->cb.Dispose();
    delete pb;
}
