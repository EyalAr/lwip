/*
 #
 #  File        : fade_images.cpp
 #                ( C++ source file )
 #
 #  Description : Compute a linear fading between two images.
 #                This file is a part of the CImg Library project.
 #                ( http://cimg.sourceforge.net )
 #
 #  Copyright   : David Tschumperle
 #                ( http://tschumperle.users.greyc.fr/ )
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
#ifndef cimg_imagepath
#define cimg_imagepath "img/"
#endif
#undef min
#undef max

// Main procedure
//---------------
int main(int argc,char **argv) {

  // Read and check command line parameters.
  cimg_usage("Compute a linear fading between two 2D images");
  const char *file_i1 = cimg_option("-i1",cimg_imagepath "sh0r.pgm","Input Image 1");
  const char *file_i2 = cimg_option("-i2",cimg_imagepath "milla.bmp","Input Image 2");
  const char *file_o  = cimg_option("-o",(char*)0,"Output Image");
  const bool visu     = cimg_option("-visu",true,"Visualization mode");
  const double pmin   = cimg_option("-min",40.0,"Begin of the fade (in %)")/100.0;
  const double pmax   = cimg_option("-max",60.0,"End of the fade (in %)")/100.0;
  const double angle  = cimg_option("-angle",0.0,"Fade angle")*cil::cimg::PI/180;

  // Init images.
  cil::CImg<unsigned char> img1(file_i1), img2(file_i2);
  if (!img2.is_sameXYZC(img1)) {
    int
      dx = cil::cimg::max(img1.width(),img2.width()),
      dy = cil::cimg::max(img1.height(),img2.height()),
      dz = cil::cimg::max(img1.depth(),img2.depth()),
      dv = cil::cimg::max(img1.spectrum(),img2.spectrum());
    img1.resize(dx,dy,dz,dv,3);
    img2.resize(dx,dy,dz,dv,3);
  }
  cil::CImg<unsigned char> dest(img1);

  // Compute the faded image.
  const double ca = std::cos(angle), sa = std::sin(angle);
  double alpha;
  cimg_forXYZC(dest,x,y,z,k) {
    const double X = ((double)x/img1.width() - 0.5)*ca + ((double)y/img1.height() - 0.5)*sa;
    if (X+0.5<pmin) alpha = 0; else {
      if (X+0.5>pmax) alpha = 1; else
        alpha = (X+0.5-pmin)/(pmax-pmin);
    }
    dest(x,y,z,k) = (unsigned char)((1 - alpha)*img1(x,y,z,k) + alpha*img2(x,y,z,k));
  }

  // Save and exit
  if (file_o) dest.save(file_o);
  if (visu) dest.display("Image fading");
  return 0;
}
