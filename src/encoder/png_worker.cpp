#include "encoder.h"

EncodeToPngBufferWorker::EncodeToPngBufferWorker(
    unsigned char * pixbuf,
    size_t width,
    size_t height,
    int compression,
    bool interlaced,
    NanCallback * callback
): NanAsyncWorker(callback), _pixbuf(pixbuf), _width(width), _height(height),
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
    for (unsigned int r = 0; r < _height; r++) {
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

    pngWriteCbData buffinf = {(unsigned char *) _pngbuf, 0};
    png_set_write_fn(png_ptr, (voidp) &buffinf, pngWriteCB, NULL);

    CImg<unsigned char> tmpimg(_pixbuf, _width, _height, 1, 3, true);
    cimg_forXY(tmpimg, x, y) {
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
    for (unsigned int r = 0; r < _height; r++) free(rowPnts[r]);
    free(rowPnts);

    return;
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
