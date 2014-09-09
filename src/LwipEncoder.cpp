#include "LwipEncoder.h"

EncodeToJpegBufferWorker::EncodeToJpegBufferWorker(
    unsigned char * pixbuf,
    size_t width,
    size_t height,
    int quality,
    NanCallback * callback
): NanAsyncWorker(callback), _pixbuff(pixbuf), _width(width), _height(height),
    _quality(quality), _jpegbuf(NULL), _jpegbufsize(0) {}

EncodeToJpegBufferWorker::~EncodeToJpegBufferWorker() {}

void EncodeToJpegBufferWorker::Execute () {
    unsigned int dimbuf = 3;
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
        SetErrorMessage("JPEG compression error");
        return;
    }

    tmp = (unsigned char *) malloc(_width * dimbuf);
    if (tmp == NULL) {
        SetErrorMessage("Out of memory");
        return;
    }

    jpeg_create_compress(&cinfo);
    jpeg_mem_dest(&cinfo, _jpegbuf, &_jpegbufsize);
    cinfo.image_width = _width;
    cinfo.image_height = _height;
    cinfo.input_components = dimbuf;
    cinfo.in_color_space = colortype;
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, _quality, TRUE);
    jpeg_start_compress(&cinfo, TRUE);

    CImg<unsigned char> tmpimg(_pixbuff, _width, _height, 1, 3, true);
    while (cinfo.next_scanline < cinfo.image_height) {
        unsigned char * ptrd = tmp;
        const unsigned char
        * ptr_r = tmpimg.data(0, cinfo.next_scanline, 0, 0),
          * ptr_g = tmpimg.data(0, cinfo.next_scanline, 0, 1),
            * ptr_b = tmpimg.data(0, cinfo.next_scanline, 0, 2);
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

void EncodeToJpegBufferWorker::HandleOKCallback () {
    NanScope();
    Local<Value> argv[] = {
        NanNull(),
        NanBufferUse(
            _jpegbuf,
            _jpegbufsize
        )
    };
    callback->Call(2, argv);
}

EncodeToPngBufferWorker::EncodeToPngBufferWorker(
    unsigned char * pixbuf,
    size_t width,
    size_t height,
    int compression,
    bool interlaced,
    NanCallback * callback
): NanAsyncWorker(callback), _pixbuff(pixbuf), _width(width), _height(height),
    _compression(compression), _interlaced(interlaced), _pngbuf(NULL),
    _pngbufsize(0) {}

EncodeToPngBufferWorker::~EncodeToPngBufferWorker() {}

void EncodeToPngBufferWorker::Execute () {
    unsigned int rowBytes = _width * 3; // TODO: 3 channels per pixel is currently hard coded
    int interlaceType;
    int compLevel;
    switch (_compression) {
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
    if (_interlaced) {
        interlaceType = PNG_INTERLACE_ADAM7;
    } else {
        interlaceType = PNG_INTERLACE_NONE;
    }

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
                          NULL, NULL, NULL);

    if (!png_ptr) {
        SetErrorMessage("Out of memory");
        return;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, (png_infopp) NULL);
        SetErrorMessage("Out of memory");
        return;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        SetErrorMessage("PNG compression error");
        return;
    }

    unsigned char ** rowPnts = (unsigned char **) malloc(
                                   _height * sizeof(unsigned char *)
                               );
    if (!rowPnts) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        SetErrorMessage("Out of memory");
        return;
    }
    for (unsigned int r = 0; r < height; r++) {
        rowPnts[r] = (unsigned char *) malloc(rowBytes * sizeof(unsigned char));
        if (!rowPnts[r]) {
            // free previous allocations
            for (unsigned int p = 0 ; p < r ; p++) free(rowPnts[p]);
            free(rowPnts);
            png_destroy_write_struct(&png_ptr, &info_ptr);
            SetErrorMessage("Out of memory");
            return;
        }
    }

    png_set_IHDR(png_ptr, info_ptr, _width, _height,
                 8, PNG_COLOR_TYPE_RGB, interlaceType,
                 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    png_set_compression_level(png_ptr, compLevel);

    pngWriteCbData buffinf {_pngbuf, 0};
    png_set_write_fn(png_ptr, (voidp) &buffinf, pngWriteCB, NULL);

    CImg<unsigned char> tmpimg(_pixbuff, _width, _height, 1, 3, true);
    cimg_forXY(tmp), x, y) {
        unsigned char r = tmpimg.atXYZC(x, y, 0, 0),
                      g = tmpimg.atXYZC(x, y, 0, 1),
                      b = tmpimg.atXYZC(x, y, 0, 2);
        rowPnts[y][3 * x] = r;
        rowPnts[y][3 * x + 1] = g;
        rowPnts[y][3 * x + 2] = b;
    }
    png_set_rows(png_ptr, info_ptr, rowPnts);

    png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

    _pngbufsize = buffinf.buffsize;

    png_destroy_write_struct(&png_ptr, &info_ptr);
    for (unsigned int r = 0; r < height; r++) free(rowPnts[r]);
    free(rowPnts);

    return;
}

void pngWriteCB(png_structp png_ptr, png_bytep data, png_size_t length) {
    pngWriteCbData * buffinf = (pngWriteCbData *) png_get_io_ptr(png_ptr);
    size_t size = buffinf->buffsize + length;

    if (buffinf->buffer) {
        buffinf->buffer = (unsigned char *) realloc(buffinf->buffer, size * sizeof(unsigned char));
    } else {
        buffinf->buffer = (unsigned char *) malloc(size * sizeof(unsigned char));
    }

    if (!buffinf->buffer) {
        png_error(png_ptr, "Out of memory");
        return;
    }

    memcpy(buffinf->buffer + buffinf->buffsize, data, length);
    buffinf->buffsize += length;
}

void EncodeToPngBufferWorker::HandleOKCallback () {
    NanScope();
    Local<Value> argv[] = {
        NanNull(),
        NanBufferUse(
            _pngbuf,
            _pngbufsize
        )
    };
    callback->Call(2, argv);
}

// encoder.jpeg(pixbuf, width, height, quality, callback)
NAN_METHOD(encodeToJpegBuffer) {
    NanScope();

    Local<Object> buff = args[0].As<Object>();
    unsigned char * pixbuf = (unsigned char *) Buffer::Data(buff);
    size_t width = args[1].As<Number>();
    size_t height = args[2].As<Number>();
    int quality = args[3].As<Number>();
    NanCallback * callback = new NanCallback(args[4].As<Function>());

    NanAsyncQueueWorker(
        new EncodeToJpegBufferWorker(
            pixbuf,
            width,
            height,
            quality,
            callback
        ));
    NanReturnUndefined();
}

// encoder.png(pixbuf, width, height, compression, interlaced, callback)
NAN_METHOD(encodeToPngBuffer) {
    NanScope();

    Local<Object> buff = args[0].As<Object>();
    char * pixbuf = Buffer::Data(buff);
    size_t width = args[1].As<Number>();
    size_t height = args[2].As<Number>();
    int compression = args[3].As<Number>();
    bool interlaced = args[4].As<Boolean>();
    NanCallback * callback = new NanCallback(args[5].As<Function>());

    NanAsyncQueueWorker(
        new EncodeToPngBufferWorker(
            pixbuf,
            width,
            height,
            compression,
            interlaced,
            callback
        )
    );
    NanReturnUndefined();
}

// create an init function for our node module
void InitAll(Handle<Object> exports) {
    exports->Set(NanNew("jpeg"),
                 NanNew<FunctionTemplate>(encodeToJpegBuffer)->GetFunction());
    exports->Set(NanNew("png"),
                 NanNew<FunctionTemplate>(encodeToPngBuffer)->GetFunction());
}

// use NODE_MODULE macro to register our module:
NODE_MODULE(lwip_encoder, InitAll)
