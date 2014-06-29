/*
  #
  #  File        : use_chlpca.cpp
  #                ( C++ source file )
  #
  #  Description : Example of use for the CImg plugin 'plugins/chlpca.h'.
  #                This file is a part of the CImg Library project.
  #                ( http://cimg.sourceforge.net )
  #
  #  Copyright  : Jerome Boulanger
  #               ( http://www.irisa.fr/vista/Equipe/People/Jerome.Boulanger.html )
  #
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

#define cimg_plugin "plugins/chlpca.h"
#include "CImg.h"
using namespace cimg_library;
#ifndef cimg_imagepath
#define cimg_imagepath "img/"
#endif

// Main procedure
//----------------
int main(int argc,char **argv) {
  cimg_usage("Patch based denoising ");
  const char *file_i  = cimg_option("-i",cimg_imagepath "milla.bmp","Input image");
  const int p = cimg_option("-p",3,"patch radius");
  const int w = cimg_option("-w",10,"window radius");
  const float lambda_min = cimg_option("-l",(float)2.f,"component selection threshold");
  const int nstep = cimg_option("-nstep",5,"sub-sampling");
  const float nsim = cimg_option("-nsim",(float)5.f,"dictionnary size a multiple of the patch size");
  const float noise_std = cimg_option("-sigma",(float)-1.f,"noise std (-1:estimated)");
  const bool use_svd = cimg_option("-svd",(float)-1.f,"use svd for computing PCA");
  const char *file_o  = cimg_option("-o",(char*)NULL,"Output file");
  CImg<> img(file_i);
  img = img.get_chlpca(p, w, nstep, nsim, lambda_min, noise_std, use_svd);
  img.display();
  if (file_o) img.save(file_o);
  return 0;
}
