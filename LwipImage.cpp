#include "LwipImage.h"
#include <node.h>
#include <v8.h>

using namespace v8;

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
    node::SetPrototypeMethod(tpl, "width", width);
    node::SetPrototypeMethod(tpl, "height", height);
    node::SetPrototypeMethod(tpl, "toBuffer", toBuffer);
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

// image.toBuffer(format,callback):
// --------------------------------
Handle<Value> LwipImage::toBuffer(const Arguments& args) {
    // this scope will discard all internal local objects for us.
    HandleScope scope;

    // arguments validation:
    if (args.Length() < 2){
        ThrowException(Exception::TypeError(String::New("'open' takes 2 arguments: (String, Function)")));
        return scope.Close(Undefined());
    }
    if (!args[0]->IsString()){
        ThrowException(Exception::TypeError(String::New("First argument must be a format string")));
        return scope.Close(Undefined());
    }
    if (!args[1]->IsFunction()){
        ThrowException(Exception::TypeError(String::New("Second argument must be a callback function")));
        return scope.Close(Undefined());
    }

    // open the file in 'path' asynchronously
    // the baton is on the heap because it lives in async calls outside the
    // scope of the current function
    ToBufferBaton * tbb = new ToBufferBaton();
    if (tbb == NULL){
        ThrowException(Exception::TypeError(String::New("Out of memory")));
        return scope.Close(Undefined());
    }
    tbb->request.data = tbb;
    tbb->cb = Persistent<Function>::New(Local<Function>::Cast(args[1]));
    tbb->format = std::string(*String::Utf8Value(args[0]->ToString()));
    tbb->img = ObjectWrap::Unwrap<LwipImage>(args.This());
    uv_queue_work(uv_default_loop(), &tbb->request, toBufferAsync, toBufferAsyncDone);

    // Close the scope and return 'undefined'
    return scope.Close(Undefined());
}

void toBufferAsync(uv_work_t * request){
    // compress image according to specified format, create a Buffer object
    // and pass it to the callback
    ToBufferBaton * tbb = static_cast<ToBufferBaton *>(request->data);
    // TODO: choose encoder according to format. Currently only jpeg.

    unsigned int
        dimbuf = 0,
        spectrum = tbb->img->_data->spectrum(),
        width = tbb->img->_data->width(),
        height = tbb->img->_data->height();
    J_COLOR_SPACE colortype = JCS_RGB;
    JSAMPROW row_pointer[1];
    unsigned char * tmp;

    switch(spectrum) {
        case 1 : dimbuf = 1; colortype = JCS_GRAYSCALE; break;
        case 2 : dimbuf = 3; colortype = JCS_RGB; break;
        case 3 : dimbuf = 3; colortype = JCS_RGB; break;
        default : dimbuf = 4; colortype = JCS_CMYK; break;
    }

    // TODO:
    // 1. deal with the various cases of spetrum
    // 2. handle jpeglib error
    // 3. allow user to specify jpeg quality

    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    jpeg_mem_dest(&cinfo, &tbb->buffer, &tbb->bufferSize);
    cinfo.image_width = width;
    cinfo.image_height = height;
    cinfo.input_components = dimbuf;
    cinfo.in_color_space = colortype;
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, 100, TRUE);
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
        // define the arguments for the callback
        const unsigned int argc = 1;
        Local<Value> argv[argc] = {
            Local<Value>::New(Exception::Error(String::New(tbb->errMsg.c_str())))
        };
        // run the callback
        tbb->cb->Call(Context::GetCurrent()->Global(), argc, argv);
    } else {
        // build Buffer object
        Handle<Object> bufferObj = node::Buffer::New((const char *) tbb->buffer, tbb->bufferSize)->handle_;
        // define the arguments for the callback
        const unsigned int argc = 2;
        Handle<Value> argv[argc] = {
            Local<Value>::New(Null()),
            bufferObj
        };
        // run the callback
        tbb->cb->Call(Context::GetCurrent()->Global(), argc, argv);
    }
    // dispose of cb, because it's a persistent function
    tbb->cb.Dispose();
    // free baton's memory
    free(tbb->buffer);
    delete tbb;
}
