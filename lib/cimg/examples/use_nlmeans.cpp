/*
 #
 #  File        : use_nlmeans.cpp
 #                ( C++ source file )
 #
 #  Description : Example of use for the CImg plugin 'plugins/nlmeans.h'.
 #                This file is a part of the CImg Library project.
 #                ( http://cimg.sourceforge.net )
 #
 #  Copyright  : Jerome Boulanger
 #               ( http://www.irisa.fr/vista/Equipe/People/Jerome.Boulanger.html )
 #
 #  Benchmark   : (CPU intel pentium 4 2.60GHz) compiled with cimg_debug=0.
 #                patch lambda* alpha   T    sigma  PSNR
 #                3x3    15      9x9    3.6s   20   28.22
 #                5x5    17     15x15  22.2s   20   27.91
 #                7x7    42     21x21  80.0s   20   28.68
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

#define cimg_plugin "plugins/nlmeans.h"
#include "CImg.h"
using namespace cimg_library;
#ifndef cimg_imagepath
#define cimg_imagepath "img/"
#endif

// Main procedure
//----------------
int main(int argc,char **argv) {

  // Read command line argument s
  //-----------------------------
  cimg_usage("Non-local means denoising algorithm.\n [1] Buades, A. Coll, B. and Morel, J.: A review of image "
             "denoising algorithms, with a new one. Multiscale Modeling and Simulation: A SIAM Interdisciplinary "
             "Journal 4 (2004) 490-530  \n [2] Gasser, T. Sroka,L. Jennen Steinmetz,C. Residual variance and residual "
             "pattern nonlinear regression. Biometrika 73 (1986) 625-659 \n Build : ");

  // input/output and general options
  const char *file_i  = cimg_option("-i",cimg_imagepath "milla.bmp","Input image");
  const char *file_o  = cimg_option("-o",(char*)NULL,"Output file");
  const double zoom   = cimg_option("-zoom",1.0,"Image magnification");
  const double noiseg = cimg_option("-ng",0.0,"Add gauss noise before aplying the algorithm");
  const double noiseu = cimg_option("-nu",0.0,"Add uniform noise before applying the algorithm");
  const double noises = cimg_option("-ns",0.0,"Add salt&pepper noise before applying the algorithm");
  const unsigned int visu = cimg_option("-visu",1,"Visualization step (0=no visualization)");

  // non local means options
  const int patch_size = cimg_option("-p",1,"Half size of the patch (2p+1)x(2p+1)");
  const float lambda = (float)cimg_option("-lambda",-1.0f,"Bandwidth as defined in [1] (-1 : automatic bandwidth)");
  const double sigma = cimg_option("-sigma",-1,"Noise standard deviation (-1 : robust estimation)");
  const int alpha = cimg_option("-alpha",3,"Neighborhood size (3)");
  const int sampling = cimg_option("-sampling",1,"Sampling of the patch (1: slow, 2: fast)");

  // Read image
  //------------
  CImg<> img;
  if (file_i) {
    img = CImg<>(file_i);
    if (zoom>1)
      img.resize((int)(img.width()*zoom),(int)(img.height()*zoom),(int)(img.depth()*zoom),-100,3);
  } else throw CImgException("You need to specify at least one input image (option -i)");
  CImg<> original=img;

  // Add some noise
  //-----------------
  img.noise(noiseg,0).noise(noiseu,1).noise(noises,2);

  // Apply the filter
  //---------------------
  long tic = cimg::time();
  CImg<> dest;
  dest = img.get_nlmeans(patch_size,lambda,alpha,sigma,sampling);
  long tac = cimg::time();

  // Save result
  //-----------------
  if (file_o) dest.cut(0,255.f).save(file_o);

  // Display (option -visu)
  //-------------------
  if (visu){
    fprintf(stderr,"Image computed in %f s \n",(float)(tac-tic)/1000.);
    fprintf(stderr,"The pnsr is %f \n",20.*std::log10(255./std::sqrt( (dest-original).pow(2).sum()/original.size() )));
    if (noiseg==0 && noiseu==0 && noises==0)
      CImgList<>(original,dest,((dest-original)*=2)+=128).display("Original + Restored + Estimated Noise");

    else {
      CImgList<>(original,img,dest,((dest-img)*=2)+=128,((dest-original)*=2)+=128).display("Original + Noisy + Restored + Estimated Noise + Original Noise");
    }
  }

  return 0;
}


