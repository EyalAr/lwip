#ifdef GIFLIB_MAJOR
#  if GIFLIB_MAJOR > 4
#    define HAVE_GIFLIB_5
#  endif
#endif

#if !HAVE_GIFLIB_5
typedef struct GraphicsControlBlock {
    GifByteType TransparentColor;
};

static int DGifSavedExtensionToGCB(GifFileType *gif, int i, GraphicsControlBlock *gcb) {
    return GIF_ERROR;
}

#define GifMakeMapObject MakeMapObject
#define GifFreeMapObject FreeMapObject
#define GifUnionColorMap UnionColorMap
#define GifQuantizeBuffer QuantizeBuffer
#define GifMakeSavedImage MakeSavedImage
#endif

static const char * MyGifErrorString(int errcode) {
    #if HAVE_GIFLIB_5
    return GifErrorString(errcode);
    #else
    switch (errcode) {
        case D_GIF_ERR_OPEN_FAILED:
            return "Failed to open given file";

        case D_GIF_ERR_READ_FAILED:
            return "Failed to read from given file";

        case D_GIF_ERR_NOT_GIF_FILE:
            return "Data is not a GIF file"; 

        case D_GIF_ERR_NO_SCRN_DSCR:
            return "No screen descriptor detected"; 

        case D_GIF_ERR_NO_IMAG_DSCR:
            return "No image descriptor detected"; 

        case D_GIF_ERR_NO_COLOR_MAP:
            return "Neither global nor local color map"; 

        case D_GIF_ERR_WRONG_RECORD:
            return "Wrong record type detected"; 

        case D_GIF_ERR_DATA_TOO_BIG:
            return "Number of pixels bigger than width * height"; 

        case D_GIF_ERR_NOT_ENOUGH_MEM:
            return "Failed to allocate required memory"; 

        case D_GIF_ERR_CLOSE_FAILED:
            return "Failed to close given file"; 

        case D_GIF_ERR_NOT_READABLE:
            return "Given file was not opened for read"; 

        case D_GIF_ERR_IMAGE_DEFECT:
            return "Image is defective, decoding aborted"; 

        case D_GIF_ERR_EOF_TOO_SOON:
            return "Image EOF detected, before image complete"; 

        default:
            return "Unknown error"; 
    }
    #endif
}
