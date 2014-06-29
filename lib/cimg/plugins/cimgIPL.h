/*
#
#  File        : cimgIPL.h
#                ( C++ header file - CImg plug-in )
#
#  Description : CImg plug-in providing the CImg->IPL and IPL->CImg
#                conversions for generic image types
#                ( IPL = Intel Performance Library )
#                This file is a part of the CImg Library project.
#                ( http://cimg.sourceforge.net )
#
#  Copyright   : newleft (haibo.zheng@gmail.com)
#                         newleftist@hotmail.com
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
*/

#ifndef cimg_plugin_cimgIPL
#define cimg_plugin_cimgIPL

// Conversion IPL -> CImg (constructor)
CImg(const IplImage* src):_width(0),_height(0),_depth(0),_spectrum(0),_is_shared(false),_data(0) {
  assign(src);
}

// Conversion IPL -> CImg (in-place constructor)
CImg<T>& assign(const IplImage* src) {
  if (!src) return assign();
  switch (src->depth) {
  case IPL_DEPTH_1U: { // 1-bit int.
    IplImage *src1 = cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);
    cvConvert(src,src1);
    CImg<ucharT>((unsigned char*)src1->imageData,src1->nChannels,src1->width,src1->height,1,true).
      get_permute_axes("yzcx").move_to(*this);
    cvReleaseImage(&src1);
  } break;
  case IPL_DEPTH_8U: // 8-bit unsigned int.
    CImg<ucharT>((unsigned char*)src->imageData,src->nChannels,src->width,src->height,1,true).
      get_permute_axes("yzcx").move_to(*this);
    break;
  case IPL_DEPTH_8S: // 8-bit signed int.
    CImg<charT>((char*)src->imageData,src->nChannels,src->width,src->height,1,true).
      get_permute_axes("yzcx").move_to(*this);
    break;
  case IPL_DEPTH_16U: // 16-bit unsigned int.
    CImg<ushortT>((unsigned short*)src->imageData,src->nChannels,src->width,src->height,1,true).
      get_permute_axes("yzcx").move_to(*this);
    break;
  case IPL_DEPTH_16S: // 16-bit signed int.
    CImg<shortT>((short*)src->imageData,src->nChannels,src->width,src->height,1,true).
      get_permute_axes("yzcx").move_to(*this);
    break;
  case IPL_DEPTH_32S: // 32-bit signed int.
    CImg<intT>((int*)src->imageData,src->nChannels,src->width,src->height,1,true).
      get_permute_axes("yzcx").move_to(*this);
    break;
  case IPL_DEPTH_32F: // 32-bit float.
    CImg<floatT>((float*)src->imageData,src->nChannels,src->width,src->height,1,true).
      get_permute_axes("yzcx").move_to(*this);
    break;
  case IPL_DEPTH_64F: // 64-bit double.
    CImg<doubleT>((double*)src->imageData,src->nChannels,src->width,src->height,1,true).
      get_permute_axes("yzcx").move_to(*this);
    break;
  default:
    throw CImgInstanceException("CImg<%s>::assign(const IplImage* img) : IplImage depth is invalid.",
                                pixel_type());
    break;
  }
  if (!std::strcmp(src->channelSeq,"BGR")) mirror('v');
  else if (!std::strcmp(src->channelSeq,"BGRA")) get_shared_channels(0,2).mirror('v');
  return *this;
}

// Conversion CImg -> IPL
IplImage* get_IPL(const unsigned int z=0) const {
  if (is_empty())
    throw CImgInstanceException("CImg<%s>::get_IPL() : instance image (%u,%u,%u,%u,%p) is empty.",
                                pixel_type(),_width,_height,_depth,_spectrum,_data);
  if (z>=_depth)
    throw CImgInstanceException("CImg<%s>::get_IPL() : specified slice %u is out of image bounds (%u,%u,%u,%u,%p).",
                                pixel_type(),z,_width,_height,_depth,_spectrum,_data);
  const CImg<T>
    _slice = _depth>1?get_slice(z):CImg<T>(),
    &slice = _depth>1?_slice:*this;
  CImg<T> buf(slice);
  if (_spectrum==3 || _spectrum==4) buf.get_shared_channels(0,2).mirror('v');
  buf.permute_axes("cxyz");
  IplImage* const dst = cvCreateImage(cvSize(_width,_height),sizeof(T)*8,_spectrum);
  std::memcpy(dst->imageData,buf.data(),buf.size()*sizeof(T));
  return dst;
}

#endif
