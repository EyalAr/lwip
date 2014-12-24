#include "decoder.h"

#define ALPHA_TRANS 0
#define ALPHA_OPAQUE 255
#define C_TRANS 0

string decode_gif_buffer(char * buffer, size_t size, CImg<unsigned char> ** cimg) {

    gifReadCbData buffinf = {(unsigned char *) buffer, size, 0};
    GifFileType * gif = NULL;
    int errcode = 0;

    // buffinf will be available in gifReadCB as gif->userData
    gif = DGifOpen((void *) &buffinf, gifReadCB, &errcode);

    if (NULL == gif)
        return GifErrorString(errcode);

    if (GIF_ERROR == DGifSlurp(gif))
        return GifErrorString(gif->Error);

    GraphicsControlBlock gcb;

    // only for the first image
    bool hasGCB = DGifSavedExtensionToGCB(gif, 0, &gcb) != GIF_ERROR;
    // if may return GIF_ERROR, which means this image has no gcb.
    // that's fine, as gcb's are optional

    SavedImage * gifimg = &gif->SavedImages[0];
    size_t width = (size_t) gifimg->ImageDesc.Width;
    size_t height = (size_t) gifimg->ImageDesc.Height;
    GifByteType * ipxls = gifimg->RasterBits; // pixels (indexed)
    ColorMapObject * cmap = NULL != gifimg->ImageDesc.ColorMap
        ? gifimg->ImageDesc.ColorMap
        : gif->SColorMap;

    // allocate a CImg with the correct dimensions as the image
    *cimg = new CImg<unsigned char>();
    (*cimg)->assign(width, height, 1, 4);

    // pointers to RGBA sections in CImg
    unsigned char *ptr_r = (*cimg)->data(0, 0, 0, 0),
                    *ptr_g = (*cimg)->data(0, 0, 0, 1),
                     *ptr_b = (*cimg)->data(0, 0, 0, 2),
                      *ptr_a = (*cimg)->data(0, 0, 0, 3);

    size_t i = 0, len = width * height;
    GifByteType ci;
    GifColorType c;
    GifColorType c_trans = {C_TRANS, C_TRANS, C_TRANS};
    unsigned char alpha;

    for (; i < len; i++){
        ci = ipxls[i];
        if (hasGCB && ci == gcb.TransparentColor){
            c = c_trans;
            alpha = ALPHA_TRANS;
        } else {
            c = cmap->Colors[ci];
            alpha = ALPHA_OPAQUE;
        }
        *(ptr_r++) = c.Red;
        *(ptr_g++) = c.Green;
        *(ptr_b++) = c.Blue;
        *(ptr_a++) = alpha;
    }

    if (GIF_ERROR == DGifCloseFile(gif, &errcode)){
        delete *cimg;
        return GifErrorString(errcode);
    }

    return "";
}

// 'gif' gives us access to our user data, which has a pointer to our
// buffinf struct.
// 'buf' is the buffer into which we need to copy the data
// 'length' is how many bytes we need to copy
// we should return how many bytes were actually copied
int gifReadCB(GifFileType * gif, GifByteType * buf, int length) {
    gifReadCbData * buffinf = (gifReadCbData *) gif->UserData;

    // need to read 'length' bytes from the source buffer and copy them to buf.

    if (buffinf->read + length > buffinf->size) {
        // no more bytes in source
        memcpy(buf, buffinf->src + buffinf->read, buffinf->size - buffinf->read);
        buffinf->read = buffinf->read;
        return buffinf->size - buffinf->read;
    }

    memcpy(buf, buffinf->src + buffinf->read, length);
    buffinf->read += length;

    return length;
}
