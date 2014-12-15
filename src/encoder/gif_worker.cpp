#include "encoder.h"

#define RGB_N_CHANNELS 3
#define RGBA_N_CHANNELS 4

EncodeToGifBufferWorker::EncodeToGifBufferWorker(
    Local<Object> & buff,
    size_t width,
    size_t height,
    int cmapSize,
    int colors,
    bool interlaced,
    bool trans,
    NanCallback * callback
): NanAsyncWorker(callback), _width(width), _height(height),
    _cmapSize(cmapSize), _colors(colors), _interlaced(interlaced), _trans(trans),
    _gifbuf(NULL), _gifbufsize(0) {
    SaveToPersistent("buff", buff); // make sure buff isn't GC'ed
    _pixbuf = (unsigned char *) Buffer::Data(buff);
    // if (_trans){
    //     // make room in the color table for a transparent color
    //     if (_cmapSize == 256){
    //         if (_colors == 256) _colors--;
    //     } else if (_cmapSize == _colors){
    //         _cmapSize *= 2;
    //     }
    // }
}

EncodeToGifBufferWorker::~EncodeToGifBufferWorker() {}

void EncodeToGifBufferWorker::Execute () {

    printf("width: %d\nheight: %d\ncmapSize: %d\ncolors: %d\ninterlaced: %s\ntrans: %s\n\n", _width, _height, _cmapSize, _colors, _interlaced ? "yes" : "no", _trans ? "yes" : "no");

    GifByteType
        * redBuff = (GifByteType *) _pixbuf,
        * greenBuff = (GifByteType *) _pixbuf + _width * _height,
        * blueBuff = (GifByteType *) _pixbuf + 2 * _width * _height,
        * alphaBuff = (GifByteType *) _pixbuf + 3 * _width * _height,
        * gifimgbuf = (GifByteType *) malloc(_width * _height * sizeof(GifByteType)); // the indexed image
    ColorMapObject *cmap;
    SavedImage * simg;

    if (NULL == gifimgbuf){
        SetErrorMessage("Out of memory");
        return;
    }

    cmap = GifMakeMapObject(_cmapSize, NULL);

    if (NULL == cmap){
        free(gifimgbuf);
        SetErrorMessage("Out of memory");
        return;
    }

    if (GIF_ERROR == GifQuantizeBuffer(
                _width, _height, &_colors,
               redBuff, greenBuff, blueBuff,
               gifimgbuf, cmap->Colors
        )){
        free(gifimgbuf);
        GifFreeMapObject(cmap);
        SetErrorMessage("Unable to quantize image");
        return;
    }
    // cmap->ColorCount = _colors;

    printf("colors: %d\n", _colors);

    int errcode;
    gifWriteCbData buffinf = {NULL, 0};
    GifFileType * gif;

    gif = EGifOpen((void *) &buffinf, gifWriteCB, &errcode);

    if (NULL == gif){
        free(gifimgbuf);
        GifFreeMapObject(cmap);
        SetErrorMessage(GifErrorString(errcode));
        return;
    }

    gif->SWidth = _width;
    gif->SHeight = _height;
    gif->SColorResolution = _cmapSize;

    simg = GifMakeSavedImage(gif, NULL);

    if (NULL == simg){
        free(gifimgbuf);
        EGifCloseFile(gif, &errcode); // will also free cmap
        SetErrorMessage("Out of memory");
        return;
    }

    simg->ImageDesc.Left = 0;
    simg->ImageDesc.Top = 0;
    simg->ImageDesc.Width = _width;
    simg->ImageDesc.Height = _height;
    simg->ImageDesc.Interlace = _interlaced;
    simg->ImageDesc.ColorMap = cmap;
    simg->RasterBits = gifimgbuf;

    // printf("123extblockcount %d\n", simg->ExtensionBlockCount);

    // // transperancy
    // if (_trans){
    //     printf("yes, trans\n");
    //     ExtensionBlock ext;
    //     // 1. assign transparent color index in color table
    //     GraphicsControlBlock gcb = {0, false, 0, _colors++};
    //     // 2. replace transparent pixels above threshold with this color
    //     remapTransparentPixels(gifimgbuf, alphaBuff, _width, _height, gcb.TransparentColor, 50);
    //     // gif->SBackGroundColor = gcb.TransparentColor;
    //     // 3. create a control block
    //     size_t extlen = EGifGCBToExtension(&gcb, (GifByteType *) &ext);
    //     if (GIF_ERROR == GifAddExtensionBlock(
    //             &(gif->ExtensionBlockCount),
    //             &(gif->ExtensionBlocks),
    //             GRAPHICS_EXT_FUNC_CODE,
    //             extlen,
    //             (unsigned char *) &ext)
    //         ) {
    //         EGifCloseFile(gif, &errcode);
    //         SetErrorMessage("Out of memory");
    //         return;
    //     }
    // }

    if (GIF_ERROR == EGifSpew(gif)){
        EGifCloseFile(gif, &errcode);
        SetErrorMessage(GifErrorString(gif->Error));
        return;
    }

    _gifbuf = (char *) buffinf.buff;
    _gifbufsize = buffinf.buffsize;

    return;
}

void EncodeToGifBufferWorker::HandleOKCallback () {
    NanScope();
    Local<Value> argv[] = {
        NanNull(),
        NanBufferUse(
            _gifbuf,
            _gifbufsize
        )
    };
    callback->Call(2, argv);
}

int gifWriteCB(GifFileType * gif, const GifByteType * chunk, int len) {
    gifWriteCbData * buffinf = (gifWriteCbData *) gif->UserData;
    size_t size = buffinf->buffsize + len;

    if (NULL != buffinf->buff)
        buffinf->buff = (unsigned char *) realloc(buffinf->buff, size * sizeof(GifByteType));
    else
        buffinf->buff = (unsigned char *) malloc(size * sizeof(GifByteType));

    if (!buffinf->buff) return 0;

    memcpy(buffinf->buff + buffinf->buffsize, chunk, len * sizeof(GifByteType));
    buffinf->buffsize += len;

    return len;
}

void remapTransparentPixels(unsigned char * target, const unsigned char * map, size_t width, size_t height, int transColor, int threshold){
    // printf("transcolor: %d, threshold: %d\n", transColor, threshold);
    size_t i = 0, len = width * height;
    for (; i < len; i++){
        if (map[i] < threshold) *(target + i) = transColor;
        // printf("%d,", target[i]);
    }
    // printf("\n");
}
