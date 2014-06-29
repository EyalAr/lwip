/*
 #
 #  File        : curve_editor2d.cpp
 #                ( C++ source file )
 #
 #  Description : A simple user interface to construct 2D spline curves.
 #                This file is a part of the CImg Library project.
 #                ( http://cimg.sourceforge.net )
 #
 #  Copyright   : David Tschumperle
 #                ( http://tschumperle.users.greyc.fr/ )
 #                Antonio Albiol Colomer
 #                ( http://personales.upv.es/~aalbiol/index-english.html )
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

// Compute distance from a point to a segment.
//---------------------------------------------
float dist_segment(const float x, const float y, const float x1, const float y1, const float x2, const float y2) {
  const float
    dx = x2 - x1,
    dy = y2 - y1,
    long_segment = (float)std::sqrt(dx*dx + dy*dy);
  if (long_segment==0) { const float ddx = x - x1, ddy = y - y1; return (float)std::sqrt(ddx*ddx + ddy*ddy); }
  const float
    unitx = dx/long_segment,
    unity = dy/long_segment,
    vx = x - x1,
    vy = y - y1,
    long_proy = vx*unitx + vy*unity,
    proyx = x1 + long_proy*unitx,
    proyy = y1 + long_proy*unity;
  if (long_proy>long_segment) { const float ddx = x - x2, ddy = y - y2; return std::sqrt(ddx*ddx+ddy*ddy); }
  else if (long_proy<0) { const float ddx = x - x1, ddy = y - y1; return std::sqrt(ddx*ddx+ddy*ddy); }
  const float ddx = x - proyx, ddy = y - proyy;
  return std::sqrt(ddx*ddx+ddy*ddy);
}

// Main procedure
//---------------
int main(int argc, char **argv) {

  // Read command line parameters
  //-----------------------------
  cimg_usage("2D Spline Curve Editor");
  const char *file_i = cimg_option("-i",(char*)0,"Input image");
  const float contrast = cimg_option("-contrast",0.6f,"Image contrast");
  const char *file_ip = cimg_option("-ip",(char*)0,"Input control points");
  const char *file_oc = cimg_option("-oc",(char*)0,"Output curve points");
  const char *file_op = cimg_option("-op",(char*)0,"Output control points");
  const char *file_od = cimg_option("-od",(char*)0,"Output distance function");
  bool interp = cimg_option("-poly",true,"Use polynomial interpolation");
  bool closed = cimg_option("-closed",true,"Closed curve");
  bool show_tangents = cimg_option("-tangents",false,"Show tangents");
  bool show_points = cimg_option("-points",true,"Show control points");
  bool show_outline = cimg_option("-outline",true,"Show polygon outline");
  bool show_indices = cimg_option("-indices",true,"Show points indices");
  bool show_coordinates = cimg_option("-coords",false,"Show points coordinates");
  const float precision = cimg_option("-prec",0.05f,"Precision of curve discretization");

  // Init image data
  //-----------------
  const unsigned char yellow[] = { 255,255,0 }, white[] = { 255,255,255 }, green[] = { 0,255,0 },
                      blue[] = { 120,200,255 }, purple[] = { 255,100,255 }, black[] = { 0,0,0 };
  CImg<unsigned char> img0, img, help_img;
  if (file_i) {
    std::fprintf(stderr,"\n - Load input image '%s' : ",cimg::basename(file_i));
    img0 = CImg<>(file_i).normalize(0,255.0f*contrast);
    std::fprintf(stderr,"Size = %dx%dx%dx%d \n",img0.width(),img0.height(),img0.depth(),img0.spectrum());
    img0.resize(-100,-100,1,3);
  }
  else {
    std::fprintf(stderr,"\n - No input image specified, use default 512x512 image.\n");
    img0.assign(512,512,1,3,0).draw_grid(32,32,0,0,false,false,green,0.4f,0xCCCCCCCC,0xCCCCCCCC);
  }

  help_img.assign(220,210,1,3,0).
    draw_text(5,5,
              "------------------------------------------\n"
              "2D Curve Editor\n"
              "------------------------------------------\n"
              "Left button : Create or move control point\n"
              "Right button : Delete control point\n"
              "Spacebar : Switch interpolation\n"
              "Key 'C' : Switch open/closed mode\n"
              "Key 'T' : Show/hide tangents\n"
              "Key 'P' : Show/hide control points\n"
              "Key 'O' : Show/hide polygon outline\n"
              "Key 'N' : Show/hide points indices\n"
              "Key 'X' : Show/hide points coordinates\n"
              "Key 'H' : Show/hide this help\n"
              "Key 'S' : Save control points\n"
              "Key 'R' : Reset curve\n",
              green);
  CImgDisplay disp(img0,"2D Curve Editor",0);
  CImgList<float> points, curve;
  bool moving = false, help = !file_i;

  if (file_ip) {
    std::fprintf(stderr," - Load input control points '%s' : ",cimg::basename(file_ip));
    points = CImg<>(file_ip).transpose()<'x';
    std::fprintf(stderr," %u points\n",points.size());
  }

  // Enter interactive loop
  //------------------------
  while (!disp.is_closed() && !disp.is_keyESC() && !disp.is_keyQ()) {

    // Handle mouse manipulation
    //---------------------------
    const unsigned int button = disp.button();
    const float
      x = disp.mouse_x()*(float)img0.width()/disp.width(),
      y = disp.mouse_y()*(float)img0.height()/disp.height();

    if (points && button && x>=0 && y>=0) {

      // Find nearest point and nearest segment
      float dmin_pt = cimg::type<float>::max(), dmin_seg = dmin_pt;
      unsigned int p_pt = 0, p_seg = 0;
      cimglist_for(points,p) {
        const unsigned int
          pnext = closed?(p+1)%points.size():(p+1<(int)points.size()?p+1:p);
        const float
          xp = points(p,0),
          yp = points(p,1);
        const float
          d_pt  = (xp-x)*(xp-x) + (yp-y)*(yp-y),
          d_seg = dist_segment(x,y,xp,yp,points(pnext,0),points(pnext,1));
        if (d_pt<dmin_pt)   { dmin_pt = d_pt; p_pt = p; }
        if (d_seg<dmin_seg) { dmin_seg = d_seg; p_seg = p; }
      }

      // Handle button
      if (button&1) {
        if (dmin_pt<100 || moving) { points(p_pt,0) = x; points(p_pt,1) = y; }
        else points.insert(CImg<>::vector(x,y),p_seg+1);
        moving = true;
      }
      if (button&2 && dmin_pt<100) {
        if (points.size()>3) points.remove(p_pt);
        disp.set_button();
      }
    }
    if (!button) moving = false;

    if (disp.key()) {
      switch (disp.key()) {
      case cimg::keySPACE : interp = !interp; break;
      case cimg::keyC : closed = !closed; break;
      case cimg::keyT : show_tangents = !show_tangents; break;
      case cimg::keyP : show_points = !show_points; break;
      case cimg::keyO : show_outline = !show_outline; break;
      case cimg::keyN : show_indices = !show_indices; break;
      case cimg::keyX : show_coordinates = !show_coordinates; break;
      case cimg::keyR : points.assign(); break;
      case cimg::keyH : help = !help; break;
      case cimg::keyS : {
        const char *filename = file_op?file_op:"curve_points.dlm";
        std::fprintf(stderr," - Save control points in '%s'\n",filename);
        (points>'x').transpose().save(filename);
      } break;
      }
      disp.set_key();
    }

    // Init list of points if empty
    //------------------------------
    if (!points) {
      const float
        x0 = img0.width()/4.0f,
        y0 = img0.height()/4.0f,
        x1 = img0.width() - x0,
        y1 = img0.height() - y0;
      points.insert(CImg<>::vector(x0,y0)).
        insert(CImg<>::vector(x1,y0)).
        insert(CImg<>::vector(x1,y1)).
        insert(CImg<>::vector(x0,y1));
    }

    // Estimate curve tangents
    //-------------------------
    CImg<> tangents(points.size(),2);
    cimglist_for(points,p) {
      const unsigned int
        p0 = closed?(p+points.size()-1)%points.size():(p?p-1:0),
        p1 = closed?(p+1)%points.size():(p+1<(int)points.size()?p+1:p);
      const float
        x  = points(p,0),
        y  = points(p,1),
        x0 = points(p0,0),
        y0 = points(p0,1),
        x1 = points(p1,0),
        y1 = points(p1,1),
        u0 = x-x0,
        v0 = y-y0,
        n0 = 1e-8f + (float)std::sqrt(u0*u0 + v0*v0),
        u1 = x1-x,
        v1 = y1-y,
        n1 = 1e-8f + (float)std::sqrt(u1*u1 + v1*v1),
        u = u0/n0 + u1/n1,
        v = v0/n0 + v1/n1,
        n = 1e-8f + (float)std::sqrt(u*u + v*v),
        fact = 0.5f*(n0 + n1);
      tangents(p,0) = fact*u/n;
      tangents(p,1) = fact*v/n;
    }

    // Estimate 3th-order polynomial interpolation
    //---------------------------------------------
    curve.assign();
    const unsigned int pmax = points.size()-(closed?0:1);
    for (unsigned int p0 = 0; p0<pmax; p0++) {
      const unsigned int
        p1 = closed?(p0+1)%points.size():(p0+1<points.size()?p0+1:p0);
      const float
        x0 = points(p0,0),
        y0 = points(p0,1),
        x1 = points(p1,0),
        y1 = points(p1,1);
      float ax = 0, bx = 0, cx = 0, dx = 0, ay = 0, by = 0, cy = 0, dy = 0;
      if (interp) {
        const float
          u0 = tangents(p0,0),
          v0 = tangents(p0,1),
          u1 = tangents(p1,0),
          v1 = tangents(p1,1);
        ax = 2*(x0 - x1) + u0 + u1;
        bx = 3*(x1 - x0) - 2*u0 - u1;
        cx = u0;
        dx = x0;
        ay = 2*(y0 - y1) + v0 + v1;
        by = 3*(y1 - y0) - 2*v0 - v1;
        cy = v0;
        dy = y0;
      } else {
        ax = ay = bx = by = 0;
        dx = x0;
        dy = y0;
        cx = x1 - x0;
        cy = y1 - y0;
      }
      const float tmax = 1 + precision;
      for (float t = 0; t<tmax; t+=precision) {
        const float
          xt = ax*t*t*t + bx*t*t + cx*t + dx,
          yt = ay*t*t*t + by*t*t + cy*t + dy;
        curve.insert(CImg<>::vector(xt,yt));
      }
    }

    // Draw curve and display image
    //-------------------------------
    const float
      factx = (float)disp.width()/img0.width(),
      facty = (float)disp.height()/img0.height();
    img = img0.get_resize(disp.width(),disp.height());
    if (help) img.draw_image(help_img,0.6f);
    if (interp && show_outline) {
      CImg<> npoints = points>'x';
      npoints.get_shared_row(0)*=factx;
      npoints.get_shared_row(1)*=facty;
      img.draw_polygon(npoints,blue,0.4f);
      if (closed) img.draw_polygon(npoints,yellow,0.8f,0x11111111);
      else img.draw_line(npoints,yellow,0.8f,0x11111111);
    }
    CImg<> ncurve = curve>'x';
    ncurve.get_shared_row(0)*=factx;
    ncurve.get_shared_row(1)*=facty;
    if (closed) img.draw_polygon(ncurve,white,1.0f,~0U);
    else img.draw_line(ncurve,white);

    if (show_points) cimglist_for(points,p) {
      const float
        x = points(p,0)*factx,
        y = points(p,1)*facty;
      if (show_tangents) {
        const float
          u = tangents(p,0),
          v = tangents(p,1),
          n = 1e-8f + (float)std::sqrt(u*u + v*v),
          nu = u/n,
          nv = v/n;
        img.draw_arrow((int)(x - 15*nu),(int)(y - 15*nv),(int)(x + 15*nu),(int)(y + 15*nv),green);
      }
      if (show_indices) img.draw_text((int)x,(int)(y-16),"%d",purple,black,1,13,p);
      if (show_coordinates) img.draw_text((int)(x-24),(int)(y+8),"(%d,%d)",yellow,black,0.5f,13,(int)points(p,0),(int)points(p,1));
      img.draw_circle((int)x,(int)y,3,blue,0.7f);
    }

    img.display(disp);
    disp.wait();

    if (disp.is_resized()) disp.resize(false);
  }

  // Save output result and exit
  //-----------------------------
  if (file_op) {
    std::fprintf(stderr," - Save control points in '%s'\n",cimg::basename(file_op));
    (points>'x').transpose().save(file_op);
  }
  if (file_oc) {
    std::fprintf(stderr," - Save curve points in '%s'\n",cimg::basename(file_oc));
    (curve>'x').transpose().save(file_oc);
  }
  if (file_od) {
    std::fprintf(stderr," - Computing distance function, please wait...."); std::fflush(stderr);
    CImg<> ncurve = (closed?(+curve).insert(curve[0]):curve)>'x';
    const float zero = 0.0f, one = 1.0f;
    CImg<> distance =
      CImg<>(img0.width(),img0.height(),1,1,-1.0f).draw_line(ncurve,&zero).draw_fill(0,0,&one).
      distance(0);
    std::fprintf(stderr,"\n - Save distance function in '%s'\n",cimg::basename(file_od));
    distance.save(file_od);
  }

  std::fprintf(stderr," - Exit.\n");
  std::exit(0);
  return 0;
}
