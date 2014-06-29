/*
 #
 #  File        : CImg_demo.cpp
 #                ( C++ source file )
 #
 #  Description : A multi-part demo demonstrating some of the CImg capabilities.
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
 #  As a counterpart to the access to the source code and rights to copy,
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

// Include static image data, so that the exe does not depend on external image files.
#include "img/CImg_demo.h"

// Include CImg library header.
#include "CImg.h"
using namespace cimg_library;
#undef min
#undef max

// Item : Blurring Gradient
//----------------------------
void* item_blurring_gradient() {
  const CImg<float> src(data_milla,211,242,1,3);
  CImgList<float> grad = src.get_gradient();
  CImgList<unsigned char> visu = (src,sqrt(grad[0].pow(2) + grad[1].pow(2)).normalize(0,255),src);
  CImgDisplay disp(visu,"[#1] - Color Image, Gradient Norm and Blurring Gradient",0);

  for (double sigma = 0; !disp.is_closed() && !disp.is_keyQ() && !disp.is_keyESC(); sigma+=0.05) {
    visu[2] = visu[1].get_blur((float)cimg::abs(30*std::cos(sigma))).normalize(0,255);
    disp.resize(false).display(visu).wait(20);
  }
  return 0;
}

// Item : Rotozoom
//-----------------
void* item_rotozoom() {
  CImg<unsigned char> src = CImg<unsigned char>(data_milla,211,242,1,3,false).resize(400,300,1,3,3), img(src);
  CImgDisplay disp(img.width(),img.height(),"[#2] - Rotozoom",0);
  float alpha = 0, t = 0, angle = 0, zoom0 = -0.9f;
  const unsigned char color[] = { 16,32,64 };

  while (!disp.is_closed() && !disp.is_keyQ() && !disp.is_keyESC()) {
    cimg_forYC(src,y,k) {
      const int xc = 4*src.width() + (int)(60*std::sin((float)y*3/src.height() + 10*t));
      cimg_forX(src,x) {
        const float val = (float)(src((xc+x)%src.width(),y,0,k)*
                                  (1.3f + 0.20*std::sin(alpha + k*k*((float)src.width()/2 - x)*
                                                        ((float)src.height()/2 - y)*std::cos(t)/300.0)));
        img(x,y,0,k) = (unsigned char)(val>255.0f?255:val);
      }
    }
    const float zoom = (float)(zoom0 + 0.3f*(1 + std::cos(3*t)));
    img.get_rotate(angle,0.5f*img.width(),0.5f*img.height(),1 + zoom,0,2).
      draw_text(3,3,"Mouse buttons\nto zoom in/out",color,0,0.8f,24).display(disp.resize(false).wait(20));
    alpha+=0.7f; t+=0.01f; angle+=0.8f;
    zoom0+=disp.button()&1?0.1f:disp.button()&2?-0.1f:0;
    if (disp.is_keyCTRLLEFT() && disp.is_keyF()) disp.resize(400,400,false).toggle_fullscreen(false);
  }
  return 0;
}

// Item : Anisotropic Smoothing (Total variation PDE, explicit scheme)
//--------------------------------------------------------------------
void* item_anisotropic_smoothing() {
  const CImg<float> src = CImg<>(data_milla,211,242,1,3).noise(-30,1);
  CImgList<float> images(src,src);
  CImgDisplay disp(images,"[#3] - Anisotropic smoothing");
  const float white[] = { 255, 255, 255 }, black[] = { 0, 0, 0 };

  for (unsigned int iter = 0; !disp.is_closed() && !disp.is_keyQ() && !disp.is_keyESC(); ++iter) {

    // Compute PDE velocity field.
    CImg_3x3(I,float);
    CImg<float> veloc(src);
    float *ptrd = veloc.data(), betamax = 0;
    cimg_forC(src,k) cimg_for3x3(images[1],x,y,0,k,I,float) {
      const float
        ix = (Inc - Ipc)/2,
        iy = (Icn - Icp)/2,
        ng = (float)std::sqrt(1e-10f + ix*ix + iy*iy),
        ixx = Inc + Ipc - 2*Icc,
        iyy = Icn + Icp - 2*Icc,
        ixy = 0.25f*(Inn + Ipp - Ipn - Inp),
        iee = (ix*ix*iyy + iy*iy*ixx - 2*ix*iy*ixy)/(ng*ng),
        beta = iee/(0.1f + ng);
      if (beta>betamax) betamax = beta; else if (-beta>betamax) betamax = -beta;
      *(ptrd++) = beta;
    }
    veloc*=40.0f/betamax;
    images[1]+=veloc;
    images[0].draw_text(4,4,"Iteration : %u ",white,black,1,13,iter);
    disp.resize(false).display(images);
  }
  return 0;
}

// Item : Fractal Animation
//--------------------------
void* item_fractal_animation() {
  CImg<unsigned char> img(400,400,1,3,0), noise(3,2,1,3);
  CImgDisplay disp(img,"[#4] - Fractal Animation");
  float zoom = 0;

  for (unsigned int iter = 0; !disp.is_closed() && !disp.is_keyQ() && !disp.is_keyESC(); ++iter, zoom+=0.2f) {
    img.draw_image((img.width() - noise.width())/2,
                   (img.height() - noise.height())/2,
                   noise.fill(0).noise(255,1)).
      rotate((float)(10*std::sin(iter/25.0)),0.5f*img.width(),0.5f*img.height(),(float)(1.04f + 0.02f*std::sin(zoom/10)),0,0).
      resize(disp.resize(false)).display(disp.wait(25));
    if (disp.is_keyCTRLLEFT() && disp.is_keyF()) disp.resize(400,400,false).toggle_fullscreen(false);
  }
  return 0;
}

// Item : Gamma Correction and Histogram Visualization
//-----------------------------------------------------
void* item_gamma_correction() {
  CImg<float> img = CImg<>(data_milla,211,242,1,3).normalize(0,1);
  CImgList<unsigned char> visu(img*255.0,CImg<unsigned char>(512,300,1,3,0));
  CImgDisplay disp(visu,"[#5] - Gamma Corrected Image and Histogram (Click to set Gamma)");
  const unsigned char
    yellow[] = { 255, 255, 0 }, blue[] = { 0, 155, 255 }, blue2[] = { 0, 0, 255 },
    blue3[] = { 0, 0, 155 }, white[] = { 255, 255, 255 }, green[] = { 50, 128, 50 };

  for (double gamma = 1; !disp.is_closed() && !disp.is_keyQ() && !disp.is_keyESC(); ) {
    cimg_forXYZC(visu[0],x,y,z,k) visu[0](x,y,z,k) = (unsigned char)(std::pow((double)img(x,y,z,k),1.0/gamma)*256);
    const CImg<float> hist = visu[0].get_histogram(50,0,255);
    visu[1].fill(0).draw_text(50,5,"Gamma = %.3g",white,0,1,24,gamma).
      draw_graph(hist,green,1,3,0,20000,0).draw_graph(hist,yellow,1,2,0,20000,0).
      draw_axes(0,256,20000,0,white,0.7f);
    const int xb = (int)(50 + gamma*150);
    visu[1].draw_grid(20,20,0,0,false,false,white,0.3f,0xCCCCCCCC,0xCCCCCCCC).
      draw_rectangle(51,31,xb-1,39,blue2).draw_rectangle(50,30,xb,30,blue).draw_rectangle(xb,30,xb,40,blue).
      draw_rectangle(xb,40,50,39,blue3).draw_rectangle(50,30,51,40,blue3);
    if (disp.button() && disp.mouse_x()>=img.width() + 50 && disp.mouse_x()<=img.width() + 450)
      gamma = (disp.mouse_x() - img.width() - 50)/150.0;
    disp.resize(disp,false).display(visu).wait();
  }
  return 0;
}

// Item : Filled Triangles
//-------------------------
void* item_filled_triangles() {

  // Create a colored 640x480 background image which consists of different color shades.
  CImg<float> background(640,480,1,3);
  cimg_forXY(background,x,y) background.fillC(x,y,0,
                                              x*std::cos(6.0*y/background.height()) + y*std::sin(9.0*x/background.width()),
                                              x*std::sin(8.0*y/background.height()) - y*std::cos(11.0*x/background.width()),
                                              x*std::cos(13.0*y/background.height()) - y*std::sin(8.0*x/background.width()));
  background.normalize(0,180);

  // Init images and create display window.
  CImg<unsigned char> img0(background), img;
  unsigned char white[] = { 255, 255, 255 }, color[100][3];
  CImgDisplay disp(img0,"[#6] - Filled Triangles (Click to shrink)");

  // Define random properties (pos, size, colors, ..) for all triangles that will be displayed.
  float posx[100], posy[100], rayon[100], angle[100], veloc[100], opacity[100];
  int num = 1;
  std::srand((unsigned int)time(0));
  for (int k = 0; k<100; ++k) {
    posx[k] = (float)(cimg::rand()*img0.width());
    posy[k] = (float)(cimg::rand()*img0.height());
    rayon[k] = (float)(10 + cimg::rand()*50);
    angle[k] = (float)(cimg::rand()*360);
    veloc[k] = (float)(cimg::rand()*20 - 10);
    color[k][0] = (unsigned char)(cimg::rand()*255);
    color[k][1] = (unsigned char)(cimg::rand()*255);
    color[k][2] = (unsigned char)(cimg::rand()*255);
    opacity[k] = (float)(0.3 + 1.5*cimg::rand());
  }

  // Start animation loop.
  while (!disp.is_closed() && !disp.is_keyQ() && !disp.is_keyESC()) {
    img = img0;

    // Draw each triangle on the background image.
    for (int k = 0; k<num; ++k) {
      const int
        x0 = (int)(posx[k] + rayon[k]*std::cos(angle[k]*cimg::PI/180)),
        y0 = (int)(posy[k] + rayon[k]*std::sin(angle[k]*cimg::PI/180)),
        x1 = (int)(posx[k] + rayon[k]*std::cos((angle[k] + 120)*cimg::PI/180)),
        y1 = (int)(posy[k] + rayon[k]*std::sin((angle[k] + 120)*cimg::PI/180)),
        x2 = (int)(posx[k] + rayon[k]*std::cos((angle[k] + 240)*cimg::PI/180)),
        y2 = (int)(posy[k] + rayon[k]*std::sin((angle[k] + 240)*cimg::PI/180));
      if (k%10) img.draw_triangle(x0,y0,x1,y1,x2,y2,color[k],opacity[k]);
      else img.draw_triangle(x0,y0,x1,y1,x2,y2,img0,0,0,img0.width()-1,0,0,img.height()-1,opacity[k]);
      img.draw_triangle(x0,y0,x1,y1,x2,y2,white,opacity[k],~0U);

      // Make the triangles rotate, and check for mouse click event.
      // (to make triangles collapse or join).
      angle[k]+=veloc[k];
      if (disp.mouse_x()>0 && disp.mouse_y()>0) {
        float u = disp.mouse_x() - posx[k], v = disp.mouse_y() - posy[k];
        if (disp.button()) { u = -u; v = -v; }
        posx[k]-=0.03f*u, posy[k]-=0.03f*v;
        if (posx[k]<0 || posx[k]>=img.width()) posx[k] = (float)(cimg::rand()*img.width());
        if (posy[k]<0 || posy[k]>=img.height()) posy[k] = (float)(cimg::rand()*img.height());
      }
    }

    // Display current animation framerate, and refresh display window.
    img.draw_text(5,5,"%u frames/s",white,0,0.5f,13,(unsigned int)disp.frames_per_second());
    img0.resize(disp.display(img).resize(false).wait(20));
    if (++num>100) num = 100;

    // Allow the user to toggle fullscreen mode, by pressing CTRL+F.
    if (disp.is_keyCTRLLEFT() && disp.is_keyF()) disp.resize(640,480,false).toggle_fullscreen(false);
  }
  return 0;
}

// Item : Mandelbrot/Julia Explorer
//----------------------------------
void* item_mandelbrot_explorer() {

  // Define image canvas and corresponding display window.
  CImg<unsigned char> img(800,600,1,3,0);
  CImgDisplay disp(img);

  // Start main explorer loop.
  double julia_r = 0, julia_i = 0;
  for (bool endflag = false, fractal_type = false, smooth = false, show_help = true; !endflag;) {
    bool stopflag = false;
    double xmin, xmax, ymin, ymax;

    // Init default upper-left/lower-right coordinates of the fractal set.
    if (fractal_type) { xmin = -1.5; xmax = 1.5; ymin = -1.5; ymax = 1.5; julia_r = 0.317; julia_i = 0.029; }
    else { xmin = -2.25; xmax = 1.0; ymin = -1.5; ymax = 1.5; julia_r = julia_i = 0; }

    // Create random palette for displaying the fractal set.
    const CImg<unsigned char> palette =
      CImg<unsigned char>(256,1,1,3,16+120).noise(119,1).resize(1024,1,1,3,3).fillC(0,0,0,0,0,0);

    // Enter event loop for the current fractal set.
    for (unsigned int maxiter = 64; !stopflag; ) {

      // Draw Mandelbrot or Julia fractal set on the image.
      img.resize(disp.resize().set_title("[#7] - %s Set : (%g,%g)-(%g,%g), %s = (%g,%g) (%u iter.)",
                                         fractal_type?"Julia":"Mandelbrot",xmin,ymin,xmax,ymax,
                                         fractal_type?"c":"z0",julia_r,julia_i,maxiter)).
        fill(0).draw_mandelbrot(palette,1,xmin,ymin,xmax,ymax,maxiter,smooth,fractal_type,julia_r,julia_i);

      // Display help if necessary.
      if (show_help) {
        const unsigned char white[] = { 255, 255, 255 };
        static CImg<unsigned char>
          help = CImg<unsigned char>().draw_text(0,0,"\n"
                                                 "  Use mouse to zoom on desired region.  \n"
                                                 "  H             Show/Hide help  \n"
                                                 "  PAD 1...9       Fractal navigation  \n"
                                                 "  PAD +/-       Zoom/Unzoom  \n"
                                                 "  SPACE         Set/Disable color smoothing  \n"
                                                 "  ENTER         Switch Mandelbrot/Julia sets  \n"
                                                 "  Arrows        Change set parameterization  \n"
                                                 "  Page UP/DOWN  Add/Reduce iteration numbers  \n\n",
                                                 white).resize(-100,-100,1,3);
        help.draw_rectangle(2,2,help.width() - 3,help.height() - 3,white,1,~0U);
        img.draw_image(img.width() - help.width(),help,0.7f);
      }

      // Get rectangular shape from the user to define the zoomed region.
      const CImg<int> selection = img.get_select(disp,2,0);
      const int xs0 = selection[0], ys0 = selection[1], xs1 = selection[3], ys1 = selection[4];

      // If the user has selected a region with the mouse, then zoom-in !
      if (xs0>=0 && ys0>=0 && xs1>=0 && ys1>=0) {
        const double dx =(xmax - xmin)/img.width(), dy =(ymax - ymin)/img.height();
        const int dsmax = (ys1 - ys0)/2, xs = (xs0 + xs1)/2, ys = (ys0 + ys1)/2;

        // If the region is too small (point) then reset the fractal set position and zoom.
        if (dsmax<5) stopflag = true;
        xmin+=(xs - dsmax*dy/dx)*dx;
        ymin+=(ys - dsmax)*dy;
        xmax-=(img.width() - xs - dsmax*dy/dx)*dx;
        ymax-=(img.height() - ys - dsmax)*dy;
      }

      // Also, test if a key has been pressed.
      // (moving in the fractal set can be done, using keyboard).
      switch (disp.key()) {

        // Show/hide help.
      case cimg::keyH: show_help = !show_help; break;

        // Switch between Julia/Mandelbrot sets.
      case cimg::keyENTER: fractal_type = !fractal_type; stopflag = true; break;

        // Enable/disable smoothed colors.
      case cimg::keySPACE: smooth = !smooth; break;

        // Change fractal set parameters.
      case cimg::keyARROWLEFT: julia_r-=fractal_type?0.001f:0.05f; break;
      case cimg::keyARROWRIGHT: julia_r+=fractal_type?0.001f:0.05f; break;
      case cimg::keyARROWUP: julia_i+=fractal_type?0.001f:0.05f; break;
      case cimg::keyARROWDOWN: julia_i-=fractal_type?0.001f:0.05f; break;

        // Add/remove iterations.
      case cimg::keyPAGEDOWN: maxiter-=32; break;
      case cimg::keyPAGEUP: maxiter+=16; break;

        // Move left, right, up and down in the fractal set.
      case cimg::keyPAD4: { const double delta = (xmax - xmin)/10; xmin-=delta; xmax-=delta; } break;
      case cimg::keyPAD6: { const double delta = (xmax - xmin)/10; xmin+=delta; xmax+=delta; } break;
      case cimg::keyPAD8: { const double delta = (ymax - ymin)/10; ymin-=delta; ymax-=delta; } break;
      case cimg::keyPAD2: { const double delta = (ymax - ymin)/10; ymin+=delta; ymax+=delta; } break;

        // Allow to zoom in/out in the fractal set.
      case cimg::keyPADADD: {
        const double xc = 0.5*(xmin + xmax), yc = 0.5*(ymin + ymax), dx = (xmax - xmin)*0.85/2, dy = (ymax - ymin)*0.85/2;
        xmin = xc - dx; ymin = yc - dy; xmax = xc + dx; ymax = yc + dy;
      } break;
      case cimg::keyPADSUB:
        const double xc = 0.5*(xmin + xmax), yc = 0.5*(ymin + ymax), dx = (xmax - xmin)*1.15/2, dy = (ymax - ymin)*1.15/2;
        xmin = xc - dx; ymin = yc - dy; xmax = xc + dx; ymax = yc + dy;
        break;
      }

      // Do a simple test to check if more/less iterations are necessary for the next step.
      const float value = (float)img.get_norm().get_histogram(256,0,255)(0)*3;
      if (value>img.size()/6.0f) maxiter+=16;
      if (maxiter>1024) maxiter = 1024;
      if (value<img.size()/10.0f) maxiter-=4;
      if (maxiter<32) maxiter = 32;

      // Check if the user want to quit the explorer.
      if (disp.is_closed() || disp.is_keyQ() || disp.is_keyESC()) stopflag = endflag = true;
    }
  }
  return 0;
}

// Item : Mini-Paint
//------------------
void* item_mini_paint() {
  int xo = -1, yo = -1, x = -1, y = -1;
  bool redraw = true;
  CImg<unsigned char> img(256,256+64,1,3,0);
  unsigned char color[] = { 255, 255, 255 };
  cimg_for_inY(img,256,img.height()-1,yy) cimg_forX(img,xx) img.fillC(xx,yy,0,xx,(yy - 256)*4,(3*xx)%256);
  CImgDisplay disp(img.draw_text(5,5,"   ",color,color),"[#8] - Mini-Paint");
  while (!disp.is_closed() && !disp.is_keyQ() && !disp.is_keyESC()) {
    const unsigned int but = disp.button();
    redraw = false;
    xo = x; yo = y; x = disp.mouse_x(); y = disp.mouse_y();
    if (xo>=0 && yo>=0 && x>=0 && y>=0) {
      if (but&1 || but&4) {
        if (y<253) {
          const float tmax = (float)cimg::max(cimg::abs(xo - x),cimg::abs(yo - y)) + 0.1f;
          const int radius = (but&1?3:0) + (but&4?6:0);
          for (float t = 0; t<=tmax; ++t) img.draw_circle((int)(x + t*(xo - x)/tmax),(int)(y + t*(yo - y)/tmax),radius,color);
        }
        if (y>=256) {
          color[0] = img(x,y,0); color[1] = img(x,y,1); color[2] = img(x,y,2);
          img.draw_text(5,5,"   ",color,color);
        }
        redraw = true;
      }
      if (y>=253) y = 252;
      if (disp.button()&2) { img.draw_fill(x,y,color); redraw = true; }
    }
    if (redraw) disp.display(img);
    disp.resize(disp).wait();
    if (disp.key()) cimg_forC(img,k) { img.get_shared_rows(0,255,0,k).fill(0); img.display(disp); }
  }
  return 0;
}

// Item : Soccer Bobs
//-------------------
void* item_soccer_bobs() {
  CImg<unsigned char> foot(data_foot,200,200,1,3,false), canvas0(640,480,1,3,0);
  const unsigned char color[] = { 255, 255, 0 };
  float zoom = 0.2f;
  cimg_forXY(canvas0,x,y) canvas0(x,y,1) = (unsigned char)(20 + (y*215/canvas0.height()) + 19*cimg::crand());
  canvas0.draw_text(5,5,"Left/Right Mouse Button = Zoom In/Out\nMiddle Button = Reset Screen",color);
  CImgList<unsigned char> canvas(16,canvas0);
  CImg<float> mask(foot.width(),foot.height());
  cimg_forXY(mask,x,y) mask(x,y) = (foot(x,y,0)==255 && !foot(x,y,1) && !foot(x,y,2))?0:0.8f;
  CImgDisplay disp(canvas0,"[#9] - Unlimited Soccer Bobs");
  for (unsigned int curr_canvas = 0; !disp.is_closed() && !disp.is_keyQ() && !disp.is_keyESC(); (++curr_canvas) %= 16) {
    if (disp.mouse_x()>=0 && disp.mouse_y()>=0)
      canvas[curr_canvas].draw_image((int)(disp.mouse_x() - zoom*foot.width()/2),
                                     (int)(disp.mouse_y() - zoom*foot.height()/2),
                                     foot.get_resize((int)(foot.width()*zoom),(int)(foot.height()*zoom)),
                                     mask.get_resize((int)(foot.width()*zoom),(int)(foot.height()*zoom)));
    zoom+=disp.button()&1?0.03f:disp.button()&2?-0.03f:0;
    zoom = zoom<0.1f?0.1f:zoom>1?1.0f:zoom;
    if (disp.button()&4) cimglist_for(canvas,l) canvas[l] = canvas0;
    if (disp.is_keyCTRLLEFT() && disp.is_keyF()) disp.toggle_fullscreen(false);
    disp.display(canvas[curr_canvas]).resize(disp,false).wait(20);
  }
  return 0;
}

// Item : Bump Effect
//--------------------
void* item_bump() {
  CImg<float> logo = CImg<>(56,32,1,1,0).draw_text(12,3,"I Love\nCImg !",CImg<>::vector(255).data()).
    resize(-800,-800,1,1,3).blur(6).normalize(0,255);
  logo+=CImg<>(logo.width(),logo.height(),1,1,0).noise(80,1).deriche(2,0,'y',false).deriche(10,0,'x',false);
  CImgList<float> grad = logo.get_gradient();
  cimglist_apply(grad,normalize)(-140,140);
  logo.normalize(0,255);
  CImg<float> light = CImg<>(300 + 2*logo.width(),300 + 2*logo.height());
  light.draw_gaussian(0.5f*light.width(),0.5f*light.height(),80,CImg<>::vector(255).data());
  CImg<unsigned char> img(logo.width(),logo.height(),1,3,0);
  CImgDisplay disp(img,"[#10] - Bump Effect (Move lightsource with mouse)");
  for (float t = 0; !disp.is_closed() && !disp.is_keyQ() && !disp.is_keyESC(); t+=0.03f) {
    const int
      mouse_x = (disp.mouse_x()>=0 && disp.button())?disp.mouse_x()*img.width()/disp.width():(int)(img.width()/2 + img.width()*std::cos(1*t)/2),
      mouse_y = (disp.mouse_y()>=0 && disp.button())?disp.mouse_y()*img.height()/disp.height():(int)(img.height()/2 + img.height()*std::sin(3*t)/2);
    cimg_forXY(img,x,y) {
      const int gx = (int)grad[0](x,y), gy = (int)grad[1](x,y);
      const float val = 40 + (gx + gy)/2 + light(light.width()/2 + mouse_x - x + gx,light.height()/2 + mouse_y - y + gy);
      img(x,y,0) = img(x,y,1) = img(x,y,2) = (unsigned char)(val>255?255:val<0?0:val);
    }
    disp.resize(false).display(img.draw_image(0,0,0,1,logo,0.1f)).wait(25);
    if (disp.is_keyCTRLLEFT() && disp.is_keyF()) disp.resize(640,480,false).toggle_fullscreen(false);
  }
  return 0;
}

// Item : Bouncing Bubble
//------------------------
void* item_bouncing_bubble() {
  CImg<unsigned char> back(420,256,1,3,0), img;
  cimg_forXY(back,x,y) back(x,y,2) = (unsigned char)((y<2*back.height()/3)?30:(255-2*(y+back.height()/2)));
  CImgDisplay disp(back,"[#11] - Bouncing bubble");
  const unsigned char col1[] = { 40, 100, 10 }, col2[] = { 20, 70, 0 }, col3[] = { 40, 150, 10 },
                      col4[] = { 200, 255, 100 }, white[] = { 255, 255, 255 };
  float u = (float)std::sqrt(2.0f), cx = back.width()/2.0f, t = 0, vt = 0.05f, vx = 2;
  while (!disp.is_closed() && !disp.is_keyQ() && !disp.is_keyESC()) {
    img = back;
    int xm = (int)cx, ym = (int)(img.height()/2 - 70 + (img.height()/2 + 10)*(1 - cimg::abs(std::cos((t+=vt)))));
    float r1 = 50, r2 = 50;
    vt = 0.05f;
    if (xm + r1>=img.width()) { const float delta = (xm + r1) - img.width(); r1-=delta; r2+=delta; }
    if (xm - r1<0) { const float delta = -(xm - r1); r1-=delta; r2+=delta; }
    if (ym + r2>=img.height()-40) { const float delta = (ym + r2) - img.height() + 40; r2-=delta; r1+=delta; vt = 0.05f - 0.0015f*(50 - r2); }
    if (ym - r2<0) { const float delta = -(ym - r2); r2-=delta; r1+=delta; }
    img.draw_ellipse(xm,ym,r1,r2,0,col1).
      draw_ellipse((int)(xm + 0.03*r1*u),(int)(ym - 0.03*r2*u),0.85f*r1,0.85f*r2,0,col2).
      draw_ellipse((int)(xm + 0.1*r1*u),(int)(ym - 0.1*r2*u),0.8f*r1,0.8f*r2,0,col1).
      draw_ellipse((int)(xm + 0.2*r1*u),(int)(ym - 0.2*r2*u),r1/2,r2/2,0,col3).
      draw_ellipse((int)(xm + 0.3*r1*u),(int)(ym - 0.3*r2*u),r1/4,r2/4,0,col4).
      draw_image(0,img.height() - 40,img.get_crop(0,img.height() - 80,img.width() - 1,img.height() - 40).mirror('y'),0.45f).
      draw_text(xm - 70,(int)(ym - r2 - 30),"Bubble (%d,%d)",white,0,0.7f,24,xm,ym);
    if ((cx+=20*vt*vx)>=img.width()-30 || cx<30) vx = -vx;
    disp.display(img).wait(20);
    if (disp.is_resized()) {
      back.resize(disp.resize(disp.window_width()>200?disp.window_width():200,disp.height(),false));
      cx = back.width()/2.0f;
    }
  }
  return 0;
}

// Item : Virtual Landscape
//--------------------------
void* item_virtual_landscape() {
  CImg<int> background(400,300,1,3,0), visu(background);
  cimg_forXY(background,x,y) {
    if (y>background.height()/2) { background(x,y,2) = 255; background(x,y,0) = (y - background.height()/2)*512/background.height(); }
    else background(x,y,2) = y*512/background.height();
  }
  const int white[] = { 255, 255, 255 };
  CImgDisplay disp(visu.draw_text(10,10,"Please wait, generating landscape...",white).
                   normalize(0,255),"[#12] - Virtual Landscape",0);
  CImg<float>
    map = 5.0*(CImg<>(700,700,1,1,300).noise(300).draw_plasma(0.2f,300).normalize(-140,150).blur(5).cut(0,150)),
    cmap(map.width(),map.height());
  CImg_3x3(I,float); Ipp = Inp = Icc = Ipn = Inn = 0;
  cimg_for3x3(map,x,y,0,0,I,float) {
    const float nox = 0.5f*(Inc - Ipc), noy = 0.5f*(Icn - Icp);
    cmap(x,y) = cimg::max(0.0f,0.5f*nox + noy);
  }
  cmap.normalize(0,255);

  for (float t = 0; !disp.is_closed() && !disp.is_keyQ() && !disp.is_keyESC(); t+=0.0025f) {
    visu = background;
    const int
      xm = (int)(map.width()/2 + (map.width()/3)*std::cos(4.2f*t)),
      ym = (int)(map.height()/2 + (map.height()/3)*std::sin(5.6f*t));
    const CImg<float>
      smap = map.get_crop(xm,ym,xm + 100,ym + 90),
      scmap = cmap.get_crop(xm,ym,xm + 100,ym + 90);
    CImg<int> ymin(visu.width(),1,1,1,visu.height()), ymax(ymin.width(),1,1,1,0);
    cimg_forY(smap,z) {
      const int y0 = (int)(visu.height() - 1 - 10*std::pow((double)z,0.63) + 80);
      cimg_forX(visu,x) {
        const int nz = smap.height() - z;
        float mx = x*(smap.width() - 2.0f*nz*0.2f)/visu.width() + 0.2f*nz;
        const int y = (int)(y0 - smap.linear_atX(mx,z)/(1 + 0.02*z));
        const float cc = (float)scmap.linear_atX(mx,z);
        if (y<visu.height() && y<ymin(x)) {
          const float cz = (smap.height() - (float)z)/smap.height(), czz = cz>0.25?1:4*cz;
          if (y!=y0) for (int l = y>0?y:0; l<ymin(x); ++l) {
            visu(x,l,0) = (int)((1 - czz)*visu(x,l,0) + 4*cc*czz);
            visu(x,l,1) = (int)((1 - czz)*visu(x,l,1) + 3*cc*czz);
            visu(x,l,2) = (int)((1 - czz)*visu(x,l,2) + cc*czz);
          } else for (int l = y>0?y:0; l<ymin(x); ++l) {
              int cl = l - visu.height()/2;
              visu(x,l,0) = 10; visu(x,l,1) = 200 - cl; visu(x,l,2) = 255 - cl;
            }
        }
        ymin(x) = cimg::min(ymin(x),y); ymax(x) = cimg::max(ymax(x),y);
      }
    }
    visu.draw_text(5,5,"%u frames/s",white,0,0.5f,13,(unsigned int)disp.frames_per_second());
    disp.resize(false).display(visu.cut(0,255)).wait(25);
    if (disp.is_keyCTRLLEFT() && disp.is_keyF()) disp.resize(400,300,false).toggle_fullscreen(false);
  }
  return 0;
}

// Item : Plasma Effect with Sinus Scrolling.
//-------------------------------------------
void* item_plasma() {
  CImg<float> plasma, camp(3), cfreq(3), namp(3), nfreq(3);
  CImgList<unsigned char> font = CImgList<unsigned char>::font(53);
  CImg<unsigned char> visu(400,300,1,3,0), letter, scroll(visu.width() + 2*font['W'].width(),font['W'].height(),1,1,0);
  const char *text = "   * The CImg Library : C++ Template Image Processing Toolkit *";
  CImgDisplay disp(visu,"[#13] - Plasma Effect");
  const unsigned char white[] = { 255, 255, 255 };
  unsigned int cplasma = 0, pos = 0, tpos = 0, lwidth = 0;
  float tx = 0, ts = 0, alpha = 2, beta = 0;
  namp.fill(0).noise(visu.height()/4,0);
  nfreq.fill(0).noise(0.1);

  visu.draw_text(10,10,"Please wait, generating plasma...",white).display(disp);
  const unsigned int nb_plasmas = 5;
  plasma.assign(5*visu.width()/3,visu.height()+1,1,nb_plasmas,0).noise(100).draw_plasma();
  cimg_forC(plasma,k) plasma.get_shared_channel(k).blur((float)(cimg::rand()*6)).normalize(0,255);

  while (!disp.is_closed() && !disp.is_keyQ() && !disp.is_keyESC()) {
    if (alpha>1) {
      alpha-=1;
      cplasma = (cplasma + 1)%plasma.spectrum();
      camp = namp;
      cfreq = nfreq;
      namp.fill(0).noise(100).normalize(0,visu.height()/4.0f);
      nfreq.fill(0).noise(0.2);
    }
    const unsigned int
      v0 = cplasma, v1 = (cplasma + 1)%plasma.spectrum(),
      v2 = (cplasma + 2)%plasma.spectrum(), v3 = (cplasma + 3)%plasma.spectrum();
    const float umalpha = 1 - alpha;

    unsigned char *ptr_r = visu.data(0,0,0,0), *ptr_g = visu.data(0,0,0,1), *ptr_b = visu.data(0,0,0,2);
    cimg_forY(visu,y) {
      const float
        *ptr_r1 = plasma.data((unsigned int)cimg::max(0.0f,camp(0)*(1.1 + std::sin(tx+cfreq(0)*y))),y,v0),
        *ptr_g1 = plasma.data((unsigned int)cimg::max(0.0f,camp(1)*(1.1 + std::sin(tx+cfreq(1)*y))),y,v1),
        *ptr_b1 = plasma.data((unsigned int)cimg::max(0.0f,camp(2)*(2 + std::sin(tx+cfreq(2)*y))),y,v2),
        *ptr_r2 = plasma.data((unsigned int)cimg::max(0.0f,namp(0)*(1.1 + std::sin(tx+nfreq(0)*y))),y,v1),
        *ptr_g2 = plasma.data((unsigned int)cimg::max(0.0f,namp(1)*(1.1 + std::sin(tx+nfreq(1)*y))),y,v2),
        *ptr_b2 = plasma.data((unsigned int)cimg::max(0.0f,namp(2)*(2 + std::sin(tx+nfreq(2)*y))),y,v3);
      cimg_forX(visu,x) {
        *(ptr_r++) = (unsigned char)(umalpha*(*(ptr_r1++)) + alpha*(*(ptr_r2++)));
        *(ptr_g++) = (unsigned char)(umalpha*(*(ptr_g1++)) + alpha*(*(ptr_g2++)));
        *(ptr_b++) = (unsigned char)(umalpha*(*(ptr_b1++)) + alpha*(*(ptr_b2++)));
      }
    }
    if (!pos) {
      const CImg<unsigned char>& letter = font(text[tpos]+256);
      lwidth = (unsigned int)letter.width();
      scroll.draw_image(visu.width(),letter);
      (++tpos) %= std::strlen(text);
    }
    scroll.shift(-2);
    if ((pos+=2)>lwidth+2) pos = 0;
    cimg_forX(visu,x) {
      const int y0 = (int)(visu.height()/2 + visu.height()/4*std::sin(ts + x/(70 + 30*std::cos(beta))));
      cimg_forY(scroll,y) {
        if (scroll(x,y)) {
          const unsigned int y1 = y0 + y + 2; visu(x,y1,0)/=2; visu(x,y1,1)/=2; visu(x,y1,2)/=2;
          const unsigned int y2 = y1 - 6; visu(x,y2,0) = visu(x,y2,1) = visu(x,y2,2) = 255;
        }
      }
    }
    alpha+=0.007f; beta+=0.04f; tx+=0.09f; ts+=0.04f;
    disp.resize(false).display(visu).wait(20);
    if (disp.is_keyCTRLLEFT() && disp.is_keyF()) disp.resize(640,480,false).toggle_fullscreen(false);
  }
  return 0;
}

// Item : Oriented Convolutions
//------------------------------
void* item_oriented_convolutions() {
  const CImg<unsigned char> img = CImg<unsigned char>(data_lena,256,256,1,1,false).noise(50,2);
  CImgList<unsigned char> visu = (img,img,img);
  CImg<float> mask(16,16);
  const float value = 255;
  CImgDisplay disp(visu,"[#14] - Original image, Oriented kernel and Convolved image");
  for (float angle = 0; !disp.is_closed() && !disp.is_keyQ() && !disp.is_keyESC(); angle+=0.1f) {
    const float ca = (float)std::cos(angle), sa = (float)std::sin(angle);
    const CImg<float>
      u = CImg<>::vector(ca,sa),
      v = CImg<>::vector(-sa,ca),
      tensor = 30.0*u*u.get_transpose() + 2.0*v*v.get_transpose();
    mask.draw_gaussian(0.5f*mask.width(),0.5f*mask.height(),tensor,&value);
    mask/=mask.sum();
    visu[1] = mask.get_resize(img).normalize(0,255).
      draw_text(2,2,"Angle = %d deg",&value,0,1,13,cimg::mod((int)(angle*180/cimg::PI),360));
    visu[2] = img.get_convolve(mask);
    disp.resize(disp.window_width(),(int)(disp.height()*disp.window_width()/disp.width()),false).
      display(visu).wait(25);
  }
  return 0;
}

// Item : Shade Bobs
//-------------------
void* item_shade_bobs() {
  float t = 100, rx = 0, ry = 0, rz = 0, rt = 0, rcx = 0;
  CImg<unsigned char> img(512,512,1,1,0), palette;
  CImgDisplay disp(img,"[#15] - Shade Bobs");
  const unsigned char one = 1;
  int nbbobs = 0, rybobs = 0;
  while (!disp.is_closed() && !disp.is_keyQ() && !disp.is_keyESC()) {
    if ((t+=0.015f)>4*cimg::PI) {
      img.fill(0);
      rx = (float)(cimg::crand());
      ry = (float)(cimg::crand());
      rz = (float)(cimg::crand());
      rt = (float)(cimg::crand());
      rcx = 0.6f*(float)(cimg::crand());
      t = 0;
      palette = CImg<unsigned char>(3,4 + (int)(12*cimg::rand()),1,1,0).noise(255,2).resize(3,256,1,1,3);
      palette(0) = palette(1) = palette(2) = 0;
      nbbobs = 20 + (int)(cimg::rand()*80);
      rybobs = (10 + (int)(cimg::rand()*50))*cimg::min(img.width(),img.height())/300;
    }
    for (int i = 0; i<nbbobs; ++i) {
      const float
        r = (float)(ry + rx*std::cos(6*rz*t) + (1 - rx)*std::sin(6*rt*t)),
        a = (float)((360*std::sin(rz*t) + 30*ry*i)*cimg::PI/180),
        ax = (float)(i*2*cimg::PI/nbbobs + t);
      const int
        cx = (int)((1 + rcx*std::cos(ax) + r*std::cos(a))*img.width()/2),
        cy = (int)((1 + rcx*std::sin(ax) + r*std::sin(a))*img.height()/2);
      img.draw_circle(cx,cy,rybobs,&one,-1.0f);
    }
    CImg_3x3(I,unsigned char); Ipp = Inp = Ipn = Inn = 0;
    CImg<unsigned char> tmp(img);
    cimg_for3x3(tmp,x,y,0,0,I,unsigned char) img(x,y) = (Inc + Ipc + Icn + Icp + (Icc<<2))>>3;
    CImg<unsigned char> visu(img.width(),img.height(),1,3);
    cimg_forXY(visu,xx,yy) {
      const unsigned char *col = palette.data(0,img(xx,yy));
      visu(xx,yy,0) = *(col++);
      visu(xx,yy,1) = *(col++);
      visu(xx,yy,2) = *(col++);
    }
    disp.display(visu).wait(25);
    if (disp.is_keyCTRLLEFT() && disp.is_keyF()) disp.resize(640,480,false).toggle_fullscreen(false);
    if (disp.is_resized()) img.resize(disp.resize(false),3);
    if ((disp.key() && !disp.is_keyCTRLLEFT()) || disp.button()) {
      t = 70; if (!(disp.is_keyQ() || disp.is_keyESC())) disp.set_key();
      disp.set_button();
    }
  }
  return 0;
}

// Item : Fourier Filtering
//-------------------------
void* item_fourier_filtering() {
  const CImg<unsigned char> img = CImg<unsigned char>(data_lena,256,256,1,1,false).resize(256,256);
  CImgList<float> F = img.get_FFT();
  cimglist_apply(F,shift)(img.width()/2,img.height()/2,0,0,2);
  const CImg<unsigned char> mag = ((F[0].get_pow(2) + F[1].get_pow(2)).sqrt() + 1).log().normalize(0,255);
  CImgList<unsigned char> visu(img,mag);
  CImgDisplay disp(visu,"[#16] - Fourier Filtering (Click to set filter)");
  CImg<unsigned char> mask(img.width(),img.height(),1,1,1);
  const unsigned char one[] = { 1 }, zero[] = { 0 }, white[] = { 255 };
  int rmin = 0, rmax = 256;
  while (!disp.is_closed() && !disp.is_keyQ() && !disp.is_keyESC()) {
    disp.wait();
    const int
      xm = disp.mouse_x()*2*img.width()/disp.width() - img.width(),
      ym = disp.mouse_y()*img.height()/disp.height(),
      x = xm - img.width()/2,
      y = ym - img.height()/2;
    if (disp.button() && xm>=0 && ym>=0) {
      const int r = (int)cimg::max(0.0f,(float)std::sqrt((float)x*x + y*y) - 3);
      if (disp.button()&1) rmax = r;
      if (disp.button()&2) rmin = r;
      if (rmin>=rmax) rmin = cimg::max(rmax - 1,0);
      mask.fill(0).draw_circle(mag.width()/2,mag.height()/2,rmax,one).
        draw_circle(mag.width()/2,mag.height()/2,rmin,zero);
      CImgList<float> nF(F);
      cimglist_for(F,l) nF[l].mul(mask).shift(-img.width()/2,-img.height()/2,0,0,2);
      visu[0] = nF.FFT(true)[0].normalize(0,255);
    }
    if (disp.is_resized()) disp.resize(disp.window_width(),disp.window_width()/2).display(visu);
    visu[1] = mag.get_mul(mask).draw_text(5,5,"Freq Min/Max = %d / %d",white,zero,0.6f,13,(int)rmin,(int)rmax);
    visu.display(disp);
  }
  return 0;
}

// Item : Image Zoomer
//---------------------
void* item_image_zoomer() {
  const CImg<unsigned char> img = CImg<unsigned char>(data_logo,555,103,1,3,false);
  CImgDisplay disp(img,"[#17] - Original Image"), dispz(500,500,"[#17] - Zoomed Image",0);
  disp.move((CImgDisplay::screen_width() - dispz.width())/2,(CImgDisplay::screen_height() - dispz.height() - disp.height())/2);
  dispz.move(disp.window_x(),disp.window_y() + disp.window_height() + 40);
  int factor = 20, x = 0, y = 0;
  bool grid = false, redraw = false;
  while (!disp.is_closed() && !dispz.is_closed() &&
         !disp.is_keyQ() && !dispz.is_keyQ() && !disp.is_keyESC() && !dispz.is_keyESC()) {
    if (disp.mouse_x()>=0) { x = disp.mouse_x(); y = disp.mouse_y(); redraw = true; }
    if (redraw) {
      const int
        x0 = x - factor, y0 = y - factor,
        x1 = x + factor, y1 = y + factor;
      const unsigned char red[] = { 255, 0, 0 }, black[] = { 0, 0, 0 }, white[] = { 255, 255, 255 };
      (+img).draw_rectangle(x0,y0,x1,y1,red,1.0f,~0U).display(disp);
      CImg<unsigned char> visu = img.get_crop(x0,y0,x1,y1).draw_point(x - x0,y - y0,red,0.2f).resize(dispz);
      if (grid) {
        const int bfac = 2*factor + 1;
        for (int i = 0; i<bfac; ++i) {
          const int X = i*dispz.width()/bfac, Y = i*dispz.height()/bfac;
          visu.draw_line(X,0,X,dispz.height() - 1,black).draw_line(0,Y,dispz.width() - 1,Y,black);
        }
      }
      visu.draw_text(2,2,"Coords (%d,%d)",white,0,1,13,x,y).display(dispz);
    }
    if (disp.button()&1) { factor = (int)(factor/1.5f); if (factor<3) factor = 3; disp.set_button(); redraw = true; }
    if (disp.button()&2) { factor = (int)(factor*1.5f); if (factor>100) factor = 100; disp.set_button(); redraw = true; }
    if (disp.button()&4 || dispz.button()) { grid = !grid; disp.set_button(); dispz.set_button(); redraw = true; }
    if (disp.is_resized()) disp.resize(disp);
    if (dispz.is_resized()) { dispz.resize(); redraw = true; }
    CImgDisplay::wait(disp,dispz);
  }
  return 0;
}

// Item : Blobs Editor
//--------------------
void* item_blobs_editor() {
  CImg<unsigned int> img(300,300,1,3);
  CImgList<unsigned int> colors;
  CImgList<float> blobs;
  CImgDisplay disp(img,"[#18] - Blobs Editor",0);
  const unsigned int white[] = { 255, 255, 255 };
  bool moving = false;

  for (float alpha = 0; !disp.is_closed() && !disp.is_keyQ() && !disp.is_keyESC(); alpha+=0.1f) {
    const int xm = disp.mouse_x()*img.width()/disp.width(), ym = disp.mouse_y()*img.height()/disp.height();
    int selected = -1;
    img.fill(0);

    if (blobs) {
      float dist = 0, dist_min = (float)img.width()*img.width() + img.height()*img.height();
      cimglist_for(blobs,l) {
        const CImg<float>& blob = blobs[l];
        const float
          xb = blob[0], yb = blob[1], rb = blob[2],
          sigma = (float)(rb*(1 + 0.05f*std::cos(blob[3]*alpha))),
          sigma2 = 2*sigma*sigma, precision = 4.5f*sigma2;
        const int tx0 = (int)(xb - 3*sigma), ty0 = (int)(yb - 3*sigma), tx1 = (int)(xb + 3*sigma), ty1 = (int)(yb + 3*sigma);
        const unsigned int
          col1 = colors[l](0), col2 = colors[l](1), col3 = colors[l](2), wh = img.width()*img.height(),
          x0 = tx0<0?0:tx0, y0 = ty0<0?0:ty0,
          x1 = tx1>=img.width()?img.width()-1:tx1, y1 = ty1>=img.height()?img.height()-1:ty1;
        float dy = y0 - yb;
        unsigned int *ptr = img.data(x0,y0);
        for (unsigned int y = y0; y<=y1; ++y) {
          float dx = x0 - xb;
          for (unsigned int x = x0; x<=x1; ++x) {
            float dist = dx*dx + dy*dy;
            if (dist<precision) {
              const float val = (float)exp(-dist/sigma2);
              *ptr+=(unsigned int)(val*col1);
              *(ptr + wh)+=(unsigned int)(val*col2);
              *(ptr + 2*wh)+=(unsigned int)(val*col3);
            }
            ++dx; ++ptr;
          }
          ptr+=img.width() - (x1 -x0) - 1;
          ++dy;
        }
        if ((dist = (xb - xm)*(xb - xm) + (yb - ym)*(yb - ym))<dist_min) { dist_min = dist; selected = l; }
      }

      for (unsigned int *ptr1 = img.data(0,0,0,1), *ptr2 = img.data(0,0,0,2), *ptr3 = img.end() + 1,
             off = 0, wh = img.width()*img.height(); ptr1>img.data(); ++off) {
        unsigned int val1 = *(--ptr1), val2 = *(--ptr2), val3 = *(--ptr3);
        const unsigned int pot = val1*val1 + val2*val2 + val3*val3;
        if (pot<128*128) { *ptr1 = *ptr3 = 255*off/wh; *ptr2 = 180*off/wh; }
        else {
          if (pot<140*140) { *ptr1 >>= 1; *ptr2 >>= 1; *ptr3 >>= 1; }
          else {
            *ptr1 = val1<255?val1:255;
            *ptr2 = val2<255?val2:255;
            *ptr3 = val3<255?val3:255;
          }
        }
      }
      cimglist_for(blobs,ll) {
        const CImg<float>& blob = blobs[ll];
        const int
          rb = (int)(blob[2]*(1 + 0.05f*std::cos(blob[3]*alpha))),
          xb = (int)(blob[0] + rb/2.5f),
          yb = (int)(blob[1] - rb/2.5f);
        img.draw_circle(xb,yb,rb>>1,white,0.2f).draw_circle(xb,yb,rb/3,white,0.2f).
          draw_circle(xb,yb,rb/5,white,0.2f);
      }
    } else {
      CImg<unsigned int> text;
      text.draw_text(0,0,
                     "CImg Blobs Editor\n"
                     "-----------------------\n\n"
                     "* Left mouse button :\n   Create and Move Blob.\n\n"
                     "* Right mouse button :\n  Remove nearest Blob.\n\n"
                     "* Colors and size of Appearing Blobs\n"
                     "  are randomly chosen.\n\n\n"
                     " >> Press mouse button to start ! <<",
                     white).resize(-100,-100,1,3);
      img.fill(100).draw_image((img.width() - text.width())/2,
                               (img.height() - text.height())/2,
                               text,text,1,255U);
    }

    if (disp.mouse_x()>=0 && disp.mouse_y()>=0) {
      if (disp.button()&1) {
        float dist_selected = 0;
        if (selected>=0) {
          const float a = xm - blobs[selected](0), b = ym - blobs[selected](1), c = blobs[selected](2);
          dist_selected = a*a + b*b - c*c;
        }
        if (moving || dist_selected<0) { blobs[selected](0) = (float)xm; blobs[selected](1) = (float)ym; }
        else {
          blobs.insert(CImg<>::vector((float)xm,(float)ym,(float)(10 + 30*cimg::rand()),(float)(3*cimg::rand())));
          colors.insert(CImg<>(3).fill(0).noise(255,1).normalize(0,255));
        }
        moving = true;
      } else moving = false;
      if (selected>=0 && disp.button()&2) { blobs.remove(selected); colors.remove(selected); disp.set_button(); }
    }

    img.display(disp.wait(25));
    if (disp.is_resized()) {
      img.resize(disp.resize(false));
      cimglist_for(blobs,l) if (blobs[l](0)>=img.width() || blobs[l](1)>=img.height()) { blobs.remove(l); colors.remove(l--); }
    }
  }
  return 0;
}

// Item : Double Torus
//---------------------
void* item_double_torus() {
  CImg<unsigned char> visu(300,256,1,3,0);
  CImgDisplay disp(visu,"[#19] - Double 3D Torus");
  CImgList<unsigned int> primitives;
  CImg<float> points = CImg<>::torus3d(primitives,60,20), points2 = CImg<>::rotation_matrix(1,0,0,(float)cimg::PI/2.0f)*points;
  CImgList<unsigned char> colors(2*primitives.size(),CImg<unsigned char>::vector(255,255,0));
  cimglist_for(primitives,ll) colors[ll++].fill(100,255,100);
  cimglist_for(primitives,l) if (l%2) colors[primitives.size()+l].fill(255,200,255); else colors[primitives.size()+l].fill(200,150,255);
  const CImg<float> opacities = CImg<>(primitives.size(),1,1,1,1.0f).append(CImg<>(primitives.size(),1,1,1,0.4f));
  points.shift_object3d(-30,0,0).append_object3d(primitives,points2.shift_object3d(30,0,0),primitives);
  float alpha = 0, beta = 0, gamma = 0, theta = 0;
  while (!disp.is_closed() && !disp.is_keyQ() && !disp.is_keyESC()) {
    visu.get_shared_channels(1,2).fill(0);
    visu.get_shared_row(visu.height()-1,0,0).noise(200,1);
    CImg_3x3(I,unsigned char); Ipp = Icp = Inp = Ipc = Inc = 0;
    cimg_for3x3(visu,x,y,0,0,I,unsigned char) visu(x,y,0) = (Icc+Ipn+Icn+Inn)>>2;
    for (unsigned int y = 0; y<100; ++y) std::memset(visu.data(0,y,0,2),255-y*255/100,visu.width());
    const CImg<float>
      rpoints = CImg<>::rotation_matrix(1,1,0,(alpha+=0.01f))*CImg<>::rotation_matrix(1,0,1,(beta-=0.02f))*
      CImg<>::rotation_matrix(0,1,1,(gamma+=0.03f))*points;
    if (disp.is_resized()) disp.resize(false);
    if (disp.is_keyCTRLLEFT() && disp.is_keyF()) disp.resize(300,256,false).toggle_fullscreen(false);
    visu.draw_object3d(visu.width()/2.0f,visu.height()/2.0f,0,
                       rpoints,primitives,colors,opacities,4,
                       false,500.0f,(float)(std::cos(theta+=0.01f)+1)*visu.width()/2.0f,
                       (float)visu.height(),-100.0f,0.1f,1.5f).
      display(disp.wait(25));
  }
  return 0;
}

// Item : 3D Metaballs
//---------------------
struct metaballs3d {
  float cx1, cy1, cz1, cx2, cy2, cz2, cx3, cy3, cz3;
  inline float operator()(const float x, const float y, const float z) const {
    const float
      x1 = x - cx1, y1 = y - cy1, z1 = z - cz1,
      x2 = x - cx2, y2 = y - cy2, z2 = z - cz2,
      x3 = x - cx3, y3 = y - cy3, z3 = z - cz3,
      r1 = 0.3f*(x1*x1 + y1*y1 + z1*z1),
      r2 = 0.4f*(x2*x2 + y2*y2 + z2*z2),
      r3 = 0.5f*(x3*x3 + y3*y3 + z3*z3);
    float potential = 0;
    if (r1<1.3f) potential+= 1.0f - r1*(r1*(4*r1+17)-22)/9;
    if (r2<1.3f) potential+= 1.0f - r2*(r2*(4*r2+17)-22)/9;
    if (r3<1.3f) potential+= 1.0f - r3*(r3*(4*r3+17)-22)/9;
    return potential;
  }
};

void* item_3d_metaballs() {
  CImg<unsigned char> img = CImg<unsigned char>(100,100,1,3,0).noise(100,2).draw_plasma(1,0,10).resize(512,320,1,3).blur(4);
  img.get_shared_channel(2)/=4; img.get_shared_channel(1)/=2;
  metaballs3d met;
  CImgList<unsigned int> primitives;
  CImgList<unsigned char> colors;
  const unsigned char white[] = { 255,255,255 };
  float alpha = 0, beta = 0, delta = 0, theta = 0, gamma = 0;
  CImgDisplay disp(img,"[#20] - 3D Metaballs");
  while (!disp.is_closed() && !disp.is_keyQ() && !disp.is_keyESC()) {
    met.cx2 = 1.5f*(float)std::cos(theta); met.cy2 = 2.5f*(float)std::sin(3*(theta+=0.017f)); met.cz2 = 0;
    met.cx1 = 0; met.cy1 = 2.0f*(float)std::sin(4*gamma); met.cz1 = 1.2f*(float)std::cos(2*(gamma-=0.0083f));
    met.cx3 = 2.5f*(float)std::cos(2.5*delta); met.cy3 = 0; met.cz3 = 1.5f*(float)std::sin(2*(delta+=0.0125f));
    const CImg<float>
      points = CImg<>::isosurface3d(primitives,met,0.8f,-4.5f,-4.5f,-3.5f,4.5f,4.5f,3.5f,24,24,24),
      rot = 50.0*CImg<>::rotation_matrix(0,0,1,(alpha+=0.02f))*CImg<>::rotation_matrix(1,1,0,(beta+=0.076f)),
      rpoints = rot*points;
    primitives.reverse_object3d();
    if (colors.size()<primitives.size()) colors.assign(primitives.size(),1,3,1,1);
    cimglist_for(primitives,ll) {
      colors(ll,0) = -60+191+64*ll/primitives.size();
      colors(ll,1) = -30+191+64*ll/primitives.size();
      colors(ll,2) = 255*ll/primitives.size();
    }
    if (primitives.size()) {
      (+img).draw_object3d(img.width()/2.0f,img.height()/2.0f,0.0f,
                           rpoints,primitives,
                           colors.get_shared_images(0,primitives.size()-1),
                           4,false,500, 0,0,-500, 0.1f,1.5f).
        draw_text(5,5,"%u frames/s",white,0,0.5f,13,(unsigned int)disp.frames_per_second()).display(disp.wait(20));
    }
    if (disp.is_resized()) disp.resize(false);
    if (disp.is_keyCTRLLEFT() && disp.is_keyF()) disp.resize(512,320,false).toggle_fullscreen(false);
  }
  return 0;
}

// Item : Fireworks
//------------------
void* item_fireworks() {
  CImg<unsigned char> img(640,480,1,3,0);
  CImgDisplay disp(img,"[#21] - Fireworks (Click to add/explode rockets)");
  CImgList<unsigned char> colors;
  const unsigned char white[] = { 255,255,255 }, red[] = { 128,0,0 };
  CImgList<float> particles;
  float time = 0, speed = 100.0f;

  while (!disp.is_closed() && !disp.is_keyQ() && !disp.is_keyESC()) {

    if (disp.button()&1 || !particles.size() || (--time)<0) {
      particles.insert(CImg<>::vector((float)cimg::rand()*img.width(),(float)img.height(),
                                      (float)cimg::crand()*4,-6-(float)cimg::rand()*3,
                                      30+60*(float)cimg::rand(),3));
      colors.insert(CImg<unsigned char>::vector(255,255,255));
      time = (float)(cimg::rand()*speed);
    }
    img*=0.92f;

    cimglist_for(particles,l) {
      bool remove_particle = false;
      float &x = particles(l,0), &y = particles(l,1), &vx = particles(l,2), &vy = particles(l,3),
            &t = particles(l,4), &r = particles(l,5);
      const float r2 = (t>0 || t<-42)?r/3:r*(1-2*(-(t+2)/40.0f)/3);
      img.draw_ellipse((int)x,(int)y,r,r2,(float)(std::atan2(vy,vx)*180/cimg::PI),colors[l].data(),0.6f);
      x+=vx; y+=vy; vy+=0.09f; t--;
      if (y>img.height()+10 || x<0 || x>=img.width()+10) remove_particle = true;

      if (t<0 && t>=-1) {
        if ((speed*=0.9f)<10) speed=10.0f;
        const unsigned char
          r = cimg::min(50+3*(unsigned char)(100*cimg::rand()), 255),
          g = cimg::min(50+3*(unsigned char)(100*cimg::rand()), 255),
          b = cimg::min(50+3*(unsigned char)(100*cimg::rand()), 255);
        const float di = 10+(float)cimg::rand()*60, nr = (float)cimg::rand()*30;
        for (float i=0; i<360; i+=di) {
          const float rad = i*(float)cimg::PI/180, c = (float)std::cos(rad), s = (float)std::sin(rad);
          particles.insert(CImg<>::vector(x,y,2*c+vx/1.5f,2*s+vy/1.5f,-2.0f,nr));
          colors.insert(CImg<unsigned char>::vector(r,g,b));
        }
        remove_particle = true;
      } else if (t<-1) { r*=0.95f; if (r<0.5f) remove_particle=true; }
      if (remove_particle) { particles.remove(l); colors.remove(l); l--; }
    }
    if (disp.button()&2) cimglist_for(particles,l) if (particles(l,4)>0) particles(l,4)=0.5f;
    img.draw_text(5,5," %u frames/s ",white,red,0.5f,13,(unsigned int)disp.frames_per_second());
    disp.display(img).wait(25);
    if (disp.is_keyCTRLLEFT() && disp.is_keyF()) disp.resize(640,480,false).toggle_fullscreen(false);
    if (disp.is_resized()) disp.resize(disp,false);
  }
  return 0;
}

// Item : Rubber Logo
//--------------------
void* item_rubber_logo() {
  const unsigned char white[] = { 255,255,255 };
  CImg<unsigned char> background = CImg<unsigned char>(300,300).noise(100,2);
  background(0,0) = background(299,0) = background(299,299) = background(0,299) = 0;
  background.draw_plasma().blur(1.0f,14.0f,0.0f,0).resize(-100,-100,1,3);
  CImgDisplay disp(CImg<unsigned char>(background).
                   draw_text(10,10,"Please wait, generating rubber object...",white),"[#22] - 3D Rubber Logo");

  CImg<unsigned char> vol = CImg<unsigned char>().draw_text(30,30,"CImg",white,0,1,57).resize(-100,-100,15,1);
  for (unsigned int k = 0; k<5; ++k) { vol.get_shared_slice(k).fill(0); vol.get_shared_slice(vol.depth()-1-k).fill(0); }
  vol.resize(vol.width()+30,vol.height()+30,-100,1,0).blur(2).resize(-50,-50);
  CImgList<unsigned int> faces;
  CImg<float> points = vol.get_isosurface3d(faces,45);
  CImgList<unsigned int> colors(faces.size(),CImg<unsigned char>::vector(100,100,255));
  cimglist_for(colors,l) {
    const float x = (points(faces(l,0),0) + points(faces(l,1),0) + points(faces(l,2),0))/3;
    if (x<30) colors[l] = CImg<unsigned char>::vector(255,100,100);
    else { if (x<34) colors[l] = CImg<unsigned char>::vector(200,155,100);
    else { if (x<55) colors[l] = CImg<unsigned char>::vector(100,255,155);
    }}}
  faces.reverse_object3d();
  points.shift_object3d()*=5.5f;

  CImgList<unsigned char> frames(100,background);
  bool ok_visu = false;
  unsigned int nb_frame = 0;
  float alpha = 0, beta = 0, gamma = 0;

  while (!disp.is_closed() && !disp.is_keyQ() && !disp.is_keyESC()) {
    CImg<unsigned char>& frame = frames[nb_frame++];
    if (nb_frame>=frames.size()) { ok_visu = true; nb_frame = 0; }
    const CImg<float>
      rot = CImg<>::rotation_matrix(0,1,0.2f,alpha+=0.011f)*
      CImg<>::rotation_matrix(1,0.4f,1,beta+=0.015f)*
      (1+0.1f*std::cos((double)(gamma+=0.1f)));
    (frame=background).draw_object3d(frame.width()/2.0f,frame.height()/2.0f,frame.depth()/2.0f,rot*points,faces,colors,5,
                                     false,500,0,0,-5000,0.1f,1.0f);
    if (ok_visu) {
      CImg<unsigned char> visu(frame);
      cimglist_for(frames,l) {
        const unsigned int
          y0 = l*visu.height()/frames.size(),
          y1 = (l+1)*visu.height()/frames.size()-1;
        cimg_forC(visu,k) visu.get_shared_rows(y0,y1,0,k) = frames[(nb_frame+l)%frames.size()].get_shared_rows(y0,y1,0,k);
      }
      visu.get_resize(disp,1).draw_text(5,5," %u frames/s ",white,0,0.5f,13,(unsigned int)disp.frames_per_second()).display(disp.wait(20));
    }
    if (disp.is_keyCTRLLEFT() && disp.is_keyF()) disp.resize(300,300,false).toggle_fullscreen(false);
    if (disp.is_resized()) disp.resize();
  }
  return 0;
}

// Item : Image Waves
//--------------------
void* item_image_waves() {
  const CImg<unsigned char> img = CImg<unsigned char>(data_milla,211,242,1,3,false).get_resize(128,128,1,3);
  CImgList<unsigned int> faces0;
  CImgList<unsigned char> colors0;
  const CImgList<float>
    points0 = (img.get_elevation3d(faces0,colors0,img.get_channel(0).fill(0)).shift_object3d()*=3)<'x',
    opacities0(faces0.size(),1,1,1,1,1.0f);
  CImg<unsigned char>
    back = CImg<unsigned char>(400,300,1,3).sequence(0,130),
    ball = CImg<unsigned char>(12,12,1,3,0).draw_circle(6,6,5,CImg<unsigned char>::vector(0,128,64).data());
  const CImg<float> mball = CImg<>(12,12,1,1,0).draw_circle(6,6,5,CImg<>::vector(1.0f).data());
  ball.draw_circle(7,5,4,CImg<unsigned char>::vector(16,96,52).data()).
    draw_circle(8,4,2,CImg<unsigned char>::vector(0,128,64).data()).
    draw_circle(8,4,1,CImg<unsigned char>::vector(64,196,128).data());
  CImg<float> uc(img.width()/2,img.height()/2,1,1,0), up(uc), upp(uc);
  CImgList<float> particles;
  CImgDisplay disp(back,"[#23] - Image Waves (Try mouse buttons!)");
  for (float alpha = 0.0f, count = 10.0f; !disp.is_closed() && !disp.is_keyQ() && !disp.is_keyESC(); ) {
    if ((disp.button()&1 && disp.mouse_x()>=0) || --count<0) {
      CImg<>::vector((float)(cimg::rand()*(img.width()-1)),(float)(cimg::rand()*(img.height()-1)),-200,0).move_to(particles);
      count = (float)(cimg::rand()*15);
    }
    alpha = (disp.mouse_x()>=0 && disp.button()&2)?(float)(disp.mouse_x()*2*cimg::PI/disp.width()):(alpha+0.02f);
    if (disp.is_keyCTRLLEFT() && disp.is_keyF()) disp.resize(400,300,false).toggle_fullscreen(false);
    cimglist_for(particles,l) {
      float& z = up((int)particles(l,0)>>1,(int)particles(l,1)>>1);
      if ((particles(l,2)+=(particles(l,3)+=0.5f))>z-10) { z = 250.0f; particles.remove(l--); }
    }
    CImg_3x3(U,float); Upp = Unp = Ucc = Upn = Unn = 0;
    cimg_for3x3(up,x,y,0,0,U,float) uc(x,y) = (Unc+Upc+Ucn+Ucp)/2 - upp(x,y);
    (uc-=(float)(uc.blur(0.7f).mean())).swap(upp).swap(up);
    CImgList<float> points(points0);
    CImgList<unsigned int> faces(faces0);
    CImgList<unsigned char> colors(colors0);
    CImgList<float> opacities(opacities0);
    cimglist_for(points,p) points(p,2) = cimg::min(30 + uc.linear_atXY((p%img.width())/2.0f,(p/img.width())/2.0f),70.0f);
    cimglist_for(particles,l) {
      points.insert(CImg<>::vector(3*(particles(l,0)-img.width()/2.0f),3*(particles(l,1)-img.height()/2.0f),30.0f+particles(l,2)));
      faces.insert(CImg<unsigned int>::vector(points.size()-1));
      colors.insert(ball,~0U,true);
      opacities.insert(mball,~0U,true);
    }
    const CImg<float>
      rot = CImg<>::rotation_matrix(1.0f,0,0,(float)(cimg::PI/3.0f))*CImg<>::rotation_matrix(0,0,1.0f,alpha),
      rpoints = rot*(points>'x');
    (+back).draw_object3d(back.width()/2.0f,back.height()/2.0f,0,rpoints,faces,colors,opacities,4,false,500.0f,0,0,0,1,1).
      display(disp.resize(false).wait(30));
  }
  return 0;
}

// Item : Breakout
//-----------------
void* item_breakout() {

  // Init graphics
  CImg<unsigned char>
    board(8,10,1,1,0),
    background = CImg<unsigned char>(board.width()*32,board.height()*16+200,1,3,0).noise(20,1).draw_plasma().blur(1,8,0,true),
    visu0(background/2.0), visu(visu0), brick(16,16,1,1,200), racket(64,8,1,3,0), ball(8,8,1,3,0);
  const unsigned char white[] = { 255,255,255 }, green1[] = { 60,150,30 }, green2[] = { 130,255,130 };
  cimg_for_borderXY(brick,x,y,1) brick(x,y) = x>y?255:128;
  cimg_for_insideXY(brick,x,y,1) brick(x,y) = cimg::min(255,64+8*(x+y));
  brick.resize(31,15,1,1,1).resize(32,16,1,1,0);
  ball.draw_circle(4,4,2,white); ball-=ball.get_erode(3)/1.5;
  racket.draw_circle(4,3,4,green1).draw_circle(3,2,2,green2);
  cimg_forY(racket,y) racket.draw_rectangle(4,y,racket.width()-7,y,CImg<unsigned char>::vector(y*4,255-y*32,255-y*25).data());
  racket.draw_image(racket.width()/2,racket.get_crop(0,0,racket.width()/2-1,racket.height()-1).mirror('x'));
  const int
    w = visu.width(), h = visu.height(), w2 = w/2, h2 = h/2,
    bw = ball.width(), bh = ball.height(), bw2 = bw/2, bh2 = bh/2,
    rw = racket.width(), rh = racket.height(), rw2 = rw/2;
  float xr = (float)(w-rw2), oxr = (float)xr, xb = 0, yb = 0, oxb = 0, oyb = 0, vxb = 0, vyb = 0;
  const CImg<unsigned char> racket_mask = racket.get_threshold(1).channel(1), ball_mask = ball.get_threshold(1).channel(1);

  // Begin game loop
  CImgDisplay disp(visu,"[#24] - Breakout");
  disp.move((CImgDisplay::screen_width() - w)/2,(CImgDisplay::screen_height() - h)/2);
  for (unsigned int N = 0, N0 = 0; !disp.is_closed() && !disp.is_keyQ() && !disp.is_keyESC(); ) {
    if (N0) {
      int X = (int)xr;
      if (disp.mouse_x()>=0) X = (int)(w2+((disp.mouse_x()<0?w2:disp.mouse_x())-w2)*2);
      else disp.set_mouse(xr>w2?w-81:80,h2);
      if (X<rw2) { X = rw2; disp.set_mouse(80,h2); }
      if (X>=w-rw2) { X = w-rw2-1; disp.set_mouse(w-81,h2); }
      oxr = xr; xr = (float)X; oxb = xb; oyb = yb; xb+=vxb; yb+=vyb;
      if ((xb>=w-bw2) || (xb<bw2)) { xb-=vxb; yb-=vyb; vxb=-vxb; }
      if (yb<bh2) { yb = (float)bh2; vyb=-vyb; }
      if (yb>=h-rh-8-bh2 && yb<h-8-bh2 && xr-rw2<=xb && xr+rw2>=xb) {
        xb = oxb; yb = h-rh-8.0f-bh2; vyb=-vyb; vxb+=(xr-oxr)/4;
        if (cimg::abs(vxb)>8) vxb*=8/cimg::abs(vxb);
      }
      if (yb<board.height()*16) {
        const int X = (int)xb/32, Y = (int)yb/16;
        if (board(X,Y)) {
          board(X,Y) = 0;
          ++N;
          const unsigned int x0 = X*brick.width(), y0 = Y*brick.height(), x1 = (X+1)*brick.width()-1, y1 = (Y+1)*brick.height()-1;
          visu0.draw_image(x0,y0,background.get_crop(x0,y0,x1,y1));
          if (oxb<(X<<5) || oxb>=((X+1)<<5)) vxb=-vxb;
          else if (oyb<(Y<<4) || oyb>=((Y+1)<<4)) vyb=-vyb;
        }
      }
      disp.set_title("[#24] - Breakout : %u/%u",N,N0);
    }
    if (yb>h || N==N0) {
      disp.show_mouse();
      while (!disp.is_closed() && !disp.key() && !disp.button()) {
        ((visu=visu0)/=2).draw_text(50,visu.height()/2-10,N0?" Game Over !":"Get Ready ?",white,0,1,24).
          display(disp);
        disp.wait();
        if (disp.is_resized()) disp.resize(disp);
      }
      board.fill(0); visu0 = background;
      cimg_forXY(board,x,y) if (0.2f+cimg::crand()>=0) {
        CImg<float> cbrick = CImg<double>::vector(100+cimg::rand()*155,100+cimg::rand()*155,100+cimg::rand()*155).
          unroll('v').resize(brick.width(),brick.height());
        cimg_forC(cbrick,k) (cbrick.get_shared_channel(k).mul(brick))/=255;
        visu0.draw_image(x*32,y*16,cbrick);
        board(x,y) = 1;
      }
      N0 = (int)board.sum(); N = 0;
      oxb = xb = (float)w2; oyb = yb = board.height()*16.0f+bh; vxb = 2.0f; vyb = 3.0f;
      disp.hide_mouse();
    } else disp.display((visu=visu0).draw_image((int)(xr-rw2),h-rh-8,racket,racket_mask).
                        draw_image((int)(xb-bw2),(int)(yb-bh2),ball,ball_mask));
    if (disp.is_resized()) disp.resize(disp);
    disp.wait(20);
  }
  return 0;
}

// Item : 3D Reflection
//----------------------
void* item_3d_reflection() {

  // Init images and display
  CImgDisplay disp(512,512,"[#25] - 3D Reflection",0);
  CImg<unsigned char> back = CImg<unsigned char>(200,400,1,3,0).rand(0,255).draw_plasma();
  ((back,back.get_mirror('x'),back)>'x').blur(15,1,0,true).columns(100,499).normalize(0,120).move_to(back);
  CImg<unsigned char> light0 = back.get_resize(-50,-50,1,1).normalize(1,255), visu(back), reflect(back.width(),back.height(),1,1), light(light0);
  back.get_shared_channel(0)/=3;
  back.get_shared_channel(2)/=2;

  // Create 3D objects.
  CImgList<unsigned int> back_faces, main_faces;
  CImgList<unsigned char> main_colors, back_colors, light_colors, light_colors2;
  CImgList<float> back_pts0, main_pts = CImg<>::torus3d(main_faces,30,12,24,12)<'x';
  cimglist_for(main_faces,l)
    if (l%2) CImg<unsigned char>::vector(255,120,16).move_to(main_colors);
    else CImg<unsigned char>::vector(255,100,16).move_to(main_colors);

  const unsigned int res1 = 32, res2 = 32;
  for (unsigned int v = 1; v<res2; ++v) for (unsigned int u = 0; u<res1; ++u) {
    const float
      alpha = (float)(u*2*cimg::PI/res1), beta = (float)(-cimg::PI/2 + v*cimg::PI/res2),
      x = (float)(std::cos(beta)*std::cos(alpha)),
      y = (float)(std::cos(beta)*std::sin(alpha)),
      z = (float)(std::sin(beta));
    back_pts0.insert(CImg<>::vector(x,y,z));
  }
  const unsigned int N = back_pts0.size();
  back_pts0.insert(CImg<>::vector(0,0,-140)).insert(CImg<>::vector(0,0,140));
  CImg<float> back_pts = back_pts0>'x';
  for (unsigned int vv = 0; vv<res2-2; ++vv) for (unsigned int uu = 0; uu<res1; ++uu) {
    const int nv = (vv+1)%(res2-1), nu = (uu+1)%res1;
    back_faces.insert(CImg<unsigned int>::vector(res1*vv+nu,res1*nv+uu,res1*vv+uu));
    back_faces.insert(CImg<unsigned int>::vector(res1*vv+nu,res1*nv+nu,res1*nv+uu));
    back_colors.insert(CImg<unsigned char>::vector(128,255,255));
    back_colors.insert(CImg<unsigned char>::vector(64,240,196));
  }
  for (unsigned int uu = 0; uu<res1; ++uu) {
    const int nu = (uu+1)%res1;
    back_faces.insert(CImg<unsigned int>::vector(nu,uu,N));
    back_faces.insert(CImg<unsigned int>::vector(res1*(res2-2)+nu, N+1,res1*(res2-2)+uu));
    if (uu%2) back_colors.insert(2,CImg<unsigned char>::vector(128,255,255));
    else back_colors.insert(2,CImg<unsigned char>::vector(64,240,196));
  }
  light_colors.assign(main_faces.size(),CImg<unsigned char>::vector(255));
  light_colors2.assign(back_faces.size(),CImg<unsigned char>::vector(255)).insert(light,~0U,true);

  // Start 3D animation.
  for (float main_x = -1.5f*visu.width(),
         back_alpha = 0, back_beta = 0, back_theta = -3.0f,
         main_alpha = 0, main_beta = 0, main_theta = 0;
       !disp.is_closed() && !disp.is_keyQ() && !disp.is_keyESC();
       main_alpha+=0.041f, main_beta+=0.063f, main_theta+=0.02f,
         back_alpha+=0.0031f, back_beta+=0.0043f, back_theta+=0.01f) {
    const int
      main_X = (int)(visu.width()/2 + main_x + 100*std::cos(2.1*main_theta)),
      main_Y = (int)(visu.height()/2 + 120*std::sin(1.8*main_theta));
    CImg<float>
      rmain_pts = (CImg<>::rotation_matrix(-1,1,0,main_alpha)*CImg<>::rotation_matrix(1,0,1,main_beta))*(main_pts>'x'),
      rback_pts = (CImg<>::rotation_matrix(1,1,0,back_alpha)*CImg<>::rotation_matrix(0.5,0,1,back_beta))*back_pts;

    (light=light0).draw_object3d(main_X/2.0f,main_Y/2.0f,0,rmain_pts,main_faces,light_colors,3,false,500,0,0,-5000,0.2f,0.1f);
    reflect.fill(0).draw_object3d(2*visu.width()/3.0f,visu.height()/2.0f,0,rback_pts,back_faces,light_colors2,5,false,500,0,0,-5000,0.2f,0.1f);
    rmain_pts*=2;
    (visu=back).draw_object3d(2*visu.width()/3.0f,visu.height()/2.0f,0,rback_pts,back_faces,back_colors,3,false,500,0,0,-5000,0.2f,0.1f);

    unsigned char *ptrs = reflect.data(), *ptrr = visu.data(0,0,0,0), *ptrg = visu.data(0,0,0,1), *ptrb = visu.data(0,0,0,2);
    cimg_foroff(reflect,xy) {
      const unsigned char v = *(ptrs++);
      if (v) { *ptrr = (*ptrr+v)>>1; *ptrg = (*ptrr+v)>>1; *ptrb = (*ptrb+v)>>1; }
      ++ptrr; ++ptrg; ++ptrb;
    }

    visu.draw_object3d((float)main_X,(float)main_Y,0,rmain_pts,main_faces,main_colors,4,
                       false,500,0,0,-5000,0.1f,1.4f);

    if (disp.is_resized()) {
      const int s = cimg::min(disp.window_width(),disp.window_height());
      disp.resize(s,s,false);
    }
    if (disp.is_keyCTRLLEFT() && disp.is_keyF()) disp.resize(512,512,false).toggle_fullscreen(false);
    disp.display(visu).wait(20);
    back.shift(4,0,0,0,2);
    light0.shift(-2,0,0,0,2);
    if (main_x<0) main_x +=2;
    const float H = back_theta<0?0.0f:(float)(0.3f-0.3f*std::cos(back_theta));
    for (unsigned int p = 0, v = 1; v<res2; ++v) for (unsigned int u = 0; u<res1; ++u) {
      const float
        alpha = (float)(u*2*cimg::PI/res1), beta = (float)(-cimg::PI/2 + v*cimg::PI/res2),
        x = back_pts0(p,0), y = back_pts0(p,1), z = back_pts0(p,2),
        altitude = 140*(float)cimg::abs(1+H*std::sin(3*alpha)*std::cos(5*beta));
      back_pts(p,0) = altitude*x; back_pts(p,1) = altitude*y; back_pts(p,2) = altitude*z;
      ++p;
    }
  }
  return 0;
}

// Item : Fish-Eye Magnification
//------------------------------
void* item_fisheye_magnification() {
  const unsigned char purple[] = { 255, 0, 255 }, white[] = { 255, 255, 255 }, black[] = { 0, 0, 0 };
  const CImg<unsigned char> img0 = CImg<unsigned char>(data_logo,555,103,1,3,true).get_resize(-144,-144,1,3,6);
  CImgDisplay disp(img0,"[#26] - Fish-Eye Magnification");
  int rm = 80, xc = 0, yc = 0, rc = 0;
  CImg<unsigned char> img, res;
  for (float alpha = 0; !disp.is_closed() && !disp.is_keyQ() && !disp.is_keyESC(); alpha+=0.02f) {
    if (!img) img = img0.get_resize(disp,3);
    if (disp.mouse_x()>=0) { xc = disp.mouse_x(); yc = disp.mouse_y(); rc = rm; }
    else {
      xc = (int)(img.width()*(1 + 0.9f*std::cos(1.2f*alpha))/2);
      yc = (int)(img.height()*(1 + 0.8f*std::sin(3.4f*alpha))/2);
      rc = (int)(90 + 60*std::sin(alpha));
    }
    const int x0 = xc - rc, y0 = yc - rc, x1 = xc + rc, y1 = yc + rc;
    res = img;
    cimg_for_inXY(res,x0,y0,x1,y1,x,y) {
      const float X = (float)x - xc, Y = (float)y - yc, r2 = X*X + Y*Y, rrc = (float)std::sqrt(r2)/rc;
      if (rrc<1) {
        const int xi = (int)(xc + rrc*X), yi = (int)(yc + rrc*Y);
        res(x,y,0) = img(xi,yi,0); res(x,y,1) = img(xi,yi,1); res(x,y,2) = img(xi,yi,2);
      }
    }
    const int xf = xc+3*rc/8, yf = yc-3*rc/8;
    res.draw_circle(xc,yc,rc,purple,0.2f).draw_circle(xf,yf,rc/3,white,0.2f).draw_circle(xf,yf,rc/5,white,0.2f).
      draw_circle(xf,yf,rc/10,white,0.2f).draw_circle(xc,yc,rc,black,0.7f,~0U);
    disp.display(res).wait(20);
    rm+=(disp.button()&1?8:(disp.button()&2?-8:0));
    rm = rm<30?30:(rm>200?200:rm);
    if (disp.is_resized()) { disp.resize(false); img.assign(); }
  }
  return 0;
}

// Item : Word Puzzle
//--------------------
void* item_word_puzzle() {

  // Create B&W and color letters
  CImg<unsigned char> model(60,60,1,3,0), color(3), background, canvas, elaps;
  CImgList<unsigned char> letters('Z'-'A'+1), cletters(letters);
  const unsigned char white[] = { 255, 255, 255 }, gray[] = { 128, 128, 128 }, black[] = { 0, 0, 0 };
  char tmptxt[] = { 'A',0 };
  model.fill(255).draw_rectangle(5,5,54,54,gray).blur(3,0).threshold(140).normalize(0,255);
  cimglist_for(letters,l)
    (letters[l].draw_text(5,2,&(tmptxt[0]='A'+l),white,0,1,57).resize(60,60,1,1,0,0,0.5,0.5).
     resize(-100,-100,1,3)|=model).blur(0.5);
  { cimglist_for(cletters,l) {
    CImg<int> tmp = letters[l];
    color.rand(100,255);
    cimg_forC(tmp,k) (tmp.get_shared_channel(k)*=color[k])/=255;
    cletters[l] = tmp;
  }}

  CImgDisplay disp(500,400,"[#27] - Word Puzzle",0);
  while (!disp.is_closed() && !disp.is_keyQ() && !disp.is_keyESC()) {

    // Create background, word data and display.
    background.assign(40,40,1,2,0).noise(30,2).distance(255).normalize(0,255).resize(500,400,1,3,3);
    CImg<int> current(14,6,1,1,0), solution(14,4,1,1,0);
    current.get_shared_row(0).fill('T','H','E','C','I','M','G','L','I','B','R','A','R','Y');
    current.get_shared_row(1).rand(-30,background.width()-30);
    current.get_shared_row(2).rand(-30,background.height()-30);
    solution.get_shared_row(0) = current.get_shared_row(0);
    solution.get_shared_row(1).fill(20,80,140,100,180,260,340,40,100,160,220,280,340,400);
    solution.get_shared_row(2).fill(20,20,20,120,150,180,210,310,310,310,310,310,310,310);
    cimg_forX(solution,l) background.draw_image(solution(l,1),solution(l,2),letters(solution(l)-'A'),0.3f);
    const int last = current.width()-1;

    // Start user interaction
    int timer = 0, completed = 0;
    for (bool selected = false, refresh_canvas = true, stopflag = false;
         !stopflag && !disp.is_closed() && !disp.is_keyQ() && !disp.is_keyESC(); disp.resize(disp).wait(20)) {
      if (refresh_canvas) {
        canvas = background;
        cimg_forX(current,l) if (!current(l,5)) {
          int &x = current(l,1), &y = current(l,2);
          if (x<-30) x = -30; else if (x>canvas.width()-30) x = canvas.width()-30;
          if (y<-30) y = -30; else if (y>canvas.height()-30) y = canvas.height()-30;
          canvas.draw_rectangle(x+8,y+8,x+67,y+67,black,0.3f).draw_image(x,y,cletters(current(l)-'A'));
        }
        refresh_canvas = false;
      }
      (+canvas).draw_text(280,3,"Elapsed Time : %d",white,0,0.7f,24,timer++).display(disp);

      if (disp.button()&1) {
        const int mx = disp.mouse_x(), my = disp.mouse_y();
        if (mx>=0 && my>=0) {
          if (!selected) {
            int ind = -1;
            cimg_forX(current,l) if (!current(l,5)) {
              const int x = current(l,1), y = current(l,2), dx = mx - x, dy = my - y;
              if (dx>=0 && dx<60 && dy>=0 && dy<60) { selected = true; ind = l; current(l,3) = dx; current(l,4) = dy; }
            }
            if (ind>=0 && ind<last) {
              const CImg<int> vec = current.get_column(ind);
              current.draw_image(ind,current.get_crop(ind+1,last)).draw_image(last,vec);
            }
          } else { current(last,1) = mx - current(last,3); current(last,2) = my - current(last,4); refresh_canvas = true; }
        }
      } else {
        bool win = true;
        cimg_forX(solution,j) if (!solution(j,3)) {
          win = false;
          const int x = solution(j,1), y = solution(j,2);
          cimg_forX(current,i) if (!current(i,5) && solution(j)==current(i)) {
            const int xc = current(i,1), yc = current(i,2), dx = cimg::abs(x-xc), dy = cimg::abs(y-yc);
            if (dx<=12 && dy<=12) {
              cimg_forC(background,k) cimg_forY(letters[0],y)
                background.get_shared_row(solution(j,2)+y,0,k).
                draw_image(solution(j,1),0,
                           (CImg<>(cletters(solution(j)-'A').get_shared_row(y,0,k))*=2.0*std::cos((y-30.0f)/18)).
                           cut(0,255),0.8f);
              current(i,5) = solution(j,3) = 1; refresh_canvas = true;
            }
          }
        }
        selected = false;
        if (win) { stopflag = true; completed = 1; }
      }
    }

    // Display final score
    const char
      *const mention0 = "Need more training !", *const mention1 = "Still amateur, hu ?",
      *const mention2 = "Not so bad !", *const mention3 = "  Good !", *const mention4 = "Very good !",
      *const mention5 = " Expert !",
      *mention = completed?(timer<700?mention5:timer<800?mention4:timer<900?mention3:timer<1000?mention2:timer<1200?mention1:mention0):mention0;
    canvas.assign().draw_text(0,0,"Final time : %d\n\n%s",white,0,1,32,timer,mention).resize(-100,-100,1,3);
    ((background/=2)&CImg<unsigned char>(2,2).fill(0,255,255,0).resize(background,0,2)).
      draw_image((background.width()-canvas.width())/2,(background.height()-canvas.height())/2,
                 canvas,canvas.get_dilate(3).dilate(3).dilate(3),1,255).display(disp.flush());
    while (!disp.is_closed() && !disp.key() && !disp.button()) disp.resize(disp).wait();
  }
  return 0;
}

// Run a selected effect
//-----------------------
void start_item(const unsigned int demo_number) {
  switch (demo_number) {
  case 1: item_blurring_gradient(); break;
  case 2: item_rotozoom(); break;
  case 3: item_anisotropic_smoothing(); break;
  case 4: item_fractal_animation(); break;
  case 5: item_gamma_correction(); break;
  case 6: item_filled_triangles(); break;
  case 7: item_mandelbrot_explorer(); break;
  case 8: item_mini_paint(); break;
  case 9: item_soccer_bobs(); break;
  case 10: item_bump(); break;
  case 11: item_bouncing_bubble(); break;
  case 12: item_virtual_landscape(); break;
  case 13: item_plasma(); break;
  case 14: item_oriented_convolutions(); break;
  case 15: item_shade_bobs(); break;
  case 16: item_fourier_filtering(); break;
  case 17: item_image_zoomer(); break;
  case 18: item_blobs_editor(); break;
  case 19: item_double_torus(); break;
  case 20: item_3d_metaballs(); break;
  case 21: item_fireworks(); break;
  case 22: item_rubber_logo(); break;
  case 23: item_image_waves(); break;
  case 24: item_breakout(); break;
  case 25: item_3d_reflection(); break;
  case 26: item_fisheye_magnification(); break;
  case 27: item_word_puzzle(); break;
  default: break;
  }
}

/*---------------------------

  Main procedure

  --------------------------*/
int main(int argc, char **argv) {

  // Display info about the CImg Library configuration
  //--------------------------------------------------
  unsigned int demo_number = cimg_option("-run",0,0);
  if (demo_number) start_item(demo_number);
  else {
    cimg::info();

    // Demo selection menu
    //---------------------
    const unsigned char
      white[]  = { 255, 255, 255 }, black[] = { 0, 0, 0 },     red[] = { 120, 50, 80 },
      yellow[] = { 200, 155, 0 },   green[] = { 30, 200, 70 }, purple[] = { 175, 32, 186 },
      blue[]   = { 55, 140, 185 },  grey[] = { 127, 127, 127 };
    float
      rx = 0, ry = 0, t = 0, gamma = 0, vgamma = 0, T = 0.9f,
      nrx = (float)(2*cimg::crand()),
      nry = (float)(2*cimg::crand());
    int y0 = 2*13;
    CImg<unsigned char> back(1,2,1,3,10), fore, text, img;
    back.fillC(0,1,0,10,10,235).resize(320,420,1,3,3).get_shared_channel(2).noise(10,1).draw_plasma();
    back.draw_rectangle(0,y0-7,back.width()-1,y0+20,red);
    fore.assign(back.width(),50,1,1,0).draw_text(20,y0-3,"** CImg %u.%u.%u Samples **",grey,0,1,23,
                                                cimg_version/100,(cimg_version/10)%10,cimg_version%10);
    (fore+=fore.get_dilate(3).dilate(3)).resize(-100,-100,1,3);
    cimg_forXY(fore,x,y)
      if (fore(x,y)==127) fore(x,y,0) = fore(x,y,1) = fore(x,y,2) = 1;
      else if (fore(x,y)) {
        const float val = cimg::min(255.0f,7.0f*(y-3));
        fore(x,y,0) = (unsigned char)(val/1.5f);
        fore(x,y,1) = (unsigned char)val;
        fore(x,y,2) = (unsigned char)(val/1.1f);
      }
    text.draw_text(1,1,
                   "1- Blurring Gradient\n"
                   "2- Rotozoom\n"
                   "3- Anisotropic Smoothing\n"
                   "4- Fractal Animation\n"
                   "5- Gamma Correction\n"
                   "6- Filled Triangles\n"
                   "7- Mandelbrot explorer\n"
                   "8- Mini-Paint\n"
                   "9- Soccer Bobs\n"
                   "10- Bump Effect\n"
                   "11- Bouncing Bubble\n"
                   "12- Virtual Landscape\n"
                   "13- Plasma & Sinus Scroll\n"
                   "14- Oriented Convolutions\n"
                   "15- Shade Bobs\n"
                   "16- Fourier Filtering\n"
                   "17- Image Zoomer\n"
                   "18- Blobs Editor\n"
                   "19- Double Torus\n"
                   "20- 3D Metaballs\n"
                   "21- Fireworks\n"
                   "22- Rubber Logo\n"
                   "23- Image Waves\n"
                   "24- Breakout\n"
                   "25- 3D Reflection\n"
                   "26- Fish-Eye Magnification\n"
                   "27- Word Puzzle\n",
                   white,0,1,13).resize(-100,-100,1,3);
    fore.resize(back,0).draw_image(20,y0+2*13,text|=text.get_dilate(3)>>4);

    CImgDisplay disp(back,"CImg Library Samples",0,false,true);
    disp.move((disp.screen_width()-disp.window_width())/2,(disp.screen_height()-disp.window_height())/2);
    img = back; back*=0.15f;
    for (y0+=2*13; !disp.is_closed() && !disp.is_keyQ() && !disp.is_keyESC(); demo_number = 0) {
      while (!demo_number && !disp.is_closed() && !disp.is_keyQ() && !disp.is_keyESC()) {
        img*=0.85f; img+=back;
        for (int i = 0; i<60; ++i) {
          const float
            mx = (float)(img.width()/2+(img.width()/2-30)*((1-gamma)*std::cos(3*t+rx*i*18.0f*cimg::PI/180) +
                                                         gamma*std::cos(3*t+nrx*i*18.0f*cimg::PI/180))),
            my = (float)(img.height()/2+(img.height()/2-30)*((1-gamma)*std::sin(4*t+ry*i*18.0f*cimg::PI/180) +
                                                         gamma*std::sin(4*t+nry*i*18.0f*cimg::PI/180))),
            mz = (float)(1.3f + 1.2f*((1-gamma)*std::sin(2*t+(rx+ry)*i*20*cimg::PI/180) +
                                      gamma*std::sin(2*t+(nrx+nry)*i*20*cimg::PI/180)));
          const int j = i%5;
          img.draw_circle((int)mx,(int)my,(int)(10*mz),j!=0?(j!=1?(j!=2?(j!=3?green:red):yellow):purple):blue,0.2f).
            draw_circle((int)(mx+4*mz),(int)(my-4),(int)(3*mz),white,0.1f).
            draw_circle((int)mx,(int)my,(int)(10*mz),black,0.2f,~0U);
        }
        const unsigned char *ptrs = fore.data();
        cimg_for(img,ptrd,unsigned char) { const unsigned char val = *(ptrs++); if (val) *ptrd = val; }
        int y = disp.mouse_y();
        if (y>=y0 && y<y0+27*13) {
          y = (y/13)*13+7;
          for (int yy = y-6; yy<=y+6; ++yy) img.draw_rectangle(0,yy,0,1,img.width()-1,yy,0,1,(unsigned char)(130-15*cimg::abs(yy-y)));
          img.draw_triangle(2,y-4,2,y+4,8,y,yellow).draw_triangle(img.width()-2,y-4,img.width()-2,y+4,img.width()-8,y,yellow);
        }
        gamma+=vgamma; if (gamma>1) { gamma = vgamma = 0; rx = nrx; ry = nry; nrx=(float)(2*cimg::crand()); nry=(float)(2*cimg::crand()); }
        t+=0.006f; T+=0.005f; if (T>1) { T-=(float)(1+cimg::crand()); vgamma = 0.03f; }
        if (disp.button()) { demo_number = 1+(disp.mouse_y()-y0)/13; disp.set_button(); }
        disp.resize(disp,false).display(img).wait(25);
      }
      start_item(demo_number);
    }
  }

  // Exit demo
  //-----------
  std::exit(0);
  return 0;
}
