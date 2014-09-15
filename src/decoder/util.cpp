#include "decoder.h"

string to3Channels(CImg<unsigned char> ** img) {
    CImg<unsigned char> * tmp = NULL;
    // need to convert image to 3 channels spectrum?
    int spectrum = (*img)->spectrum();
    if (spectrum != 3) {
        if (spectrum == 1) {
            try {
                // create a 3-channels image of the same dimensions.
                // TODO: maybe can realloc current image and do it in-place?
                tmp = new CImg<unsigned char>(**img, "x,y,1,3");
            } catch (CImgException e) {
                if (tmp) delete tmp;
                return "Out of memory";
            }
            // copy the gray value of very pixel to each of R,G and B:
            cimg_forXYZ(**img, x, y, z) {
                unsigned char c = (*img)->atXYZC(x, y, z, 0);
                tmp->fillC(x, y, z, c, c, c);
            }
            delete *img;
            *img = tmp;
        } else {
            // TODO: support 4 channels (CMYK)
            // need to find out if the 4th channel is alpha...
            return "Unsupported number of channels";
        }
    }
    return "";
}
