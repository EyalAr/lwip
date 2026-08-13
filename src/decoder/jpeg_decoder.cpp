#include "decoder.h"

string decode_jpeg_buffer(char * buffer, size_t size, CImg<unsigned char> ** cimg, char ** metadata) {
    struct jpeg_decompress_struct cinfo;
    struct lwip_jpeg_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = lwip_jpeg_error_exit;

    if (setjmp(jerr.setjmp_buffer)) {
        jpeg_destroy_decompress(&cinfo);
        if (*cimg) delete *cimg;
        return "JPEG decoding error";
    }

    jpeg_create_decompress(&cinfo);
    jpeg_mem_src(&cinfo, (unsigned char *) buffer, size);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);

    if (cinfo.output_components != 1 && cinfo.output_components != 3) {
        // unsupported number of channels
        jpeg_destroy_decompress(&cinfo);
        return "Unsupported number of channels";
    }
    JSAMPROW row_pointer[1];
    *cimg = new CImg<unsigned char>();
    CImg<unsigned char> * buffimg_ptr;
    try {
        buffimg_ptr = new CImg<unsigned char>(cinfo.output_width * cinfo.output_components);
        (*cimg)->assign(cinfo.output_width, cinfo.output_height, 1, cinfo.output_components);
    }
    catch (CImgException &e) {
        jpeg_destroy_decompress(&cinfo);
        delete *cimg;
        *cimg = NULL;
        return "JPEG dimensions too large to allocate";
    }
    CImg<unsigned char> & buffimg = *buffimg_ptr;
    unsigned char * ptr_r = (*cimg)->data(),
                    * ptr_g = (*cimg)->data() + 1UL * (*cimg)->width() * (*cimg)->height(),
                      * ptr_b = (*cimg)->data() + 2UL * (*cimg)->width() * (*cimg)->height();
    while (cinfo.output_scanline < cinfo.output_height) {
        *row_pointer = buffimg._data;
        if (jpeg_read_scanlines(&cinfo, row_pointer, 1) != 1) {
            //warning: incomplete data in buffer
            break;
        }
        const unsigned char * ptrs = buffimg._data;
        switch ((*cimg)->spectrum()) {
        case 1 :
            cimg_forX(**cimg, x) *(ptr_r++) = (unsigned char) * (ptrs++);
            break;
        case 3 :
            cimg_forX(**cimg, x) {
                *(ptr_r++) = (unsigned char) * (ptrs++);
                *(ptr_g++) = (unsigned char) * (ptrs++);
                *(ptr_b++) = (unsigned char) * (ptrs++);
            }
            break;
        }
    }
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    delete buffimg_ptr;
    // TODO: implement getting metadata from GIFs; this is a placeholder
    *metadata = (char *)malloc(sizeof(char));
    *metadata[0] = '\0';

    return "";
}
