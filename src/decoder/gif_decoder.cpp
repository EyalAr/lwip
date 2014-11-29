#include "decoder.h"

string decode_gif_buffer(char * buffer, size_t size, CImg<unsigned char> ** cimg) {

    gifReadCbData buffinf = {(unsigned char *) buffer, size, 0};
    GifFileType * gif;
    int errcode;

    // buffinf will be available in gifReadCB as gif->userData
    gif = DGifOpen((void *) &buffinf, gifReadCB, &errcode);

    if (NULL == gif)
        return GifErrorString(errcode);

    if (GIF_ERROR == DGifSlurp(gif))
        return GifErrorString(gif->Error);

    GraphicsControlBlock gcb;

    // only for the first image
    if (GIF_ERROR == DGifSavedExtensionToGCB(gif, 0, &gcb))
        return GifErrorString(gif->Error);

    SavedImage * gifimg = &gif->SavedImages[0];
    size_t width = (size_t) gifimg->ImageDesc.Width;
    size_t height = (size_t) gifimg->ImageDesc.Height;
    bool trans = gcb.TransparentColor != NO_TRANSPARENT_COLOR;

    // allocate a CImg with the correct dimensions as the image
    *cimg = new CImg<unsigned char>();
    (*cimg)->assign(width, height, 1, 4);

    size_t x, y, i = 0;
    GifByteType ci;
    GifColorType c;
    printf("\nHERE2:\n");
    for (y = 0; y < height; y++){
        for (x = 0; x < width; x++){
            ci = gifimg->RasterBits[i++];
            c = gifimg->ImageDesc.ColorMap->Colors[ci];
            printf("[%d %d %d],",c.Red,c.Green,c.Blue);
        }
    }
    printf("\n-----\n");

    return "no";

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
