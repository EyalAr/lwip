/*
#
#  File        : use_cimgIPL.cpp
#                ( C++ source file )
#
#  Description : Example of use for the CImg plugin 'plugins/cimgIPL.h'.
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

#include <cv.h>
#include <highgui.h>
#include <math.h>

#pragma comment(lib, "cv.lib")
#pragma comment(lib, "cvaux.lib")
#pragma comment(lib, "cxcore.lib")
#pragma comment(lib, "highgui.lib")

#define cimg_plugin1 "plugins\cimgIPL.h"
#include "CImg.h"
using namespace cimg_library;

// Main procedure
//----------------
int main(int argc, char* argv[]) {
  int wid = 0;
  CImg<> cImg(argv[1]);
  cImg.display("cImg");
  IplImage* ipl;
  //ipl = cvLoadImage(argv[1], -1);
  ipl = cImg.get_IPL();

  IplImage *ipl8;
  IplImage *ipl16, *ipl32, *ipl64;
  IplImage *ipl16to8, *ipl32to8, *ipl64to8;
  cvNamedWindow("origin", wid++);
  cvNamedWindow("8bit_OK", wid++);
  cvNamedWindow("16bit", wid++);
  cvNamedWindow("32bit", wid++);
  cvNamedWindow("64bit", wid++);
  cvNamedWindow("16bitto8", wid++);
  cvNamedWindow("32bitto8", wid++);
  cvNamedWindow("64bitto8", wid++);

  cvShowImage("origin", ipl);

  ipl8 = cvCreateImage(cvGetSize(ipl), IPL_DEPTH_8U, ipl->nChannels);
  cvConvert(ipl, ipl8);

  ipl16 = cvCreateImage(cvGetSize(ipl), IPL_DEPTH_16U, ipl->nChannels);
  cvConvert(ipl, ipl16);

  ipl32 = cvCreateImage(cvGetSize(ipl), IPL_DEPTH_32F, ipl->nChannels);
  cvConvert(ipl, ipl32);

  ipl64 = cvCreateImage(cvGetSize(ipl), IPL_DEPTH_64F, ipl->nChannels);
  cvConvert(ipl, ipl64);

  cvShowImage("8bit_OK", ipl8);// this canbe show properly
  cvShowImage("16bit", ipl16);// maynot display properly, that's bug of cvShowImage
  cvShowImage("32bit", ipl32);// maynot display properly, that's bug of cvShowImage
  cvShowImage("64bit", ipl64);// maynot display properly, that's bug of cvShowImage

  // cvShowImage can only display IplImage with IPL_DEPTH_8X, proved by the following codes
  ipl16to8 = cvCreateImage(cvGetSize(ipl16), IPL_DEPTH_8U, ipl16->nChannels);
  cvConvert(ipl16, ipl16to8);
  ipl32to8 = cvCreateImage(cvGetSize(ipl32), IPL_DEPTH_8U, ipl32->nChannels);
  cvConvert(ipl32, ipl32to8);
  ipl64to8 = cvCreateImage(cvGetSize(ipl64), IPL_DEPTH_8U, ipl64->nChannels);
  cvConvert(ipl64, ipl64to8);
  cvShowImage("16bitto8", ipl16to8);    // diplay ok
  cvShowImage("32bitto8", ipl32to8);    // diplay ok
  cvShowImage("64bitto8", ipl64to8);    // diplay ok

  // now, we test ipl8->cImg, ipl16->cImg, ipl32->cImg, ipl64->cImg
  cImg.assign(ipl8);
  cImg.display("ipl8->cimg");
  cImg.assign(ipl16);
  cImg.display("ipl16->cimg");
  cImg.assign(ipl32);
  cImg.display("ipl32->cimg");
  cImg.assign(ipl64);
  cImg.display("ipl64->cimg");

  cvWaitKey(0);

  // test another construct
  CImg<unsigned char> testCImg1(ipl16);
  testCImg1.display("testCImg1");
  CImg<unsigned char> testCImg2(ipl32);
  testCImg2.display("testCImg2");
  CImg<unsigned char> testCImg3(ipl64);
  testCImg3.display("testCImg3");

  CImg<double> testCImg4(ipl16);
  testCImg4.display("testCImg4");
  CImg<double> testCImg5(ipl32);
  testCImg5.display("testCImg5");
  CImg<double> testCImg6(ipl64);
  testCImg6.display("testCImg6");

  cvReleaseImage(&ipl);
  cvReleaseImage(&ipl8);
  cvReleaseImage(&ipl16);
  cvReleaseImage(&ipl32);
  cvReleaseImage(&ipl64);
  cvReleaseImage(&ipl16to8);
  cvReleaseImage(&ipl32to8);
  cvReleaseImage(&ipl64to8);

  cvDestroyWindow("origin");
  cvDestroyWindow("8bit_OK");
  cvDestroyWindow("16bit");
  cvDestroyWindow("32bit");
  cvDestroyWindow("64bit");
  cvDestroyWindow("16bitto8");
  cvDestroyWindow("32bitto8");
  cvDestroyWindow("64bitto8");

  return 0;
}
