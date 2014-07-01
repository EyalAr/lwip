#include "lwip.h"

using namespace cimg_library;
using namespace v8;

void openJpegAsync(uv_work_t * request){
    // here is where we actually open the image file and retrieve the binary
    // data. open image, store data in iob->imgData.
    ImageOpenBaton * iob = static_cast<ImageOpenBaton *>(request->data);
    try{
        iob->imgData = new CImg<unsigned char>();
        iob->imgData->load_jpeg(iob->imgPath.c_str());
    } catch (CImgException e){
        delete iob->imgData;
        iob->err = true;
        iob->errMsg = "Unable to open file as JPEG";
    }
    return;
}

void openAsyncDone(uv_work_t * request, int status){
    // reading image completed. now we call the callback.
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
        lio->_data = iob->imgData;
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

Handle<Value> _open(const Arguments &args, void asyncOpener(uv_work_t *)){
    // this scope will discard all internal local objects for us.
    HandleScope scope;

    // arguments validation is done in JS land

    // open the file in 'path' asynchronously
    // the baton is on the heap because it lives in async calls outside the
    // scope of the current function
    ImageOpenBaton * iob = new ImageOpenBaton();
    if (iob == NULL){
        ThrowException(Exception::TypeError(String::New("Out of memory")));
        return scope.Close(Undefined());
    }
    iob->request.data = iob;
    iob->cb = Persistent<Function>::New(Local<Function>::Cast(args[1]));
    iob->imgPath = std::string(*String::Utf8Value(args[0]->ToString()));
    uv_queue_work(uv_default_loop(), &iob->request, asyncOpener, openAsyncDone);

    // Close the scope and return 'undefined'
    return scope.Close(Undefined());
}

Handle<Value> openJpeg(const Arguments &args){
    return _open(args, openJpegAsync);
}

Handle<Value> openPng(const Arguments &args){
    HandleScope scope;
    ThrowException(Exception::TypeError(String::New(
        "'open{type}': PNG is not yet supported")));
    return scope.Close(Undefined());
}

// create an init function for our node module
void init(Handle<Object> exports, Handle<Object> module){
    LwipImage::Init();
    NODE_SET_METHOD(exports, "openJpeg", openJpeg);
    NODE_SET_METHOD(exports, "openPng", openPng);
}

// use NODE_MODULE macro to register our module:
NODE_MODULE(lwip,init)
