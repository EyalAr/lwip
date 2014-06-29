#define BUILDING_NODE_EXTENSION

#include <string>
#include <cstring>
#include <setjmp.h>
#include <node.h>
#include <v8.h>
#include "LwipImage.h"
#include "lib/jpeg/jpeglib.h"

using namespace v8;

struct ImageOpenBaton {
    uv_work_t request;
    Persistent<Function> cb;
    JSAMPARRAY imgData;
    unsigned int width;
    unsigned int height;
    std::string imgPath;
    bool err;
    std::string errMsg;
};

struct lwip_jpeg_error_mgr {
  struct jpeg_error_mgr pub;
  jmp_buf setjmp_buffer;
};
typedef struct lwip_jpeg_error_mgr * err_ptr_t;

METHODDEF(void) lwip_error_exit (j_common_ptr cinfo) {
  err_ptr_t err = (err_ptr_t) cinfo->err;
  /* Return control to the setjmp point */
  longjmp(err->setjmp_buffer, 1);
}

void openImageAsync(uv_work_t * request){
    // here is where we actually open the image file and retrieve the binary
    // data. open image, store data in iob->imgData, set width, height.

    ImageOpenBaton * iob = static_cast<ImageOpenBaton *>(request->data);
    struct jpeg_decompress_struct cinfo;
    struct lwip_jpeg_error_mgr jerr;
    FILE * infile;
    int row_stride;

    if ((infile = fopen(iob->imgPath.c_str(), "rb")) == NULL) {
        iob->err = true;
        iob->errMsg = "Unable to open file";
        return;
    }

    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = lwip_error_exit;
    /* Establish the setjmp return context for lwip_error_exit to use. */
    if (setjmp(jerr.setjmp_buffer)) {
        /* If we get here, the JPEG code has signaled an error.
         * We need to clean up the JPEG object, close the input file, and return.
         */
        jpeg_destroy_decompress(&cinfo);
        fclose(infile);
        iob->err = true;
        iob->errMsg = "JPEG decompression error";
        return;
    }

    /* Now we can initialize the JPEG decompression object. */
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, infile);
    (void) jpeg_read_header(&cinfo, TRUE);
    (void) jpeg_start_decompress(&cinfo);
    row_stride = cinfo.output_width * cinfo.output_components;
    iob->imgData = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
    iob->width = cinfo.image_width;
    iob->height = cinfo.image_height;
    while (cinfo.output_scanline < cinfo.output_height) {
        (void) jpeg_read_scanlines(&cinfo, iob->imgData, 1);
        /* Assume put_scanline_someplace wants a pointer and sample count. */
        //put_scanline_someplace(iob->imgData[0], row_stride);
    }
    (void) jpeg_finish_decompress(&cinfo);
    // release mem, close file
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);

    /* At this point you may want to check to see whether any corrupt-data
    * warnings occurred (test whether jerr.pub.num_warnings is nonzero).
    */

    return;
}

void openImageAsyncDone(uv_work_t * request, int status){
    // reading image completed. now we call the callback.
    // TODO: build LwipImage object from the data, pass it to cb.
    ImageOpenBaton * iob = static_cast<ImageOpenBaton *>(request->data);
    if (iob->err){
        // define the arguments for the callback
        const unsigned int argc = 1;
        Local<Value> argv[argc] = {
            Local<Value>::New(Exception::Error(String::New(iob->errMsg.c_str())))
        };
        // run the callback
        iob->cb->Call(Context::GetCurrent()->Global(), argc, argv);
    } else {
        // build image object
        Handle<Value> imgObj = LwipImage::NewInstance();
        LwipImage * lio = node::ObjectWrap::Unwrap<LwipImage>(Handle<Object>::Cast(imgObj));
        lio->setData(iob->imgData);
        lio->setWidth(iob->width);
        lio->setHeight(iob->height);
        // define the arguments for the callback
        const unsigned int argc = 2;
        Local<Value> argv[argc] = {
            Local<Value>::New(Null()),
            Local<Value>::New(imgObj)
        };
        // run the callback
        iob->cb->Call(Context::GetCurrent()->Global(), argc, argv);
    }
    // dispose of cb, because it's a persistent function
    iob->cb.Dispose();
    // free baton's memory
    delete iob;
}

// create 'open' function template
Handle<Value> open(const Arguments &args){
    // this scope will discard all internal local objects for us.
    HandleScope scope;

    // arguments validation:
    if (args.Length() < 2){
        ThrowException(Exception::TypeError(String::New("'open' takes 2 arguments: (String, Function)")));
        return scope.Close(Undefined());
    }
    if (!args[0]->IsString()){
        ThrowException(Exception::TypeError(String::New("First argument must be a path string")));
        return scope.Close(Undefined());
    }
    if (!args[1]->IsFunction()){
        ThrowException(Exception::TypeError(String::New("Second argument must be a callback function")));
        return scope.Close(Undefined());
    }

    // open the file in 'path' asynchronously
    // the baton is on the heap because is lives in async calls outside the
    // scope of the current function
    ImageOpenBaton * iob = new ImageOpenBaton();
    if (iob == NULL){
        ThrowException(Exception::TypeError(String::New("Out of memory")));
        return scope.Close(Undefined());
    }
    iob->request.data = iob;
    iob->cb = Persistent<Function>::New(Local<Function>::Cast(args[1]));
    iob->imgPath = std::string(*String::Utf8Value(args[0]->ToString()));
    uv_queue_work(uv_default_loop(), &iob->request, openImageAsync, openImageAsyncDone);

    // Close the scope and return 'undefined'
    return scope.Close(Undefined());
}

// create an init function for our node module
void init(Handle<Object> exports, Handle<Object> module){
    LwipImage::Init();
    NODE_SET_METHOD(exports, "open", open);
}

// use NODE_MODULE macro to register our module:
NODE_MODULE(lwip,init)
