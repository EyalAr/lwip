#include "decoder.h"

string decode_png_buffer(char * buffer, size_t size, CImg<unsigned char> ** cimg) {
    // check it's a valid png buffer
    if (size < 8 || png_sig_cmp((png_const_bytep) buffer, 0, 8)) {
        return "Invalid PNG buffer";
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
                          NULL, NULL, NULL);
    if (!png_ptr) {
        return "Out of memory";
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return "Out of memory";
    }

    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return "Out of memory";
    }

    // Error handling callback for png file reading
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        return "PNG decoding error";
    }

    pngReadCbData buffinf = {(unsigned char *) buffer, size, 0};
    png_set_read_fn(png_ptr, (voidp) &buffinf, pngReadCB);

    // PNG info
    png_read_info(png_ptr, info_ptr);
    png_set_sig_bytes(png_ptr, 8);

    png_uint_32 width = 0, height = 0;
    int bit_depth, color_type;
    bool is_gray = false;
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                 NULL, NULL, NULL);

    if (color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(png_ptr);
        color_type = PNG_COLOR_TYPE_RGB;
        bit_depth = 8;
    }
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
        png_set_expand_gray_1_2_4_to_8(png_ptr);
        is_gray = true;
        bit_depth = 8;
    }
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
        png_set_tRNS_to_alpha(png_ptr);
        color_type |= PNG_COLOR_MASK_ALPHA;
    }
    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
        png_set_gray_to_rgb(png_ptr);
        color_type |= PNG_COLOR_MASK_COLOR;
        is_gray = true;
    }
    if (color_type == PNG_COLOR_TYPE_RGB)
        png_set_filler(png_ptr, 0xffffU, PNG_FILLER_AFTER);

    if (!(color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_RGB_ALPHA)) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        return "Invalid PNG color coding";
    }

    if (bit_depth != 8 && bit_depth != 16) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        return "Invalid PNG bit depth";
    }

    bool is_alpha = (color_type == PNG_COLOR_TYPE_RGBA);

    png_read_update_info(png_ptr, info_ptr);
    const int byte_depth = bit_depth >> 3;

    // Allocate Memory for Image Read
    png_bytep * const imgData = new png_bytep[height];
    for (unsigned int row = 0; row < height; ++row)
        imgData[row] = new png_byte[byte_depth * N_CHANNELS * width];
    png_read_image(png_ptr, imgData);
    png_read_end(png_ptr, end_info);

    *cimg = new CImg<unsigned char>();
    (*cimg)->assign(width, height, 1, (is_gray ? 1 : 3) + (is_alpha ? 1 : 0));
    unsigned char * ptr_r = (*cimg)->data(0, 0, 0, 0),
                    *ptr_g = is_gray ? 0 : (*cimg)->data(0, 0, 0, 1),
                     *ptr_b = is_gray ? 0 : (*cimg)->data(0, 0, 0, 2),
                      *ptr_a = is_alpha ? (*cimg)->data(0, 0, 0, is_gray ? 1 : 3) : NULL;
    switch (bit_depth) {
    case 8:
        cimg_forY(**cimg, y) {
            const unsigned char * ptrs = (unsigned char *)imgData[y];
            cimg_forX(**cimg, x) {
                *(ptr_r++) = (unsigned char) * (ptrs++);
                if (ptr_g) *(ptr_g++) = (unsigned char) * (ptrs++); else ++ptrs;
                if (ptr_b) *(ptr_b++) = (unsigned char) * (ptrs++); else ++ptrs;
                if (ptr_a) *(ptr_a++) = (unsigned char) * (ptrs++); else ++ptrs;
            }
        }
        break;
    case 16:
        cimg_forY(**cimg, y) {
            const unsigned short * ptrs = (unsigned short *)(imgData[y]);
            if (!cimg::endianness()) cimg::invert_endianness(ptrs, N_CHANNELS * (*cimg)->width());
            cimg_forX(**cimg, x) {
                *(ptr_r++) = (unsigned char) * (ptrs++);
                if (ptr_g) *(ptr_g++) = (unsigned char) * (ptrs++); else ++ptrs;
                if (ptr_b) *(ptr_b++) = (unsigned char) * (ptrs++); else ++ptrs;
                if (ptr_a) *(ptr_a++) = (unsigned char) * (ptrs++); else ++ptrs;
            }
        }
        break;
    }

    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);

    // Deallocate Image Read Memory
    cimg_forY(**cimg, n) delete[] imgData[n];
    delete[] imgData;

    return "";
}

void pngReadCB(png_structp png_ptr, png_bytep data, png_size_t length) {
    pngReadCbData * buffinf = (pngReadCbData *) png_get_io_ptr(png_ptr);

    // need to read 'length' bytes from the source buffer and copy them to data.
    if (buffinf->read + length > buffinf->size) {
        // no more bytes in source
        png_error(png_ptr, "PNG source buffer is missing data");
        return;
    }

    memcpy(data, buffinf->src + buffinf->read, length);
    buffinf->read += length;

    return;
}
