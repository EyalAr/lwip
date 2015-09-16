#include "encoder.h"

#define RGB_N_CHANNELS 3

EncodeToJpegBufferWorker::EncodeToJpegBufferWorker(
    Local<Object> & buff,
    size_t width,
    size_t height,
    int quality,
    Nan::Callback * callback
): Nan::AsyncWorker(callback), _width(width), _height(height),
    _quality(quality), _jpegbuf(NULL), _jpegbufsize(0) {
    SaveToPersistent("buff", buff); // make sure buff isn't GC'ed
    _pixbuf = (unsigned char *) Buffer::Data(buff);
}

EncodeToJpegBufferWorker::~EncodeToJpegBufferWorker() {}

void EncodeToJpegBufferWorker::Execute () {
    unsigned int dimbuf = RGB_N_CHANNELS;
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
    jpeg_mem_dest(&cinfo, (unsigned char **) &_jpegbuf, (unsigned long *) &_jpegbufsize);
    cinfo.image_width = _width;
    cinfo.image_height = _height;
    cinfo.input_components = dimbuf;
    cinfo.in_color_space = colortype;
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, _quality, TRUE);
    jpeg_start_compress(&cinfo, TRUE);

    // shared memory cimg. no new memory is allocated.
    CImg<unsigned char> tmpimg(_pixbuf, _width, _height, 1, RGB_N_CHANNELS, true);
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
    Nan::HandleScope();
    Local<Value> argv[] = {
        Nan::Null(),
        Nan::NewBuffer(
            _jpegbuf,
            _jpegbufsize
        ).ToLocalChecked()
    };
    callback->Call(2, argv);
}
