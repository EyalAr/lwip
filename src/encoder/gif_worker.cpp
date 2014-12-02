#include "encoder.h"

#define RGB_N_CHANNELS 3
#define RGBA_N_CHANNELS 4

EncodeToGifBufferWorker::EncodeToGifBufferWorker(
    Local<Object> & buff,
    size_t width,
    size_t height,
    int colorsExp,
    bool interlaced,
    bool trans,
    NanCallback * callback
): NanAsyncWorker(callback), _width(width), _height(height),
    _colorsExp(colorsExp), _interlaced(interlaced), _trans(trans),
    _gifbuf(NULL), _gifbufsize(0) {
    SaveToPersistent("buff", buff); // make sure buff isn't GC'ed
    _pixbuf = (unsigned char *) Buffer::Data(buff);
}

EncodeToGifBufferWorker::~EncodeToGifBufferWorker() {}

void EncodeToGifBufferWorker::Execute () {

    (void) _trans;

    int cmapSize = 1 << _colorsExp; // power of 2
    GifByteType
        * redBuff = (GifByteType *) _pixbuf,
        * greenBuff = (GifByteType *) _pixbuf + _width * _height,
        * blueBuff = (GifByteType *) _pixbuf + 2 * _width * _height,
        * gifimgbuf = (GifByteType *) malloc(_width * _height * sizeof(GifByteType)); // the indexed image
    ColorMapObject *cmap;
    SavedImage * simg;

    if (NULL == gifimgbuf){
        SetErrorMessage("Out of memory");
        return;
    }

    cmap = GifMakeMapObject(cmapSize, NULL);

    if (NULL == cmap){
        free(gifimgbuf);
        SetErrorMessage("Out of memory");
        return;
    }

    if (GIF_ERROR == GifQuantizeBuffer(
                _width, _height, &cmapSize,
               redBuff, greenBuff, blueBuff,
               gifimgbuf, cmap->Colors
        )){
        free(gifimgbuf);
        GifFreeMapObject(cmap);
        SetErrorMessage("Unable to quantize image");
        return;
    }

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
    gif->SColorResolution = cmapSize;
    gif->SBackGroundColor = 0;
    gif->SColorMap = cmap;

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
    simg->RasterBits = gifimgbuf;

    if (GIF_ERROR == EGifSpew(gif)){
        EGifCloseFile(gif, &errcode);
        SetErrorMessage(GifErrorString(gif->Error));
        return;
    }

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
