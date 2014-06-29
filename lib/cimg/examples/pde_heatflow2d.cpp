/*
 #
 #  File        : pde_heatflow2D.cpp
 #                ( C++ source file )
 #
 #  Description : A simple Heat flow on 2D images.
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

// Include library header file
#include "CImg.h"
#ifndef cimg_imagepath
#define cimg_imagepath "img/"
#endif
#undef min
#undef max

// Make a simpler namespace alias if one wants to avoid 'using namespace cimg_library'
namespace cil = cimg_library;

// Main procedure
//----------------
int main(int argc,char **argv) {

  // Read command line arguments, and init images and displays
  //-----------------------------------------------------------
  cimg_usage("Perform a simple Heat Flow on 2D images");
  cil::CImg<> img(cimg_option("-i",cimg_imagepath "milla.bmp","Input image")), veloc(img);
  const double dt = cimg_option("-dt",3.0,"Adapting time step");
  img.
    noise(cimg_option("-ng",0.0,"Add gaussian noise"),0).
    noise(cimg_option("-nu",0.0,"Add uniform noise"),1).
    noise(cimg_option("-ns",0.0,"Add Salt&Pepper noise"),2);
  cil::CImgDisplay profile(400,300,"Intensity Profile",0,false,true), disp(img,"Heat flow 2D",0,false,true);
  disp.move((cil::CImgDisplay::screen_width() - disp.width() - profile.width())/2,
            (cil::CImgDisplay::screen_height() - disp.height())/2);

  profile.move(disp.window_x() + 8 + disp.window_width(), disp.window_y());
  const float white[] = { 255,255,255 };
  bool run_PDE = true;

  // Begin PDE iteration loop
  //-------------------------
  for (int iter = 0; !disp.is_closed() && !profile.is_closed() &&
         !disp.is_keyQ() && !disp.is_keyESC() && !profile.is_keyQ() && !profile.is_keyESC();) {

    // Compute one iteration of PDE explicit scheme
    if (run_PDE) {
      CImg_3x3(I,float);
      cimg_forC(img,k) cimg_for3x3(img,x,y,0,k,I,float) veloc(x,y,k) = Inc + Ipc + Icn + Icp - 4*Icc;
      float m, M = veloc.max_min(m);
      const double xdt = dt/(M - m);
      img += veloc*xdt;
      cil::CImg<>(img).draw_text(2,2,"iter = %d",white,0,1,13,iter).display(disp.wait(25));
    }

    // Plot (R,G,B) intensity profiles and display it
    if (disp.mouse_x()>=0) {
      const int
        mx = disp.mouse_x(), my = disp.mouse_y(),
        mnx = mx*profile.width()/disp.width();
      const unsigned char red[] = { 255,0,0 }, green[] = { 0,255,0 }, blue[] = { 0,0,255 }, white[] = { 255,255,255 };
      cil::CImg<unsigned char>(profile.width(),profile.height(),1,3,0).
        draw_graph(img.get_shared_row(my,0,0),red,1,1,0,255,0).
        draw_graph(img.get_shared_row(my,0,1),green,1,1,0,255,0).
        draw_graph(img.get_shared_row(my,0,2),blue,1,1,0,255,0).
        draw_line(mnx,0,mnx,profile.height()-1,white,0.5f,cil::cimg::rol(0xFF00FF00,iter%32)).
        draw_text(2,2,"(x,y)=(%d,%d)",white,0,1,13,mx,my).
        display(profile);
    }

    // Mouse button stops/starts PDE evolution.
    if (disp.button() || profile.button()) { disp.set_button(); profile.set_button(); run_PDE = !run_PDE; }
    profile.resize();
    disp.resize(disp);
    if (run_PDE) ++iter;
  }

  return 0;
}
