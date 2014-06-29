/*
 #
 #  File        : wavelet_atrous.cpp
 #                ( C++ source file )
 #
 #  Description : Performs a 2D or 3D 'a trous' wavelet transform
 #                (using a cubic spline) on an image or a video sequence.
 #                This file is a part of the CImg Library project.
 #                ( http://cimg.sourceforge.net )
 #
 #  Author      : Renaud Peteri
 #                ( Renaud.Peteri(at)mines-paris.org )
 #                Andrea Onofri
 #                ( Andrea.Onofri(at)teletu.it )
 #
 #  Institution : CWI, Amsterdam
 #
 #  Date        : February 2005
 #
 #  References  : Starck, J.-L., Murtagh, F. and Bijaoui, A.,
 #                Image Processing and Data Analysis: The Multiscale Approach,
 #                Cambridge University Press, 1998.
 #                (Hardback and softback, ISBN 0-521-59084-1 and 0-521-59914-8.)
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

#include "CImg.h"
using namespace cimg_library;
#ifndef cimg_imagepath
#define cimg_imagepath "img/"
#endif

// Define convolution mask.
CImg<float> mask(const unsigned char dirIdx, const unsigned char scale) {
  int d1 = 1 << (scale-1);
  int d2 = 1 << scale;
  int c = d2;
  int vecLen = (1 << (scale + 1)) + 1;

  float valC  = 0.375f;  // 6/16
  float valD1 = 0.25f;   // 4/16
  float valD2 = 0.0625f; // 1/16

  switch(dirIdx){
  case 0: //x
    {
      CImg<float> m(vecLen,1,1);m.fill(0);
      m(c) = valC;
      m(c-d1) =  m(c+d1) = valD1;
      m(c-d2) =  m(c+d2) = valD2;
      return m;
    }
  case 1: //y
    {
      CImg<float> m(1,vecLen,1);m.fill(0);
      m(0,c) = valC;
      m(0,c-d1) =  m(0,c+d1) = valD1;
      m(0,c-d2) =  m(0,c+d2) = valD2;
      return m;
    }
  case 2: //t
    {
      CImg<float> m(1,1,vecLen);m.fill(0);
      m(0,0,c) = valC;
      m(0,0,c-d1) =  m(0,0,c+d1) = valD1;
      m(0,0,c-d2) =  m(0,0,c+d2) = valD2;
      return m;
    }
  default: throw CImgException("Error, unknow decompostion axe, dirIdx = '%c'.",dirIdx);
  }
}

/*------------------
  Main procedure
  ----------------*/
int main(int argc,char **argv) {

  cimg_usage("Perform an 'a trous' wavelet transform (using a cubic spline) on an image or on a video sequence.\n"
             "This wavelet transform is undecimated and produces 2 images/videos at each scale. For an example of\n"
             "decomposition on a video, try -i img/trees.inr (sequence from the MIT).\n"
             "\t(Type -h for help)");

  // Read command line parameters
  const char
    *name_i  = cimg_option("-i",cimg_imagepath "lena.pgm","Input image or video"),
    *name_o  = cimg_option("-o","","Name of the multiscale analysis output"),
    *axe_dec = cimg_option("-axe",(char*)NULL,"Perform the multiscale decomposition in just one direction ('x', 'y' or 't')");
  const unsigned int
    s = cimg_option("-s",3,"Scale of decomposition");

  const bool help = cimg_option("-h",false,"Display Help");
  if(help) exit(0);

  // Initialize Image Data
  std::fprintf(stderr," - Load image sequence '%s'...\n",cimg::basename(name_i));
  const CImg<float> texture_in(name_i);
  CImg<float> mask_conv;
  CImgList<float> res(s,texture_in.width(),texture_in.height(),texture_in.depth());
  CImgList<float> wav(s,texture_in.width(),texture_in.height(),texture_in.depth());
  cimglist_for(res,l) { res(l).fill(0.0); wav(l).fill(0.0);}
  unsigned int i;

  int firstDirIdx = 0;
  int lastDirIdx = 2;
  if (axe_dec){// The multiscale decomposition will be performed in just one direction
    char c = cimg::uncase(axe_dec[0]);
    switch(c) {
    case 'x': {firstDirIdx = 0; break;}
    case 'y': {firstDirIdx = 1; break;}
    case 't': {firstDirIdx = 2; break;}
    default: throw CImgException("Error, unknow decompostion axe '%c', try 'x', 'y' or 't'",c);
    }
    lastDirIdx = firstDirIdx;//only one direction
  }

  for(i=0;i<s;i++){
    std::fprintf(stderr," - Performing scale %u ...\n",i+1);
    if(i==0){ res(i) =  texture_in;} else {  res(i) = res(i-1);}
    for(int di=firstDirIdx;di<=lastDirIdx;di++){
      mask_conv = mask(di, i+1);
      res(i) = res(i).get_convolve(mask_conv);
    }
    if(i==0){wav(i) = texture_in - res(i);}  // res(0) and wav(0) are the 1st scale of decompostion
    else {wav(i) = res(i-1) - res(i);}
  }

  if (*name_o){
    // Save the Multi-Scale Analysis
    std::fprintf(stderr," - Saving of all output sequences : %s in the msa/ directory... \n",cimg::basename(name_o));
    int count = 1; // res0 = original image
    char filename[256] = "", filename_wav[256] = "";
    char STmp[3] = "";
    const int err = std::system("mkdir msa");
    if (!err) for(i=0;i<s;i++) {
      std::strcpy( filename, "msa/res" );
      std::strcpy( filename_wav, "msa/wav" );
      if( count < 10 )
        { std::strcat( filename, "0" );std::strcat( filename_wav, "0" );}
      std::sprintf( STmp, "%d_", count );
      std::strcat( filename, STmp ); std::strcat( filename_wav, STmp );
      std::strcat( filename,name_o);std::strcat( filename_wav,name_o);
      res(i).save(filename);
      wav(i).save(filename_wav);
      count++;
    }
  }

  // Result visualization
  const float value = 255;
  for(i=0;i<s;i++) {
    res[i].normalize(0,255).draw_text(2,2,"Scale %d",&value,0,1,13,i);
    wav[i].normalize(0,255).draw_text(2,2,"Scale %d",&value,0,1,13,i);
  }

  CImgDisplay disp(res,"Approximations levels by increasing scale",0);
  CImgDisplay disp2(wav,"Wavelet coefficients by increasing scale",0);
  while (!disp.is_closed() && !disp.is_keyQ() && !disp.is_keyESC() &&
         !disp2.is_closed() && !disp2.is_keyQ() && !disp2.is_keyESC()) {
    if (disp.is_resized()) disp.resize().display(res);
    if (disp2.is_resized()) disp2.resize().display(wav);
    CImgDisplay::wait(disp,disp2);
  }

  return 0;
}
