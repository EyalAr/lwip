#include "encoder.h"

EncodeToJpegBufferWorker::EncodeToJpegBufferWorker(
    unsigned char * pixbuf,
    size_t width,
    size_t height,
    int quality,
    NanCallback * callback
): NanAsyncWorker(callback), _width(width), _height(height),
    _quality(quality), _jpegbuf(NULL), _jpegbufsize(0) {
    // pixbuf needs to be copied, because the buffer may be gc'ed by
    // V8 at any time.
    // !!! _pixbuf still needs to be freed by us when no longer needed (see Execute)
    _pixbuf = (unsigned char *) malloc(width * height * 3 * sizeof(unsigned char));
    if (_pixbuf == NULL) {
        // TODO: check - can I use SetErrorMessage here?
        SetErrorMessage("Out of memory");
        return;
    }
    memcpy(_pixbuf, pixbuf, width * height * 3 * sizeof(unsigned char));
}

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
        free(_pixbuf);
        SetErrorMessage("JPEG compression error");
        return;
    }

    tmp = (unsigned char *) malloc(_width * dimbuf);
    if (tmp == NULL) {
        free(_pixbuf);
        SetErrorMessage("Out of memory");
        return;
    }

    jpeg_create_compress(&cinfo);
    jpeg_mem_dest(&cinfo, (unsigned char **) &_jpegbuf, (unsigned long *) &_jpegbufsize);
    cinfo.image_width = _width;
    cinfo.image_height = _height;
    cinfo.input_components = dimbuf;
    cinfo.in_color_space = colortype;
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, _quality, TRUE);
    jpeg_start_compress(&cinfo, TRUE);

    // shared memory cimg. no new memory is allocated.
    CImg<unsigned char> tmpimg(_pixbuf, _width, _height, 1, 3, true);
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
    free(_pixbuf);
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
