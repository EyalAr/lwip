#include "lwip.h"

void openJpegAsync(uv_work_t * request) {
    ImageOpenBaton * iob = static_cast<ImageOpenBaton *>(request->data);
    CImg<unsigned char> * tmp;
    try {
        tmp = new CImg<unsigned char>();
        tmp->load_jpeg(iob->imgPath.c_str());
    } catch (CImgException e) {
        if (tmp) delete tmp;
        iob->err = true;
        iob->errMsg = "Unable to open file as JPEG";
        return;
    }
    to3Channels(tmp, iob);
    return;
}

void openPngAsync(uv_work_t * request) {
    ImageOpenBaton * iob = static_cast<ImageOpenBaton *>(request->data);
    CImg<unsigned char> * tmp;
    try {
        tmp = new CImg<unsigned char>();
        tmp->load_png(iob->imgPath.c_str());
    } catch (CImgException e) {
        if (tmp) delete tmp;
        iob->err = true;
        iob->errMsg = "Unable to open file as PNG";
        return;
    }
    to3Channels(tmp, iob);
    return;
}

void to3Channels(CImg<unsigned char> * tmp, ImageOpenBaton * iob) {
    // need to convert image to 3 channels spectrum?
    int spectrum = tmp->spectrum();
    if (spectrum == 3) {
        iob->imgData = tmp;
    } else if (spectrum == 1) {
        try {
            // create a 3-channels image of the same dimensions.
            iob->imgData = new CImg<unsigned char>(*tmp, "x,y,1,3");
        } catch (CImgException e) {
            delete tmp;
            if (iob->imgData) delete iob->imgData;
            iob->err = true;
            iob->errMsg = "Out of memory";
            return;
        }
        // copy the gray value of very pixel to each of R,G and B:
        cimg_forXYZ(*(iob->imgData), x, y, z) {
            unsigned char c = tmp->atXYZC(x, y, 0, 0);
            iob->imgData->fillC(x, y, z, c, c, c);
        }
        delete tmp;
    } else {
        // TODO: support 4 channels (CMYK)
        // need to find out if the 4th channel is alpha...
        if (tmp) delete tmp;
        iob->err = true;
        iob->errMsg = "Unsupported number of spectrums";
    }
    return;
}

void openAsyncDone(uv_work_t * request, int status) {
    // reading image completed. now we call the callback.
    ImageOpenBaton * iob = static_cast<ImageOpenBaton *>(request->data);
    if (iob->err) {
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
        LwipImage * lio = ObjectWrap::Unwrap<LwipImage>(Handle<Object>::Cast(imgObj));
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

Handle<Value> open(const Arguments & args, void asyncOpener(uv_work_t *)) {
    // this scope will discard all internal local objects for us.
    HandleScope scope;

    // arguments validation is done in JS land

    // open the file in 'path' asynchronously
    // the baton is on the heap because it lives in async calls outside the
    // scope of the current function
    ImageOpenBaton * iob = new ImageOpenBaton();
    if (iob == NULL) {
        ThrowException(Exception::TypeError(String::New("Out of memory")));
        return scope.Close(Undefined());
    }
    iob->request.data = iob;
    iob->cb = Persistent<Function>::New(Local<Function>::Cast(args[1]));
    iob->imgPath = std::string(*String::Utf8Value(args[0]->ToString()));
    iob->err = false;
    uv_queue_work(uv_default_loop(), &iob->request, asyncOpener, openAsyncDone);

    // Close the scope and return 'undefined'
    return scope.Close(Undefined());
}

Handle<Value> openJpeg(const Arguments & args) {
    return open(args, openJpegAsync);
}

Handle<Value> openPng(const Arguments & args) {
    return open(args, openPngAsync);
}

// create an init function for our node module
void init(Handle<Object> exports, Handle<Object> module) {
    LwipImage::Init();
    NODE_SET_METHOD(exports, "openJpeg", openJpeg);
    NODE_SET_METHOD(exports, "openPng", openPng);
}

// use NODE_MODULE macro to register our module:
NODE_MODULE(lwip, init)
