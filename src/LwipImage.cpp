#include "LwipImage.h"
#include <node.h>
#include <v8.h>

using namespace v8;
using namespace node;

Persistent<Function> LwipImage::constructor;

LwipImage::~LwipImage() {
    // free the CImg object in memory
    if (_data != NULL) delete _data;
}

void LwipImage::Init() {
    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    tpl->SetClassName(String::NewSymbol("LwipImage"));
    SetPrototypeMethod(tpl, "width", width);
    SetPrototypeMethod(tpl, "height", height);
    SetPrototypeMethod(tpl, "resize", resize);
    SetPrototypeMethod(tpl, "rotate", rotate);
    SetPrototypeMethod(tpl, "blur", blur);
    SetPrototypeMethod(tpl, "crop", crop);
    SetPrototypeMethod(tpl, "mirror", mirror);
    SetPrototypeMethod(tpl, "pad", pad);
    SetPrototypeMethod(tpl, "toJpegBuffer", toJpegBuffer);
    SetPrototypeMethod(tpl, "toPngBuffer", toPngBuffer);
    constructor = Persistent<Function>::New(tpl->GetFunction());
}

Handle<Value> LwipImage::NewInstance() {
    HandleScope scope;
    Local<Object> instance = constructor->NewInstance();
    return scope.Close(instance);
}

Handle<Value> LwipImage::New(const Arguments & args) {
    HandleScope scope;
    LwipImage * obj = new LwipImage();
    obj->Wrap(args.This());
    return args.This();
}

// IMAGE JS OBJECT METHODS:
// ========================

// image.width():
// --------------
Handle<Value> LwipImage::width(const Arguments & args) {
    HandleScope scope;
    LwipImage * obj = ObjectWrap::Unwrap<LwipImage>(args.This());
    return scope.Close(Number::New(obj->_data->width()));
}

// image.height():
// ---------------
Handle<Value> LwipImage::height(const Arguments & args) {
    HandleScope scope;
    LwipImage * obj = ObjectWrap::Unwrap<LwipImage>(args.This());
    return scope.Close(Number::New(obj->_data->height()));
}

// image.resize(width, height, inter, callback):
// ---------------------------------------------

// args[0] - width
// args[1] - height
// args[2] - inter(polation)
// args[3] - callback
Handle<Value> LwipImage::resize(const Arguments & args) {
    HandleScope scope;
    // (arguments validation is done in JS land)
    resizeBaton * rb = new resizeBaton();
    if (rb == NULL) {
        ThrowException(Exception::TypeError(String::New("Out of memory")));
        return scope.Close(Undefined());
    }
    rb->request.data = rb;
    rb->cb = Persistent<Function>::New(Local<Function>::Cast(args[3]));
    rb->width = (unsigned int) args[0]->NumberValue();
    rb->height = (unsigned int) args[1]->NumberValue();
    rb->inter = (unsigned int) args[2]->NumberValue();
    rb->img = ObjectWrap::Unwrap<LwipImage>(args.This());
    rb->err = false;
    uv_queue_work(uv_default_loop(), &rb->request, resizeAsync, resizeAsyncDone);
    return scope.Close(Undefined());
}

void resizeAsync(uv_work_t * request) {
    resizeBaton * rb = static_cast<resizeBaton *>(request->data);
    try {
        rb->img->_data->resize(rb->width, rb->height, -100, -100, rb->inter);
    } catch (CImgException e) {
        rb->err = true;
        rb->errMsg = "Unable to resize image";
    }
    return;
}

void resizeAsyncDone(uv_work_t * request, int status) {
    // resize completed. now we call the callback.
    resizeBaton * rb = static_cast<resizeBaton *>(request->data);
    if (rb->err) {
        const unsigned int argc = 1;
        Local<Value> argv[argc] = {
            Local<Value>::New(Exception::Error(String::New(rb->errMsg.c_str())))
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
        ThrowException(Exception::TypeError(String::New("Out of memory")));
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
        unsigned int oldwidth = rb->img->_data->width(),
                     oldheight = rb->img->_data->height();
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
        cimg_forXY(*(rb->img->_data), x, y) {
            unsigned char r = (*(rb->img->_data))(x, y, 0, 0),
                          g = (*(rb->img->_data))(x, y, 0, 1),
                          b = (*(rb->img->_data))(x, y, 0, 2);
            res->fillC(x + 1, y + 1, 0, r, g, b);
        }
        delete rb->img->_data;
        rb->img->_data = res;
    }
    try {
        // linear interpolations = 1
        // neumann boundary condition = 1
        rb->img->_data->rotate(nangle, 1, 1);
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
            Local<Value>::New(Exception::Error(String::New(rb->errMsg.c_str())))
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
        ThrowException(Exception::TypeError(String::New("Out of memory")));
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
        bb->img->_data->vanvliet(bb->sigma, 0, 'x');
        bb->img->_data->vanvliet(bb->sigma, 0, 'y');
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
            Local<Value>::New(Exception::Error(String::New(bb->errMsg.c_str())))
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
        ThrowException(Exception::TypeError(String::New("Out of memory")));
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
        cb->img->_data->crop(cb->left, cb->top, 0, 0, cb->right, cb->bottom, 0, 2);
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
            Local<Value>::New(Exception::Error(String::New(cb->errMsg.c_str())))
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
        ThrowException(Exception::TypeError(String::New("Out of memory")));
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
        mb->img->_data->mirror(axes.c_str());
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
            Local<Value>::New(Exception::Error(String::New(mb->errMsg.c_str())))
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
        ThrowException(Exception::TypeError(String::New("Out of memory")));
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
    unsigned int oldwidth = pb->img->_data->width(),
                 oldheight = pb->img->_data->height(),
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
        cimg_forXY(*(pb->img->_data), x, y) {
            unsigned char r = (*(pb->img->_data))(x, y, 0, 0),
                          g = (*(pb->img->_data))(x, y, 0, 1),
                          b = (*(pb->img->_data))(x, y, 0, 2);
            res->fillC(x + pb->left, y + pb->top, 0, r, g, b);
        }
        delete pb->img->_data;
        pb->img->_data = res;
    }
    return;
}

void padAsyncDone(uv_work_t * request, int status) {
    // pad completed. now we call the callback.
    padBaton * pb = static_cast<padBaton *>(request->data);
    if (pb->err) {
        const unsigned int argc = 1;
        Local<Value> argv[argc] = {
            Local<Value>::New(Exception::Error(String::New(pb->errMsg.c_str())))
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

// image.to{type}Buffer(format,callback):
// --------------------------------------

bool initToBufferBaton(ToBufferBaton ** tbb, const Arguments & args) {
    *tbb = new ToBufferBaton();
    if (*tbb == NULL) {
        ThrowException(Exception::TypeError(String::New("Out of memory")));
        return false;
    }
    (*tbb)->request.data = *tbb;
    // callback is always last argument
    (*tbb)->cb = Persistent<Function>::New(Local<Function>::Cast(args[args.Length() - 1]));
    (*tbb)->img = ObjectWrap::Unwrap<LwipImage>(args.This());
    (*tbb)->err = false;
    (*tbb)->bufferSize = 0;
    (*tbb)->buffer = NULL;
    return true;
}

// args[0] - jpeg quality
Handle<Value> LwipImage::toJpegBuffer(const Arguments & args) {
    HandleScope scope;
    // (arguments validation is done in JS land)
    ToBufferBaton * tbb = NULL;
    if (!initToBufferBaton(&tbb, args)) {
        return scope.Close(Undefined());
    }
    tbb->jpegQuality = (unsigned int) args[0]->NumberValue();
    uv_queue_work(uv_default_loop(), &tbb->request, toJpegBufferAsync, toBufferAsyncDone);
    return scope.Close(Undefined());
}

void toJpegBufferAsync(uv_work_t * request) {
    ToBufferBaton * tbb = static_cast<ToBufferBaton *>(request->data);
    unsigned int dimbuf = 3, width, height, quality;
    J_COLOR_SPACE colortype = JCS_RGB;
    JSAMPROW row_pointer[1];
    unsigned char * tmp = NULL;
    struct jpeg_compress_struct cinfo;
    struct lwip_jpeg_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = lwip_jpeg_error_exit;
    if (setjmp(jerr.setjmp_buffer)) {
        jpeg_destroy_compress(&cinfo);
        if (tmp) free(tmp);
        tbb->err = true;
        tbb->errMsg = "JPEG compression error";
        return;
    }

    width = tbb->img->_data->width();
    height = tbb->img->_data->height();
    quality = tbb->jpegQuality;

    tmp = (unsigned char *) malloc(width * dimbuf);
    if (tmp == NULL) {
        tbb->err = true;
        tbb->errMsg = "Out of memory";
        return;
    }

    jpeg_create_compress(&cinfo);
    jpeg_mem_dest(&cinfo, &tbb->buffer, &tbb->bufferSize);
    cinfo.image_width = width;
    cinfo.image_height = height;
    cinfo.input_components = dimbuf;
    cinfo.in_color_space = colortype;
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, quality, TRUE);
    jpeg_start_compress(&cinfo, TRUE);

    while (cinfo.next_scanline < cinfo.image_height) {
        unsigned char * ptrd = tmp;
        const unsigned char
        * ptr_r = tbb->img->_data->data(0, cinfo.next_scanline, 0, 0),
          * ptr_g = tbb->img->_data->data(0, cinfo.next_scanline, 0, 1),
            * ptr_b = tbb->img->_data->data(0, cinfo.next_scanline, 0, 2);
        for (unsigned int b = 0; b < cinfo.image_width; ++b) {
            *(ptrd++) = (unsigned char) * (ptr_r++);
            *(ptrd++) = (unsigned char) * (ptr_g++);
            *(ptrd++) = (unsigned char) * (ptr_b++);
        }
        *row_pointer = tmp;
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }
    free(tmp);
    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);

    return;
}

// args[0] - compression level: 0 - none, 1 - fast, 2 - high
// args[1] - interlaced? boolean
Handle<Value> LwipImage::toPngBuffer(const Arguments & args) {
    HandleScope scope;
    // (arguments validation is done in JS land)
    ToBufferBaton * tbb = NULL;
    if (!initToBufferBaton(&tbb, args)) {
        return scope.Close(Undefined());
    }
    tbb->pngCompLevel = (unsigned int) args[0]->NumberValue();
    tbb->pngInterlaced = args[1]->BooleanValue();
    uv_queue_work(uv_default_loop(), &tbb->request, toPngBufferAsync, toBufferAsyncDone);
    return scope.Close(Undefined());
}

void toPngBufferAsync(uv_work_t * request) {
    ToBufferBaton * tbb = static_cast<ToBufferBaton *>(request->data);
    unsigned int width = tbb->img->_data->width();
    unsigned int height = tbb->img->_data->height();
    unsigned int rowBytes = width * 3; // TODO: 3 channels per pixel is currently hard coded
    int interlaceType;
    int compLevel;
    switch (tbb->pngCompLevel) {
    case 0: // no compression
        compLevel = Z_NO_COMPRESSION;
        break;
    case 1: // fast compression
        compLevel = Z_BEST_SPEED;
        break;
    case 2: // high compression
        compLevel = Z_BEST_COMPRESSION;
        break;
    default:
        compLevel = Z_DEFAULT_COMPRESSION;
        break;
    }
    if (tbb->pngInterlaced) {
        interlaceType = PNG_INTERLACE_ADAM7;
    } else {
        interlaceType = PNG_INTERLACE_NONE;
    }

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
                          NULL, NULL, NULL);

    if (!png_ptr) {
        tbb->err = true;
        tbb->errMsg = "Out of memory";
        return;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, (png_infopp) NULL);
        tbb->err = true;
        tbb->errMsg = "Out of memory";
        return;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        tbb->err = true;
        tbb->errMsg = "PNG compression error";
        return;
    }

    unsigned char ** rowPnts = (unsigned char **)malloc(height * sizeof(unsigned char *));
    if (!rowPnts) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        tbb->err = true;
        tbb->errMsg = "Out of memory";
        return;
    }
    for (unsigned int r = 0; r < height; r++) {
        rowPnts[r] = (unsigned char *)malloc(rowBytes * sizeof(unsigned char));
        if (!rowPnts[r]) {
            // free previous allocations
            for (unsigned int p = 0 ; p < r ; p++) free(rowPnts[p]);
            free(rowPnts);
            png_destroy_write_struct(&png_ptr, &info_ptr);
            tbb->err = true;
            tbb->errMsg = "Out of memory";
            return;
        }
    }

    png_set_IHDR(png_ptr, info_ptr, width, height,
                 8, PNG_COLOR_TYPE_RGB, interlaceType,
                 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    png_set_compression_level(png_ptr, compLevel);
    png_set_write_fn(png_ptr, (voidp) tbb, pngWriteCB, NULL);

    cimg_forXY(*(tbb->img->_data), x, y) {
        unsigned char r = tbb->img->_data->atXYZC(x, y, 0, 0),
                      g = tbb->img->_data->atXYZC(x, y, 0, 1),
                      b = tbb->img->_data->atXYZC(x, y, 0, 2);
        rowPnts[y][3 * x] = r;
        rowPnts[y][3 * x + 1] = g;
        rowPnts[y][3 * x + 2] = b;
    }
    png_set_rows(png_ptr, info_ptr, rowPnts);

    png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

    png_destroy_write_struct(&png_ptr, &info_ptr);
    free(rowPnts);

    return;
}

void pngWriteCB(png_structp png_ptr, png_bytep data, png_size_t length) {
    ToBufferBaton * tbb = (ToBufferBaton *) png_get_io_ptr(png_ptr);
    size_t size = tbb->bufferSize + length;

    if (tbb->buffer) {
        tbb->buffer = (unsigned char *) realloc(tbb->buffer, size * sizeof(unsigned char));
    } else {
        tbb->buffer = (unsigned char *) malloc(size * sizeof(unsigned char));
    }

    if (!tbb->buffer) {
        png_error(png_ptr, "Out of memory");
        return;
    }

    memcpy(tbb->buffer + tbb->bufferSize, data, length);
    tbb->bufferSize += length;
}

void toBufferAsyncDone(uv_work_t * request, int status) {
    // compression completed. now we call the callback.
    ToBufferBaton * tbb = static_cast<ToBufferBaton *>(request->data);
    if (tbb->err) {
        const unsigned int argc = 1;
        Local<Value> argv[argc] = {
            Local<Value>::New(Exception::Error(String::New(tbb->errMsg.c_str())))
        };
        tbb->cb->Call(Context::GetCurrent()->Global(), argc, argv);
    } else {
        // build Buffer object
        Handle<Object> bufferObj = Buffer::New((const char *) tbb->buffer, tbb->bufferSize)->handle_;
        const unsigned int argc = 2;
        Handle<Value> argv[argc] = {
            Local<Value>::New(Null()),
            bufferObj
        };
        tbb->cb->Call(Context::GetCurrent()->Global(), argc, argv);
    }
    // dispose of cb, because it's a persistent function
    tbb->cb.Dispose();
    // free baton's memory
    free(tbb->buffer);
    delete tbb;
}
