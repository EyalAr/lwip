#ifndef LWIP_H
#define LWIP_H

#define cimg_display 0
#define cimg_use_jpeg
#define cimg_verbosity 0

#include <string>
#include <cstring>
#include <node.h>
#include <v8.h>
#include "CImg.h"
#include "LwipImage.h"

struct ImageOpenBaton {
    uv_work_t request;
    v8::Persistent<Function> cb;
    CImg<unsigned char> * imgData;
    std::string imgPath;
    bool err;
    std::string errMsg;
};

#endif
