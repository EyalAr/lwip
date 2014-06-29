/*
 #
 #  File        : image_surface3d.cpp
 #                ( C++ source file )
 #
 #  Description : This tool allows to show an image as a 3D surface.
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
#ifndef cimg_imagepath
#define cimg_imagepath "img/"
#endif

// Main procedure
//----------------
int main(int argc,char **argv) {

  // Read command line arguments.
  cimg_usage("Render an image as a surface");
  const char *file_i    = cimg_option("-i",cimg_imagepath "logo.bmp","Input image");
  const char *file_o    = cimg_option("-o",(char*)0,"Output 3D object");
  const float sigma     = cimg_option("-smooth",1.0f,"Amount of image smoothing");
  const float ratioz    = cimg_option("-z",0.25f,"Aspect ratio along z-axis");
  const unsigned int di = cimg_option("-di",10,"Step for isophote skipping");

  // Load 2D image file.
  std::fprintf(stderr,"\n- Load file '%s'",cimg::basename(file_i)); std::fflush(stderr);
  const CImg<unsigned char>
    img  = CImg<>(file_i).blur(sigma).resize(-100,-100,1,3),
    norm = img.get_norm().normalize(0,255);

  // Compute surface with triangles.
  std::fprintf(stderr,"\n- Create image surface"); std::fflush(stderr);
  CImgList<unsigned int> primitives;
  CImgList<unsigned char> colors;
  const CImg<> points = img.get_elevation3d(primitives,colors,norm*-ratioz);

  // Compute image isophotes.
  std::fprintf(stderr,"\n- Compute image isophotes"); std::fflush(stderr);
  CImgList<unsigned int> isoprimitives;
  CImgList<unsigned char> isocolors;
  CImg<> isopoints;
  for (unsigned int i = 0; i<255; i+=di) {
    CImgList<> prims;
    const CImg<> pts = norm.get_isoline3d(prims,(float)i);
    isopoints.append_object3d(isoprimitives,pts,prims);
  }
  cimglist_for(isoprimitives,l) {
    const unsigned int i0 = isoprimitives(l,0);
    const float x0 = isopoints(i0,0), y0 = isopoints(i0,1);
    const unsigned char
      r = (unsigned char)img.linear_atXY(x0,y0,0),
      g = (unsigned char)img.linear_atXY(x0,y0,1),
      b = (unsigned char)img.linear_atXY(x0,y0,2);
    isocolors.insert(CImg<unsigned char>::vector(r,g,b));
  }
  cimg_forX(isopoints,ll) isopoints(ll,2) = -ratioz*norm.linear_atXY(isopoints(ll,0),isopoints(ll,1));

  // Save object if necessary
  if (file_o) {
    std::fprintf(stderr,"\n- Save 3d object as '%s'",cimg::basename(file_o)); std::fflush(stderr);
    points.save_off(primitives,colors,file_o);
  }

  // Enter event loop
  std::fprintf(stderr,
               "\n- Enter interactive loop.\n\n"
               "Reminder : \n"
               " + Use mouse to rotate and zoom object\n"
               " + key 'F'          : Toggle fullscreen\n"
               " + key 'Q' or 'ESC' : Quit\n"
               " + Any other key    : Change rendering type\n\n"); std::fflush(stderr);
  const char *const title = "Image viewed as a surface";
  CImgDisplay disp(800,600,title,0);
  unsigned int rtype = 2;
  CImg<float> pose = CImg<float>::identity_matrix(4);

  while (!disp.is_closed()) {
    const unsigned char white[3]={ 255, 255, 255 };
    CImg<unsigned char> visu(disp.width(),disp.height(),1,3,0);
    visu.draw_text(10,10,"%s",white,0,1,24,
                rtype==0?"Points":(rtype==1?"Lines":(rtype==2?"Faces":(rtype==3?"Flat-shaded faces":
               (rtype==4?"Gouraud-shaded faces":(rtype==5?"Phong-shaded faces":"Isophotes"))))));
    static bool first_time = true;
    if (rtype==6) visu.display_object3d(disp,isopoints,isoprimitives,isocolors,first_time,1,-1,true,
                                        500.0f,0.0f,0.0f,-5000.0f,0.0f,0.0f,true,pose.data());
    else visu.display_object3d(disp,points,primitives,colors,first_time,rtype,-1,true,
                               500.0f,0.0f,0.0f,-5000.0f,0.0f,0.0f,true,pose.data());
    first_time = false;
    switch (disp.key()) {
    case 0: break;
    case cimg::keyBACKSPACE: rtype = (7+rtype-1)%7; break;
    case cimg::keyQ:
    case cimg::keyESC: disp.close(); break;
    case cimg::keyF:
      if (disp.is_fullscreen()) disp.resize(800,600); else disp.resize(disp.screen_width(),disp.screen_height());
      disp.toggle_fullscreen();
      break;
    default: rtype = (rtype+1)%7; break;
    }
  }

  return 0;
}
