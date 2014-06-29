/*
 #
 #  File        : edge_explorer2d.cpp
 #                ( C++ source file )
 #
 #  Description : Real time edge detection while moving a ROI
 #                (rectangle of interest) over the original image.
 #                This file is a part of the CImg Library project.
 #                ( http://cimg.sourceforge.net )
 #
 #  Copyright   : Orges Leka
 #                ( oleka(at)students.uni-mainz.de )
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

// Main procedure
//----------------
int main(int argc, char** argv) {

  // Usage of the program displayed at the command line
  cimg_usage("Real time edge detection with CImg. (c) Orges Leka");

  // Read command line arguments
  // With cimg_option we can get a new name for the image which is to be loaded from the command line.
  const char* img_name = cimg_option("-i", cimg_imagepath "lena.pgm","Input image.");
  double
    alpha = cimg_option("-a",1.0,"Blurring the gradient image."),
    thresL = cimg_option("-tl",13.5,"Lower thresholding used in Hysteresis."),
    thresH = cimg_option("-th",13.6,"Higher thresholding used in Hysteresis.");
  const unsigned int
    mode = cimg_option("-m",1,"Detection mode: 1 = Hysteresis, 2 = Gradient angle."),
    factor = cimg_option("-s",80,"Half-size of edge-explorer window.");

  cimg_help("\nAdditional notes : user can press following keys on main display window :\n"
            "     - Left arrow : Decrease alpha.\n"
            "     - Right arrow : Increase alpha.\n");

  // Construct a new image called 'edge' of size (2*factor,2*factor)
  // and of type 'unsigned char'.
  CImg<unsigned char> edge(2*factor,2*factor);
  CImgDisplay disp_edge(512,512,"Edge Explorer");

  // Load the image with the name 'img_name' into the CImg 'img'.
  // and create a display window 'disp' for the image 'img'.
  const CImg<unsigned char> img(img_name);
  CImgDisplay disp(img,"Original Image");

  // Begin main interaction loop.
  int x = 0, y = 0;
  bool redraw = false;
  while (!disp.is_closed() && !disp.is_keyQ() && !disp.is_keyESC()) {
    disp.wait(100);
    if (disp.button()&1) { alpha+=0.05; redraw = true; }
    if (disp.button()&2) { alpha-=0.05; redraw = true; }
    if (disp.wheel()) { alpha+=0.05*disp.wheel(); disp.set_wheel(); redraw = true; }
    if (alpha<0) alpha = 0;
    if (disp_edge.is_resized()) { disp_edge.resize(); redraw = true; }
    if (disp_edge.is_closed()) disp_edge.show();
    if (disp.is_resized()) disp.resize(disp);
    if (disp.mouse_x()>=0) {
      x = disp.mouse_x(); // Getting the current position of the mouse.
      y = disp.mouse_y(); //
      redraw = true;    // The image should be redrawn.
    }
    if (redraw) {
      disp_edge.set_title("Edge explorer (alpha=%g)",alpha);
      const int
        x0 = x-factor, y0 = y-factor,  // These are the coordinates for the red rectangle
        x1 = x+factor, y1 = y+factor;  // to be drawn on the original image.
      const unsigned char
        red[3] = { 255,0,0 },          //
        black[3] = { 0,0,0 };          // Defining the colors we need for drawing.

        (+img).draw_rectangle(x0,y0,x1,y1,red,1.0f,0x55555555U).display(disp);
        //^ We draw the red rectangle on the original window using 'draw_line'. Then we display the result via '.display(disp)' .
        //  Observe, that the color 'red' has to be of type 'const unsigned char',
        //  since the image 'img' is of type 'const CImg<unsigned char>'.

        //'normalize' is used to get a greyscaled image.
        CImg<> visu_bw = CImg<>(img).get_crop(x0,y0,x1,y1).get_norm().normalize(0,255).resize(-100,-100,1,2,2);
        // get_crop(x0,y0,x1,y1) gets the rectangle we are interested in.

        edge.fill(255); // Background color in the edge-detection window is white.

        // grad[0] is the gradient image of 'visu_bw' in x-direction.
        // grad[1] is the gradient image of 'visu_bw' in y-direction.
        CImgList<> grad(visu_bw.blur((float)alpha).normalize(0,255).get_gradient());

        // To avoid unnecessary calculations in the image loops:
        const double
          pi = cimg::PI,
          p8 = pi/8.0, p38 = 3.0*p8,
          p58 = 5.0*p8, p78 = 7.0*p8;

        cimg_forXY(visu_bw,s,t) {
          // We take s,t instead of x,y, since x,y are already used.
          // s corresponds to the x-ordinate of the pixel while t corresponds to the y-ordinate.
          if ( 1 <= s && s <= visu_bw.width()-1 && 1 <= t && t <=visu_bw.height()-1) { // if - good points
            double
              Gs = grad[0](s,t),                  //
              Gt = grad[1](s,t),                               //  The actual pixel is (s,t)
              Gst = cimg::abs(Gs) + cimg::abs(Gt),    //
              // ^-- For efficient computation we observe that |Gs|+ |Gt| ~=~ sqrt( Gs^2 + Gt^2)
              Gr, Gur, Gu, Gul, Gl, Gdl, Gd, Gdr;
            // ^-- right, up right, up, up left, left, down left, down, down right.
            double theta = std::atan2(Gt,Gs)+pi; // theta is from the interval [0,Pi]
            switch(mode) {
            case 1: // Hysterese is applied
              if (Gst>=thresH) { edge.draw_point(s,t,black); }
              else if (thresL <= Gst && Gst < thresH) {
                // Neighbourhood of the actual pixel:
                Gr = cimg::abs(grad[0](s+1,t)) + cimg::abs(grad[1](s+1,t)); // right
                Gl = cimg::abs(grad[0](s-1,t)) + cimg::abs(grad[1](s-1,t)); // left
                Gur = cimg::abs(grad[0](s+1,t+1)) + cimg::abs(grad[1](s+1,t+1)); // up right
                Gdl = cimg::abs(grad[0](s-1,t-1)) + cimg::abs(grad[1](s-1,t-1)); // down left
                Gu = cimg::abs(grad[0](s,t+1)) + cimg::abs(grad[1](s,t+1)); // up
                Gd = cimg::abs(grad[0](s,t-1)) + cimg::abs(grad[1](s,t-1)); // down
                Gul = cimg::abs(grad[0](s-1,t+1)) + cimg::abs(grad[1](s-1,t+1)); // up left
                Gdr = cimg::abs(grad[0](s+1,t-1)) + cimg::abs(grad[1](s+1,t-1)); // down right
                if (Gr>=thresH || Gur>=thresH || Gu>=thresH || Gul>=thresH
                    || Gl>=thresH || Gdl >=thresH || Gu >=thresH || Gdr >=thresH) {
                  edge.draw_point(s,t,black);
                }
              };
              break;
            case 2: // Angle 'theta' of the gradient (Gs,Gt) at the point (s,t).
              if(theta >= pi)theta-=pi;
              //rounding theta:
              if ((p8 < theta && theta <= p38 ) || (p78 < theta && theta <= pi)) {
                // See (*) below for explanation of the vocabulary used.
                // Direction-pixel is (s+1,t) with corresponding gradient value Gr.
                Gr = cimg::abs(grad[0](s+1,t)) + cimg::abs(grad[1](s+1,t)); // right
                // Contra-direction-pixel is (s-1,t) with corresponding gradient value Gl.
                Gl = cimg::abs(grad[0](s-1,t)) + cimg::abs(grad[1](s-1,t)); // left
                if (Gr < Gst && Gl < Gst) {
                  edge.draw_point(s,t,black);
                }
              }
              else if ( p8 < theta && theta <= p38) {
                // Direction-pixel is (s+1,t+1) with corresponding gradient value Gur.
                Gur = cimg::abs(grad[0](s+1,t+1)) + cimg::abs(grad[1](s+1,t+1)); // up right
                // Contra-direction-pixel is (s-1,t-1) with corresponding gradient value Gdl.
                Gdl = cimg::abs(grad[0](s-1,t-1)) + cimg::abs(grad[1](s-1,t-1)); // down left
                if (Gur < Gst && Gdl < Gst) {
                  edge.draw_point(s,t,black);
                      }
              }
              else if ( p38 < theta && theta <= p58) {
                // Direction-pixel is (s,t+1) with corresponding gradient value Gu.
                Gu = cimg::abs(grad[0](s,t+1)) + cimg::abs(grad[1](s,t+1)); // up
                // Contra-direction-pixel is (s,t-1) with corresponding gradient value Gd.
                Gd = cimg::abs(grad[0](s,t-1)) + cimg::abs(grad[1](s,t-1)); // down
                if (Gu < Gst && Gd < Gst) {
                  edge.draw_point(s,t,black);
                }
              }
              else if (p58 < theta && theta <= p78) {
                // Direction-pixel is (s-1,t+1) with corresponding gradient value Gul.
                Gul = cimg::abs(grad[0](s-1,t+1)) + cimg::abs(grad[1](s-1,t+1)); // up left
                // Contra-direction-pixel is (s+1,t-1) with corresponding gradient value Gdr.
                Gdr = cimg::abs(grad[0](s+1,t-1)) + cimg::abs(grad[1](s+1,t-1)); // down right
                if (Gul < Gst && Gdr < Gst) {
                  edge.draw_point(s,t,black);
                }
              };
              break;
            } // switch
          } // if good-points
        }  // cimg_forXY */
        edge.display(disp_edge);
    }// if redraw
  } // while
  return 0;
}

// (*) Comments to the vocabulary used:
// If (s,t) is the current pixel, and G=(Gs,Gt) is the gradient at (s,t),
// then the _direction_pixel_ of (s,t) shall be the one of the eight neighbour pixels
// of (s,t) in whose direction the gradient G shows.
// The _contra_direction_pixel is the pixel in the opposite direction in which the gradient G shows.
// The _corresponding_gradient_value_ of the pixel (x,y) with gradient G = (Gx,Gy)
// shall be |Gx|+|Gy| ~=~ sqrt(Gx^2+Gy^2).
