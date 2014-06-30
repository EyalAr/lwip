#include "LwipImage.h"
#include <node.h>
#include <v8.h>

using namespace v8;
using namespace node;

Persistent<Function> LwipImage::constructor;

LwipImage::~LwipImage(){
    // free the CImg object in memory
    if (_data != NULL) delete _data;
}

void LwipImage::Init() {
    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
    tpl->SetClassName(String::NewSymbol("LwipImage"));
    tpl->InstanceTemplate()->SetInternalFieldCount(2);
    SetPrototypeMethod(tpl, "width", width);
    SetPrototypeMethod(tpl, "height", height);
    SetPrototypeMethod(tpl, "resize", resize);
    SetPrototypeMethod(tpl, "toJpegBuffer", toJpegBuffer);
    constructor = Persistent<Function>::New(tpl->GetFunction());
}

Handle<Value> LwipImage::NewInstance() {
    HandleScope scope;
    Handle<Value> argv[0] = {};
    Local<Object> instance = constructor->NewInstance(0, argv);
    return scope.Close(instance);
}

Handle<Value> LwipImage::New(const Arguments& args) {
    HandleScope scope;
    LwipImage * obj = new LwipImage();
    obj->Wrap(args.This());
    return args.This();
}

// IMAGE JS OBJECT METHODS:
// ========================

// image.width():
// --------------
Handle<Value> LwipImage::width(const Arguments& args) {
    HandleScope scope;
    LwipImage * obj = ObjectWrap::Unwrap<LwipImage>(args.This());
    return scope.Close(Number::New(obj->_data->width()));
}

// image.height():
// ---------------
Handle<Value> LwipImage::height(const Arguments& args) {
    HandleScope scope;
    LwipImage * obj = ObjectWrap::Unwrap<LwipImage>(args.This());
    return scope.Close(Number::New(obj->_data->height()));
}

// image.resize(width, height, callback):
// --------------------------------------

// args[0] - width
// args[1] - height
// args[2] - inter(polation)
// args[3] - callback
Handle<Value> LwipImage::resize(const Arguments& args) {
    HandleScope scope;
    // (arguments validation is done in JS land)
    resizeBaton * rb = new resizeBaton();
    if (rb == NULL){
        ThrowException(Exception::TypeError(String::New("Out of memory")));
        return scope.Close(Undefined());
    }
    rb->request.data = rb;
    rb->cb = Persistent<Function>::New(Local<Function>::Cast(args[3]));
    rb->width = (unsigned int) args[0]->NumberValue();
    rb->height = (unsigned int) args[1]->NumberValue();
    rb->inter = (unsigned int) args[2]->NumberValue();
    rb->img = ObjectWrap::Unwrap<LwipImage>(args.This());
    uv_queue_work(uv_default_loop(), &rb->request, resizeAsync, resizeAsyncDone);
    return scope.Close(Undefined());
}

void resizeAsync(uv_work_t * request){
    resizeBaton * rb = static_cast<resizeBaton *>(request->data);
    try{
        rb->img->_data->resize(rb->width, rb->height, -100, -100, rb->inter);
    } catch (CImgException e){
        rb->err = true;
        rb->errMsg = "Unable to resize image";
    }
    return;
}

void resizeAsyncDone(uv_work_t * request, int status){
    // resize completed. now we call the callback.
    resizeBaton * rb = static_cast<resizeBaton *>(request->data);
    if (rb->err){
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

// image.to{type}Buffer(format,callback):
// --------------------------------------

bool initToBufferBaton(ToBufferBaton ** tbb, const Arguments& args){
    *tbb = new ToBufferBaton();
    if (*tbb == NULL){
        ThrowException(Exception::TypeError(String::New("Out of memory")));
        return false;
    }
    (*tbb)->request.data = *tbb;
    // callback is always last argument
    (*tbb)->cb = Persistent<Function>::New(Local<Function>::Cast(args[args.Length() - 1]));
    (*tbb)->img = ObjectWrap::Unwrap<LwipImage>(args.This());
    return true;
}

// args[0] - jpeg quality
Handle<Value> LwipImage::toJpegBuffer(const Arguments& args) {
    HandleScope scope;
    // (arguments validation is done in JS land)
    ToBufferBaton * tbb = NULL;
    if (!initToBufferBaton(&tbb, args)){
        return scope.Close(Undefined());
    }
    tbb->jpegQuality = (unsigned int) args[0]->NumberValue();
    uv_queue_work(uv_default_loop(), &tbb->request, toJpegBufferAsync, toBufferAsyncDone);
    return scope.Close(Undefined());
}

void toJpegBufferAsync(uv_work_t * request){
    ToBufferBaton * tbb = static_cast<ToBufferBaton *>(request->data);
    unsigned int
        dimbuf = 0,
        spectrum = tbb->img->_data->spectrum(),
        width = tbb->img->_data->width(),
        height = tbb->img->_data->height(),
        quality = tbb->jpegQuality;
    J_COLOR_SPACE colortype = JCS_RGB;
    JSAMPROW row_pointer[1];
    unsigned char * tmp;
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;

    switch(spectrum) {
        case 1 : dimbuf = 1; colortype = JCS_GRAYSCALE; break;
        case 2 : dimbuf = 3; colortype = JCS_RGB; break;
        case 3 : dimbuf = 3; colortype = JCS_RGB; break;
        default : dimbuf = 4; colortype = JCS_CMYK; break;
    }

    // TODO:
    // 1. deal with the various cases of spetrum
    // 2. handle jpeglib error

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    jpeg_mem_dest(&cinfo, &tbb->buffer, &tbb->bufferSize);
    cinfo.image_width = width;
    cinfo.image_height = height;
    cinfo.input_components = dimbuf;
    cinfo.in_color_space = colortype;
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, quality, TRUE);
    jpeg_start_compress(&cinfo, TRUE);

    tmp = (unsigned char *) malloc(cinfo.image_width * dimbuf);
    while (cinfo.next_scanline < cinfo.image_height) {
        unsigned char * ptrd = tmp;
        const unsigned char
            * ptr_r = tbb->img->_data->data(0, cinfo.next_scanline, 0, 0),
            * ptr_g = tbb->img->_data->data(0, cinfo.next_scanline, 0, 1),
            * ptr_b = tbb->img->_data->data(0, cinfo.next_scanline, 0, 2);
        for(unsigned int b = 0; b < cinfo.image_width; ++b) {
            *(ptrd++) = (unsigned char)*(ptr_r++);
            *(ptrd++) = (unsigned char)*(ptr_g++);
            *(ptrd++) = (unsigned char)*(ptr_b++);
        }
        *row_pointer = tmp;
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }
    free(tmp);
    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);

    return;
}

void toBufferAsyncDone(uv_work_t * request, int status){
    // compression completed. now we call the callback.
    ToBufferBaton * tbb = static_cast<ToBufferBaton *>(request->data);
    if (tbb->err){
        const unsigned int argc = 1;
        Local<Value> argv[argc] = {
            Local<Value>::New(Exception::Error(String::New(tbb->errMsg.c_str())))
        };
        tbb->cb->Call(Context::GetCurrent()->Global(), argc, argv);
    } else {
        // build Buffer object
        Handle<Object> bufferObj = node::Buffer::New((const char *) tbb->buffer, tbb->bufferSize)->handle_;
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
