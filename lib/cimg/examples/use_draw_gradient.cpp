/*
 #
 #  File        : use_draw_gradient.cpp
 #                ( C++ source file )
 #
 #  Description : Example of use for the CImg plugin 'plugins/draw_gradient.h'.
 #                This file is a part of the CImg Library project.
 #                ( http://cimg.sourceforge.net )
 #
 #  Copyright  : Jerome Boulanger
 #                ( http://www.ricam.oeaw.ac.at/people/page.cgi?firstn=Jerome;lastn=Boulanger )
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

#define cimg_plugin "plugins/draw_gradient.h"
#include "CImg.h"
using namespace cimg_library;

// Main procedure
//---------------
int main(int argc,char **argv) {

  // Read command line arguments
  //----------------------------
  cimg_usage("Example of the use of draw_gradient CImg plugin");
  const char *const file_i  = cimg_option("-i",(char*)0,"Input image");
  const int shape  = cimg_option("-s",1,"shape [0,6]");
  const int profile  = cimg_option("-p",0,"profile [0,7]");

  // Define an image
  CImg<unsigned char> img;
  if (file_i) img.load(file_i).resize(-100,-100,-100,3);
  else img.assign(300,200,1,3,0);

  // Define the color of the gradient
  CImg<unsigned char> col(3);
  const unsigned char col1[3] = { 0,0,255 }, col2[3] = { 255,255,255 };
  CImgDisplay disp(img,"Click and drag to create color gradient",0);
  while (!disp.is_closed() && !disp.key()) {

    // Get a vector direction from the user.
    const CImg<int> selection = img.get_select(disp,1);

    // Draw a gradient using the selected coordinated.
    col.rand(100,255);
    printf("Gradient with %s from color (%d,%d,%d) to (%d,%d,%d)\n",
           CImg<>::get_gradient_str(shape,profile),col(0),col(1),col(2),col1[0],col1[1],col2[2]);
    img.draw_gradient(selection(0),selection(1),selection(3),selection(4),
                      col.data(),col1,shape,profile,.7f).display(disp);
  }

  // color 0 to transparency
  if (file_i) img.load(file_i).resize(-100,-100,-100,3);
  else img.assign(300,200,1,3,0);
  img.display(disp);
  disp.show().flush();
  while (!disp.is_closed() && !disp.key()) {

    // Get a vector direction from the user.
    const CImg<int> selection = img.get_select(disp,1);

    // Draw a gradient using the selected coordinated.
    col.rand(100,255);
    printf("Gradient with %s from color (%d,%d,%d) to transparency\n",
           CImg<>::get_gradient_str(shape,profile),col(0),col(1),col(2));
    img.draw_gradient(selection(0),selection(1),selection(3),selection(4),
                      col.data(),0,shape,profile,.7f).display(disp);
  }


  // transparency to color 1
  if (file_i) img.load(file_i).resize(-100,-100,-100,3);
  else img.assign(300,200,1,3,0);
  img.display(disp);
  disp.show().flush();
  while (!disp.is_closed() && !disp.key()) {

    // Get a vector direction from the user.
    const CImg<int> selection = img.get_select(disp,1);

    // Draw a gradient using the selected coordinated.
    col.rand(100,255);
    printf("Gradient with %s from transparency to color (%d,%d,%d)\n",
           CImg<>::get_gradient_str(shape,profile),col(0),col(1),col(2));
    img.draw_gradient(selection(0),selection(1),selection(3),selection(4),
                      0,col.data(),shape,profile,.7f).display(disp);
  }

  // random
  if (file_i) img.load(file_i).resize(-100,-100,-100,3);
  else img.assign(300,200,1,3,0);
  disp.set_title("Random color gradient").show().flush();
  CImg<unsigned char> visu(img);
  visu.display(disp);
  while (!disp.is_closed() && !disp.key()) {
    const int
      x = (int)(cimg::rand()*visu.width()),
      y = (int)(cimg::rand()*visu.height()),
      rx = (int)((cimg::rand()*25+5)*(cimg::rand()>.5?-1:1)),
      ry = (int)((cimg::rand()*25+5)*(cimg::rand()>.5?-1:1));
    col.rand(64,255);
    img.draw_gradient(x,y,x+rx,y+ry,col.data(),0,shape,profile,.4f);
    visu = img;
    visu.draw_text(10,10,"%.1ffps",col2,0,1,13,disp.frames_per_second()).display(disp);
    if (disp.is_resized()) disp.resize();
  }

  return 0;
}
