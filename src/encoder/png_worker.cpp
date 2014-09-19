#include "encoder.h"

#define RGB_N_CHANNELS 3
#define RGBA_N_CHANNELS 4

EncodeToPngBufferWorker::EncodeToPngBufferWorker(
    unsigned char * pixbuf,
    size_t width,
    size_t height,
    int compression,
    bool interlaced,
    bool trans,
    NanCallback * callback
): NanAsyncWorker(callback), _width(width), _height(height),
    _compression(compression), _interlaced(interlaced), _trans(trans),
    _pngbuf(NULL), _pngbufsize(0) {
    // pixbuf needs to be copied, because the buffer may be gc'ed by
    // V8 at any time.
    // !!! _pixbuf still needs to be freed by us when no longer needed (see Execute)
    _pixbuf = (unsigned char *) malloc(
                  width *
                  height *
                  (_trans ? RGBA_N_CHANNELS : RGB_N_CHANNELS) *
                  sizeof(unsigned char)
              );
    if (_pixbuf == NULL) {
        // TODO: check - can I use SetErrorMessage here?
        SetErrorMessage("Out of memory");
        return;
    }
    // pixbuf is actually RGBA (4 channels), but we copy the A channel (which is
    // at the end of the buffer) only if the image is marked as transparent.
    memcpy(
        _pixbuf,
        pixbuf,
        width *
        height *
        (_trans ? RGBA_N_CHANNELS : RGB_N_CHANNELS) *
        sizeof(unsigned char)
    );
}

EncodeToPngBufferWorker::~EncodeToPngBufferWorker() {}

void EncodeToPngBufferWorker::Execute () {
    int n_chan = _trans ? RGBA_N_CHANNELS : RGB_N_CHANNELS;
    unsigned int rowBytes = _width * n_chan;
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
        free(_pixbuf);
        SetErrorMessage("Out of memory");
        return;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, (png_infopp) NULL);
        free(_pixbuf);
        SetErrorMessage("Out of memory");
        return;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        free(_pixbuf);
        SetErrorMessage("PNG compression error");
        return;
    }

    unsigned char ** rowPnts = (unsigned char **) malloc(
                                   _height * sizeof(unsigned char *)
                               );
    if (!rowPnts) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        free(_pixbuf);
        SetErrorMessage("Out of memory");
        return;
    }
    for (unsigned int r = 0; r < _height; r++) {
        rowPnts[r] = (unsigned char *) malloc(rowBytes * sizeof(unsigned char));
        if (!rowPnts[r]) {
            // free previous allocations
            for (unsigned int p = 0 ; p < r ; p++) free(rowPnts[p]);
            free(rowPnts);
            png_destroy_write_struct(&png_ptr, &info_ptr);
            free(_pixbuf);
            SetErrorMessage("Out of memory");
            return;
        }
    }

    png_set_IHDR(
        png_ptr,
        info_ptr,
        _width,
        _height,
        8,
        _trans ? PNG_COLOR_TYPE_RGBA : PNG_COLOR_TYPE_RGB,
        interlaceType,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT
    );
    png_set_compression_level(png_ptr, compLevel);

    pngWriteCbData buffinf = {NULL, 0};
    png_set_write_fn(png_ptr, (voidp) &buffinf, pngWriteCB, NULL);

    CImg<unsigned char> tmpimg(
        _pixbuf,
        _width,
        _height,
        1,
        n_chan,
        true
    );
    cimg_forXY(tmpimg, x, y) {
        unsigned char r = tmpimg.atXYZC(x, y, 0, 0),
                      g = tmpimg.atXYZC(x, y, 0, 1),
                      b = tmpimg.atXYZC(x, y, 0, 2),
                      a = _trans ? tmpimg.atXYZC(x, y, 0, 3) : 0;
        a = (unsigned char) ((a / 100.0) * 255.0); // range change [0,100] -> [0,255]
        rowPnts[y][n_chan * x] = r;
        rowPnts[y][n_chan * x + 1] = g;
        rowPnts[y][n_chan * x + 2] = b;
        if (_trans) rowPnts[y][n_chan * x + 3] = a;
    }
    png_set_rows(png_ptr, info_ptr, rowPnts);

    png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

    png_destroy_write_struct(&png_ptr, &info_ptr);
    for (unsigned int r = 0; r < _height; r++) free(rowPnts[r]);
    free(rowPnts);
    free(_pixbuf);

    _pngbuf = (char *) buffinf.buff;
    _pngbufsize = buffinf.buffsize;

    return;
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

void pngWriteCB(png_structp png_ptr, png_bytep data, png_size_t length) {
    pngWriteCbData * buffinf = (pngWriteCbData *) png_get_io_ptr(png_ptr);
    size_t size = buffinf->buffsize + length;

    if (buffinf->buff) {
        buffinf->buff = (unsigned char *) realloc(buffinf->buff, size * sizeof(unsigned char));
    } else {
        buffinf->buff = (unsigned char *) malloc(size * sizeof(unsigned char));
    }

    if (!buffinf->buff) {
        png_error(png_ptr, "Out of memory");
        return;
    }

    memcpy(buffinf->buff + buffinf->buffsize, data, length);
    buffinf->buffsize += length;
}
