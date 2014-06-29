/*
#
#  File        : cimg_ipl.h
#                ( C++ header file - CImg plug-in )
#
#  Description : CImg plug-in providing the CImg->IPL and IPL->CImg
#                conversions for generic image types
#                ( IPL = Intel Performance Library )
#                This file is a part of the CImg Library project.
#                ( http://cimg.sourceforge.net )
#
#  Copyright   : Hon-Kwok Fung (oldfung@graduate.hku.hk)
#
#  License     : CeCILL v2.0
#                ( http://www.cecill.info/licences/Licence_CeCILL_V2-en.html )
#
#  This software is governed by the CeCILL  license under French law and
#  abiding by the rules of distribution of free software.  You can  use,
#  modify and/ or redistribute the software under the terms of the CeCILL
#  license as circulated by CEA, CNRS and INRIA at the following URL
#  "http://www.cecill.info".
#
#  As a counterpart to the access to the source code and  rights to copy,
#  modify and redistribute granted by the license, users are provided only
#  with a limited warranty  and the software's author,  the holder of the
#  economic rights,  and the successive licensors  have only  limited
#  liability.
#
#  In this respect, the user's attention is drawn to the risks associated
#  with loading,  using,  modifying and/or developing or reproducing the
#  software by the user in light of its specific status of free software,
#  that may mean  that it is complicated to manipulate,  and  that  also
#  therefore means  that it is reserved for developers  and  experienced
#  professionals having in-depth computer knowledge. Users are therefore
#  encouraged to load and test the software's suitability as regards their
#  requirements in conditions enabling the security of their systems and/or
#  data to be ensured and,  more generally, to use and operate it in the
#  same conditions as regards security.
#
#  The fact that you are presently reading this means that you have had
#  knowledge of the CeCILL license and that you accept its terms.
#
#
#
# Usage        :
#
# In your application code, #define the path of this plugin file as
# something like
#
#   #define cimg_plugin1 "../some_directory/cimg_ipl.h"
#
# You should define such macro before the line #include <CImg.h>.  The source
# code of CImg provides eight slots cimg_plugin1, cimg_plugin2, ...,
# cimg_plugin8 for insertion of plugins.  You may assign a different slot to
# this plugin if cimg_plugin1 is already occupied.
#
# You need also to include prior to CImg.h the following files :
#
#   #include <cstdlib>
#   #include <typeinfo>
#
# To create an IplImage from a CImg instance, you may write:
#
#   // Given a CImg instance, say, c_img, ...
#   IplImage *img = c_img.get_IplImage();  // (a) copy construction of IplImage
#
# CImg supports any number of color channels, while IplImage supports up to 4
# channels.  When the number of channels is 1 or 2, it is hard to tell if these
# channels have genuine color semantics.  Even if the number of channels is 3,
# CImg and IplImage can have different channel orders (IplImage: usually BGR;
# CImg: always RGB).  The default behaviour of get_IplImage() is to assume that
# the IplImage instance has a BGR channel order (which is the default order in
# OpenCV) and swap the channel order in the destination image buffer. That is,
# the default is to map OpenCV's blue (1st) and red (3rd) channels to CImg's
# blue (2nd) and red (0th) channel respectively.  If the user wants to specify
# this default option explicitly, he/she can write:
#
#   IplImage *img = c_img.get_IplImage(CV_CVTIMG_SWAP_RB);  // identical to (a)
#
# where CV_CVTIMG_SWAP_RB is a flag value defined by OpenCV.  If the user wants
# to keep the channel order unchanged (i.e. maps IplImage's 1st, 2nd, ...
# channels to CImg's 0th, 1st, ... channels resp.), he/she can use a zero flag
# value:
#
#   IplImage *img = c_img.get_IplImage(0);
#
# However, when the number of channels is smaller than 3, this option will be
# ignored and the default behaviour (flag value CV_CVTIMG_SWAP_RB) will be
# assumed.
#
# CImg also differs from IplImage in that the latter represents a 2D image but
# the former can be a 3D image.  If the size of the z-dimension (depth) of the
# CImg instance is larger than 1, one must choose which slice to copy:
#
#   IplImage *img1 = c_img.get_IplImage(0, z);
#   IplImage *img2 = c_img.get_IplImage(CV_CVTIMG_SWAP_RB, z);
#
# The default z-value is 0.
#
# To do conversion in another direction, write something like this:
#
#   // Suppose img1 and img2 are two pointers to IplImage, where
#   // img1->depth == IPL_DEPTH_8U and img2->depth == IPL_DEPTH_32F.
#   CImg<unsigned char> c_img1(img1);                    // (b)
#   CImg<unsigned char> c_img1(img1,CV_CVTIMG_SWAP_RB);  // identical to (b)
#   CImg<float>         c_img2(img2);                    // (c)
#   CImg<float>         c_img2(img2,CV_CVTIMG_SWAP_RB);  // identical to (c)
#
# Again, if one wants to keep the channel order unchanged when the number of
# channels is >= 3, one can write:
#
#   CImg<unsigned char> c_img1(img1,0);
#   CImg<float>         c_img2(img2,0);
#
# All such conversions are deep copy constructions, because CImg and IplImage
# have different internal memory layouts.
#
# Technically, we can write code to do conversion between an IplImage instance
# and a CImg instance with different pixel types (e.g. between an IPL_DEPTH_8S
# IplImage instance and a CImg<unsigned char> instance), but such conversion is
# problematic because either the semantics of the pixel type is lost or some
# casting is needed.  Therefore, the conversion code in this plugin only allows
# conversions of images of identical pixel types.  For instance, in line (b) of
# the example code above, if one writes
#
#   CImg<char> c_img1(img1);  // error; img1's pixel type is IPL_DEPTH_8U
#
# the conversion will generate a runtime error, despite sizeof(char) is equal
# to sizeof(unsigned char).  The is certainly inconvenient to some users as
# the pixel type of CImg has to be defined at compile time but the pixel type
# of IplImage is determined at runtime.
#
# Some architecture-dependent code is contained in the two helper functions
#
#    bool not_pixel_type_of(const IplImage*)
#
# and
#
#    int get_ipl_bit_depth() const
#
# which establish correspondences between IplImage's pixel type and C++ data
# type.  For example, they assume that IPL_DEPTH_16S corresponds to a signed
# short and IPL_DEPTH_64F corresponds to a signed double, etc..  Change the
# code if necessary.
#
# Currently, this plugin provides only conversions of OpenCV IplImage instances
# to and from CImg instances.  Conversions of general IplImage instances (e.g.
# those with bit-depth IPL_DEPTH_1U or those with origin==1) are not supported.
# Yet the conversion code has taken care of the data alignment to 4-byte or
# 8-byte boundary as well as the use of both interleaved and non-interleaved
# color channels in IplImage.
*/

#ifndef IPL_INTERFACE_H
#define IPL_INTERFACE_H

//----------------------------
// Architecture-dependent helper functions; change to suit your needs
//----------------------------

// Check if this CImg<T> instance and a given IplImage have identical pixel types.
bool not_pixel_type_of(const IplImage *const img) const {
  // to do : handle IPL_DEPTH_1U?
  return (((unsigned int)img->depth == IPL_DEPTH_8U  && typeid(T) != typeid(unsigned char)) ||
          ((unsigned int)img->depth == IPL_DEPTH_8S  && typeid(T) != typeid(char)) ||
          ((unsigned int)img->depth == IPL_DEPTH_16U && typeid(T) != typeid(unsigned short)) ||
          ((unsigned int)img->depth == IPL_DEPTH_16S && typeid(T) != typeid(unsigned)) ||
          ((unsigned int)img->depth == IPL_DEPTH_32S && typeid(T) != typeid(int)) ||
          ((unsigned int)img->depth == IPL_DEPTH_32F && typeid(T) != typeid(float)) ||
          ((unsigned int)img->depth == IPL_DEPTH_64F && typeid(T) != typeid(double)));
}

// Given this CImg<T> instance, return the corresponding bit-depth flag for use in IplImage header.
int get_ipl_bit_depth() const {
  // to do : handle IPL_DEPTH_1U?
  if (typeid(T) == typeid(unsigned char))  return IPL_DEPTH_8U;
  if (typeid(T) == typeid(char))           return IPL_DEPTH_8S;
  if (typeid(T) == typeid(unsigned short)) return IPL_DEPTH_16U;
  if (typeid(T) == typeid(short))          return IPL_DEPTH_16S;
  if (typeid(T) == typeid(int))            return IPL_DEPTH_32S;
  if (typeid(T) == typeid(float))          return IPL_DEPTH_32F;
  if (typeid(T) == typeid(double))         return IPL_DEPTH_64F;
  return 0;
}

//----------------------------
// IplImage-to-CImg conversion
//----------------------------

// Copy constructor; the optional flag will be ignored when the number of color channels is less than 3.
// Current flag options are 0 and CV_CVTIMG_SWAP_RB; may add CV_CVTIMG_FLIP and CV_CVTIMG_FLIP|CV_CVTIMG_SWAP_RB in the future.
CImg(const IplImage *const img, const int flag=0):_width(0),_height(0),_depth(0),_spectrum(0),_is_shared(false),_data(0) {
  assign(img,flag);
}

// In-place constructor; the optional flag will be ignored when the number of color channels is less than 3.
// Current flag options are 0 and CV_CVTIMG_SWAP_RB; may add CV_CVTIMG_FLIP and CV_CVTIMG_FLIP|CV_CVTIMG_SWAP_RB in the future.
CImg<T> & assign(const IplImage *const img, const int flag=CV_CVTIMG_SWAP_RB) {
  if (!img) return assign();
  if (not_pixel_type_of(img))
    throw CImgInstanceException(_cimg_instance
                                "assign(const IplImage*) : IplImage has no corresponding pixel type.",
                                cimg_instance);
  // to do: handle roi
  const int W = img->width, H = img->height;
  const char *const dataPtrI = img->imageData;
  assign(W,H,1,img->nChannels);
  char *const dataPtrC = (char *)_data;

  const int
    byte_depth = (img->depth & 255) >> 3,  // number of bytes per color
    widthStepI = img->widthStep,           // to do: handle the case img->origin==1 (currently assumption: img->origin==0)
    widthStepC = W*byte_depth,
    channelStepC = H*widthStepC;

  if (img->dataOrder==0) { // interleaved color channels
    const int pix_size = byte_depth*img->nChannels;
    for (int n = 0; n<img->nChannels; ++n) {
      const char *linePtrI  = dataPtrI + n*byte_depth;
      char *linePtrC = dataPtrC + (img->nChannels>=3 && (flag & CV_CVTIMG_SWAP_RB) && n<3?(2-n):n)*channelStepC;
      // color order is BGR in IplImage and RGB in CImg

      for (int i = 0; i<H; ++i, linePtrI+=widthStepI, linePtrC+=widthStepC) {
        const char *intensityPtrI = linePtrI;
        char *intensityPtrC = linePtrC;
        for (int j = 0; j<W; ++j, intensityPtrI+=pix_size, intensityPtrC+=byte_depth)
          std::memcpy(intensityPtrC, intensityPtrI, byte_depth);
      }
    }
  } else {  // non-interleaved color channels
    for (int n = 0; n<img->nChannels; ++n) {
      const char *linePtrI  = dataPtrI + n*byte_depth;
      char *linePtrC  = dataPtrC + (img->nChannels >= 3 && (flag & CV_CVTIMG_SWAP_RB) && n<3?(2-n):n)*channelStepC;
      for (int i = 0; i<H; ++i, linePtrI+=widthStepI, linePtrC+=widthStepC)
        std::memcpy(linePtrC, linePtrI, widthStepC);
    }
  }
  return *this;
}

//----------------------------
// CImg-to-IplImage conversion
//----------------------------
// The 'get' function; the optional flag will be ignored when the number of color channels is less than 3.
// Current flag options are 0 and CV_CVTIMG_SWAP_RB; may add CV_CVTIMG_FLIP and CV_CVTIMG_FLIP|CV_CVTIMG_SWAP_RB in future.
// z is the z-coordinate of the CImg slice that one wants to copy.
IplImage* get_IplImage(const int flag=CV_CVTIMG_SWAP_RB, const unsigned z=0) const {
  const int bit_depth = get_ipl_bit_depth();
  if (!bit_depth)
    throw CImgInstanceException(_cimg_instance
                                "get_IplImage() : IplImage has no corresponding pixel type.",
                                cimg_instance);
    if (is_empty())
    throw CImgArgumentException(_cimg_instance
                                "get_IplImage() : Empty instance.",
                                cimg_instance);
  if (z>=_depth)
    throw CImgInstanceException(_cimg_instance
                                "get_IplImage() : Instance has not Z-dimension %u.",
                                cimg_instance,
                                z);
  if (_spectrum>4)
    cimg::warn(_cimg_instance
               "get_IplImage() : OpenCV supports only 4 channels, so only the first four will be copied.",
               cimg_instance);

  IplImage *const img = cvCreateImage(cvSize(_width,_height),bit_depth,_spectrum);
  const int
    W = _width,
    H = _height,
    byte_depth = (img->depth & 255) >> 3,  // number of bytes per color
    widthStepI = img->widthStep,           // to do: handle the case img->origin==1 (current assumption: img->origin==0)
    widthStepC = W*byte_depth,
    channelStepC = H*_depth*widthStepC;
  const char *const dataPtrC = (char*)_data + z*H*widthStepC;
  char *const dataPtrI = img->imageData;

  if (!img->dataOrder) {  // interleaved color channels
    const int pix_size = byte_depth*img->nChannels;
    for (int n = 0; n<img->nChannels; ++n) {
      const char *linePtrC  = dataPtrC + (img->nChannels >= 3 && (flag & CV_CVTIMG_SWAP_RB) && n<3?(2-n):n)*channelStepC;
      char *linePtrI  = dataPtrI + n*byte_depth;

      // color order is BGR in IplImage and RGB in CImg
      for (int i = 0; i<H; ++i, linePtrI+=widthStepI, linePtrC+=widthStepC) {
        const char *intensityPtrC = linePtrC;
        char *intensityPtrI = linePtrI;
        for (int j = 0; j<W; ++j, intensityPtrI+=pix_size, intensityPtrC+=byte_depth)
          std::memcpy(intensityPtrI, intensityPtrC, byte_depth);
      }
    }
  } else {  // non-interleaved color channels
    for (int n = 0; n<img->nChannels; ++n) {
      const char *linePtrC  = dataPtrC + (img->nChannels>= 3 && (flag & CV_CVTIMG_SWAP_RB) && n<3?(2-n):n)*channelStepC;
      char *linePtrI = dataPtrI + n*byte_depth;
      for (int i = 0; i<H; ++i, linePtrI+=widthStepI, linePtrC+=widthStepC)
        std::memcpy(linePtrI, linePtrC, widthStepC);
    }
  }
  return img;
}

#endif
