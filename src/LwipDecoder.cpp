#include "LwipDecoder.h"

DecodeBufferWorker::DecodeBufferWorker(NanCallback * callback, char * buffer, string type) {}

DecodeBufferWorker::~DecodeBufferWorker() {}

void DecodeBufferWorker::Execute () {
    SetErrorMessage("Decoding buffers is not yet supported");
    return;
}

DecodeFileWorker::DecodeFileWorker(NanCallback * callback, string path, string type)
    : NanAsyncWorker(callback), _path(path), _type(type), _pixbuff(NULL),
      _width(0), _height(0), _channels(0), _trans(false) {
}

DecodeFileWorker::~DecodeFileWorker() {
    // _pixbuff is not freed because it is given to JS as a Buffer object.
}

void DecodeFileWorker::Execute () {
    try {
        CImg<unsigned char> * img = new CImg<unsigned char>();
        if (_type == "jpeg") img->load_jpeg(_path.c_str());
        else if (_type == "png") img->load_png(_path.c_str());
        // TODO: GIF support.
        // no other types for now
    } catch (CImgException e) {
        if (img) delete img;
        SetErrorMessage("Unable to open file");
        return;
    }
    string err = to3Channels(&img);
    if (err != "") {
        if (img) delete img;
        SetErrorMessage(err.c_str());
        return;
    }
    img->_is_shared = true; // don't free image data. need it for callback.
    _pixbuff = img->ptr();
    _width = img->width();
    _height = img->height();
    // TODO: support transparency:
    _channels = 3;
    _trans = false;
    delete img;
    return;
}

void DecodeFileWorker::HandleOKCallback () {
    NanScope();
    Local<Value> argv[] = {
        NanNull(),
        NanBufferUse(
            (char *) _pixbuf,
            _width * _height * _channels
        ),
        NanNew<Number>(_width),
        NanNew<Number>(_height),
        NanNew<Number>(_channels),
        NanNew<Boolean>(_trans)
    };
    callback->Call(6, argv);
}

string to3Channels(CImg<unsigned char> ** img) {
    CImg<unsigned char> * tmp = NULL;
    // need to convert image to 3 channels spectrum?
    int spectrum = (*img)->spectrum();
    if (spectrum != 3) {
        if (spectrum == 1) {
            try {
                // create a 3-channels image of the same dimensions.
                // TODO: maybe can realloc current image and do it in-place?
                tmp = new CImg<unsigned char>(**img, "x,y,1,3");
            } catch (CImgException e) {
                if (tmp) delete tmp;
                return "Out of memory";
            }
            // copy the gray value of very pixel to each of R,G and B:
            cimg_forXYZ(**img, x, y, z) {
                unsigned char c = (*img)->atXYZC(x, y, z, 0);
                tmp->fillC(x, y, z, c, c, c);
            }
            delete *img;
            *img = tmp;
        } else {
            // TODO: support 4 channels (CMYK)
            // need to find out if the 4th channel is alpha...
            return "Unsupported number of channels";
        }
    }
    return "";
}

NAN_METHOD(decodeJpegFile) {
    NanScope();

    NanUtf8String path = NanUtf8String(args[0]);
    NanCallback * callback = new NanCallback(args[1].As<Function>());

    NanAsyncQueueWorker(new DecodeFileWorker(callback, string(*path), "jpeg"));
    NanReturnUndefined();
}

NAN_METHOD(decodePngFile) {
    NanScope();

    NanUtf8String path = NanUtf8String(args[0]);
    NanCallback * callback = new NanCallback(args[1].As<Function>());

    NanAsyncQueueWorker(new DecodeFileWorker(callback, string(*path), "png"));
    NanReturnUndefined();
}

NAN_METHOD(decodeJpegBuffer) {
    NanScope();

    Local<Object> jpegBuff = args[0].As<Object>();
    char * buffer = Buffer::Data(jpegBuff);
    NanCallback * callback = new NanCallback(args[1].As<Function>());

    NanAsyncQueueWorker(new DecodeBufferWorker(callback, buffer, "jpeg"));
    NanReturnUndefined();
}

NAN_METHOD(decodePngBuffer) {
    NanScope();

    Local<Object> pngBuff = args[0].As<Object>();
    char * buffer = Buffer::Data(pngBuff);
    NanCallback * callback = new NanCallback(args[1].As<Function>());

    NanAsyncQueueWorker(new DecodeBufferWorker(callback, buffer, "png"));
    NanReturnUndefined();
}

// create an init function for our node module
void InitAll(Handle<Object> exports) {
    exports->Set(NanNew("jpegFile"),
                 NanNew<FunctionTemplate>(decodeJpegFile)->GetFunction());
    exports->Set(NanNew("pngFile"),
                 NanNew<FunctionTemplate>(decodepngFile)->GetFunction());
    exports->Set(NanNew("jpegBuffer"),
                 NanNew<FunctionTemplate>(decodeJpegBuffer)->GetFunction());
    exports->Set(NanNew("pngBuffer"),
                 NanNew<FunctionTemplate>(decodePngBuffer)->GetFunction());
}

// use NODE_MODULE macro to register our module:
NODE_MODULE(lwip_decoder, InitAll)
