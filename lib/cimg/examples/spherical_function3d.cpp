/*
 #
 #  File        : spherical_function3d.cpp
 #                ( C++ source file )
 #
 #  Description : An example that shows how to build custom 3D objects in CImg.
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

#include "CImg.h"
using namespace cimg_library;

/*---------------------------

   Main procedure

  --------------------------*/
int main() {

  CImgList<unsigned char> object_colors;
  CImgList<float> object_opacities;

  // Define a 3D centered box.
  CImg<float> object_vertices = CImg<float>(3,8,1,1,  // Define the 8 vertices of the cube.
                                            -1,-1,-1, // (x0,y0,z0)
                                            1,-1,-1,  // (x1,y1,z1)
                                            1,1,-1,   // ...
                                            -1,1,-1,
                                            -1,-1,1,
                                            1,-1,1,
                                            1,1,1,     // (x6,y6,z6)
                                            -1,1,1).transpose(); // (x7,y7,z7)
  CImgList<unsigned int> object_primitives(12,1,2,1,1, // Define the 12 segments of the cube.
                                           0,1, 1,2, 2,3, 3,0,
                                           4,5, 5,6, 6,7, 7,4,
                                           0,4, 1,5, 2,6, 3,7);
  object_colors.insert(object_primitives.size(),CImg<unsigned char>::vector(32,64,255));
  object_opacities.insert(object_primitives.size(),CImg<float>::vector(0.3f));

  // Define the spherical function's vertices.
  CImgList<float> spherical_vertices;
  const float a = 1;
  const unsigned int na = 132, nb = 132;
  for (unsigned int v = 0; v<na; ++v)
    for (unsigned int u = 0; u<nb; ++u) {
      const float
        alpha = (float)(u*2*cimg::PI/nb),
        beta = (float)(-cimg::PI/2 + v*cimg::PI/na),
        x = (float)((a*std::cos(beta))*std::cos(alpha)),
        y = (float)((a*std::cos(beta))*std::sin(alpha)),
        z = (float)(a*std::sin(beta)),
        altitude = cimg::abs(1 + 0.8f*std::cos(3*alpha)*std::sin(4*beta));
      spherical_vertices.insert(CImg<float>::vector(altitude*x,altitude*y,altitude*z));
    }

  // Define the spherical function's mesh.
  CImgList<unsigned int> spherical_primitives;
  for (unsigned int vv = 0; vv<na-1; ++vv)
    for (unsigned int uu = 0; uu<nb; ++uu) {
      const int nv = (vv+1)%na, nu = (uu+1)%nb;
      spherical_primitives.insert(CImg<unsigned int>::vector(nb*vv+nu,nb*nv+uu,nb*vv+uu));
      spherical_primitives.insert(CImg<unsigned int>::vector(nb*vv+nu,nb*nv+nu,nb*nv+uu));
      object_colors.insert(CImg<>::vector(0,255,255));
      object_colors.insert(CImg<>::vector(100,200,255));
      object_opacities.insert(2,CImg<>::vector(1));
    }

  // Merge 3D objects together.
  object_vertices.append_object3d(object_primitives,spherical_vertices>'x',spherical_primitives);
  char title[4096] = { 0 };
  std::sprintf(title,"3D Spherical Function (%u vertices, %u primitives)",object_vertices.width(),object_primitives.size());
  CImgDisplay disp(640,480,title,0);
  CImg<unsigned char>(disp.width(),disp.height(),1,3,220).
    display_object3d(disp,object_vertices,object_primitives,object_colors,object_opacities,true,4,3,false,
                     500,0,0,-5000,0.1f,1.5f);

  return 0;
}
