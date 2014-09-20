#include "decoder.h"

string toRGBA(CImg<unsigned char> ** img) {
    CImg<unsigned char> * tmp = NULL;
    // need to convert image to 4 channels RGBA spectrum?
    int spectrum = (*img)->spectrum();
    if (spectrum == 4) { // RGBA
        cimg_forXYZ(**img, x, y, z) {
            unsigned char r = (*img)->atXYZC(x, y, z, 0),
                          g = (*img)->atXYZC(x, y, z, 1),
                          b = (*img)->atXYZC(x, y, z, 2),
                          a = (*img)->atXYZC(x, y, z, 3);
            a = (unsigned char) ((a / 255.0) * 100.0); // range change [0,255] -> [0,100]
            (*img)->fillC(x, y, z, r, g, b, a);
        }
    } else if (spectrum == 1) { // grayscale
        try {
            // create a 4-channels image of the same dimensions.
            // TODO: maybe can realloc current image and do it in-place?
            tmp = new CImg<unsigned char>(**img, "x,y,1,4");
        } catch (CImgException e) {
            if (tmp) delete tmp;
            return "Out of memory";
        }
        // copy the gray value of very pixel to each of R,G and B
        // and set alpha=100%
        cimg_forXYZ(**img, x, y, z) {
            unsigned char c = (*img)->atXYZC(x, y, z, 0);
            tmp->fillC(x, y, z, c, c, c, 100);
        }
        delete *img;
        *img = tmp;
    } else if (spectrum == 2) { // grayscale + alpha
        try {
            // create a 4-channels image of the same dimensions.
            // TODO: maybe can realloc current image and do it in-place?
            tmp = new CImg<unsigned char>(**img, "x,y,1,4");
        } catch (CImgException e) {
            if (tmp) delete tmp;
            return "Out of memory";
        }
        // copy the gray value of very pixel to each of R,G and B
        // and set alpha=alpha
        cimg_forXYZ(**img, x, y, z) {
            unsigned char c = (*img)->atXYZC(x, y, z, 0),
                          a = (*img)->atXYZC(x, y, z, 1);
            a = (unsigned char) ((a / 255.0) * 100.0); // range change [0,255] -> [0,100]
            tmp->fillC(x, y, z, c, c, c, a);
        }
        delete *img;
        *img = tmp;
    } else if (spectrum == 3) { // RGB
        try {
            // create a 4-channels image of the same dimensions.
            // TODO: maybe can realloc current image and do it in-place?
            tmp = new CImg<unsigned char>(**img, "x,y,1,4");
        } catch (CImgException e) {
            if (tmp) delete tmp;
            return "Out of memory";
        }
        // copy the RGB values of very pixel to each of R,G and B
        // and set alpha=100%
        cimg_forXYZ(**img, x, y, z) {
            unsigned char r = (*img)->atXYZC(x, y, z, 0),
                          g = (*img)->atXYZC(x, y, z, 1),
                          b = (*img)->atXYZC(x, y, z, 2);
            tmp->fillC(x, y, z, r, g, b, 100);
        }
        delete *img;
        *img = tmp;
    } else {
        return "Unsupported number of channels";
    }
    return "";
}
