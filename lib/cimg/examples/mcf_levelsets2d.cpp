/*
 #
 #  File        : mcf_levelsets2d.cpp
 #                ( C++ source file )
 #
 #  Description : Implementation of the Mean Curvature Flow on a 2D curve,
 #                using the framework of Level Sets.
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
using namespace cimg_library;
#undef min
#undef max

// Retrieve the curve corresponding to the zero level set of the distance function.
template<typename T>
CImg<unsigned char> get_level0(const CImg<T>& img) {
  CImg<unsigned char> dest(img);
  CImg_2x2(I,T); Inn = 0;
  cimg_for2x2(img,x,y,0,0,I,T) if (Icc*Inc<0 || Icc*Icn<0) dest(x,y) = 255; else dest(x,y) = Icc<0?100:0;
  return dest;
}

/*--------------------

   Main procedure

----------------------*/
int main(int argc,char **argv) {
  cimg_usage("Perform a Mean Curvature Flow on closed curves, using Level Sets");
  const float dt = cimg_option("-dt",0.8f,"PDE time step");
  const unsigned int nb_iterations = cimg_option("-iter",10000,"Number of iterations");

  // Create a user-defined closed curve.
  CImg<unsigned char> curve(256,256,1,2,0);
  unsigned char col1[] = {0,255}, col2[] = {200,255}, col3[] = {255,255};
  curve.draw_grid(20,20,0,0,false,false,col1,0.4f,0xCCCCCCCC,0xCCCCCCCC).
    draw_text(5,5,"Please draw your curve\nin this window\n(Use your mouse)",col1);
  CImgDisplay disp(curve,"Mean curvature flow",0);
  int xo = -1, yo = -1, x0 = -1, y0 = -1, x1 = -1, y1 = -1;
  while (!disp.is_closed() && (x0<0 || disp.button())) {
    if (disp.button() && disp.mouse_x()>=0 && disp.mouse_y()>=0) {
      if (x0<0) { xo = x0 = disp.mouse_x(); yo = y0 = disp.mouse_y(); } else {
        x1 = disp.mouse_x(); y1 = disp.mouse_y();
        curve.draw_line(x0,y0,x1,y1,col2).display(disp);
        x0 = x1; y0 = y1;
      }
    }
    disp.wait();
    if (disp.is_resized()) disp.resize(disp);
  }
  curve.draw_line(x1,y1,xo,yo,col2).channel(0).draw_fill(0,0,col3);
  CImg<> img = CImg<>(curve.get_shared_channel(0)).normalize(-1,1);

  // Perform the "Mean Curvature Flow".
  img.distance_eikonal(10);
  CImg_3x3(I,float);
  for (unsigned int iteration = 0; iteration<nb_iterations && !disp.is_closed() &&
         !disp.is_keyQ() && !disp.is_keyESC(); ++iteration) {
    CImg<float> velocity(img.width(),img.height(),img.depth(),img.spectrum());
    float *ptrd = velocity.data(), veloc_max = 0;
    cimg_for3x3(img,x,y,0,0,I,float) {
      const float
        ix = (Inc - Ipc)/2,
        iy = (Icn - Icp)/2,
        ixx = Inc + Ipc - 2*Icc,
        iyy = Icn + Icp - 2*Icc,
        ixy = (Ipp + Inn - Inp - Ipn)/4,
        ngrad = ix*ix + iy*iy,
        iee = (ngrad>1e-5)?((iy*iy*ixx - 2*ix*iy*ixy + ix*ix*iyy)/ngrad):0;
      *(ptrd++) = iee;
      if (iee>veloc_max) veloc_max = iee; else if (-iee>veloc_max) veloc_max = -iee;
    }
    if (veloc_max>0) img+=(velocity*=dt/veloc_max);
    if (!(iteration%10)) {
      get_level0(img).resize(disp.width(),disp.height()).draw_grid(20,20,0,0,false,false,col3,0.4f,0xCCCCCCCC,0xCCCCCCCC).
        draw_text(5,5,"Iteration %d",col3,0,1,13,iteration).display(disp);
    }
    if (!(iteration%60)) img.distance_eikonal(1,3);
    if (disp.is_resized()) disp.resize();
  }

  return 0;
}
