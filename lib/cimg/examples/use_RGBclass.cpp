/*
 #
 #  File        : use_RGBclass.cpp
 #                ( C++ source file )
 #
 #  Description : A small code that shows how to write a CImg plugin to
 #                handle color image manipulation using a user-defined RGB
 #                class, instead of using classical pixel access of CImg<T>
 #                with operator().
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

#ifndef cimg_plugin
#define cimg_plugin "examples/use_RGBclass.cpp"  // Path of the plugin is relative to the CImg.h file.
#include "CImg.h"
using namespace cimg_library;
#ifndef cimg_imagepath
#define cimg_imagepath "img/"
#endif

// Main procedure
//----------------
int main() {

  // Load images.
  CImg<short> img1(cimg_imagepath "milla.bmp");
  const CImg<float> img2 = CImg<float>(cimg_imagepath "lena.pgm").resize(img1,3);
  const float default_color[] = { 30,30,80 };

  // Modify 'img1' using the RGB pixel accessor.
  cimg_forXY(img1,x,y)
    if (!((x*y)%31)) img1.RGB_at(x,y) = default_color;
    else if ((x+y)%2) img1.RGB_at(x,y) = img2.RGB_at(x,y);
  img1.display();

  // Quit.
  return 0;
}

#else

//-------------------------
// Start of the plugin code
//-------------------------

// Define a simple structure of *references* to R,G,B values.
//-----------------------------------------------------------
// (Feel free to add your own operators in there !)
struct st_RGB {
  T _R,_G,_B,&R,&G,&B;

  // Construct from R,G,B references of values.
  st_RGB(const T& nR, const T& nG, const T& nB):_R(nR),_G(nG),_B(nB),R(_R),G(_G),B(_B) {}
  st_RGB(T& nR, T& nG, T& nB):R(nR),G(nG),B(nB) {}

  // Copy constructors.
  st_RGB(const st_RGB& rgb):_R(rgb.R),_G(rgb.G),_B(rgb.B),R(_R),G(_G),B(_B) {}
  template<typename t>
  st_RGB(const t& rgb):_R(rgb[0]),_G(rgb[1]),_B(rgb[2]) {}

  // Assignement operator.
  st_RGB& operator=(const st_RGB& rgb) {
    R = (T)(rgb[0]); G = (T)(rgb[1]); B = (T)(rgb[2]);
    return *this;
  }
  template<typename t>
  st_RGB& operator=(const t& rgb) {
    R = (T)(rgb[0]); G = (T)(rgb[1]); B = (T)(rgb[2]);
    return *this;
  }

  // Data (R,G or B) access operator.
  const T& operator[](const unsigned int i) const {
    return i==2?B:(i==1?G:R);
  }
  T& operator[](const unsigned int i) {
    return i==2?B:(i==1?G:R);
  }

  // Print instance on the standard error.
  const st_RGB& print() const {
    std::fprintf(stderr,"{ %d %d %d }\n",(int)R,(int)G,(int)B);
    return *this;
  }
};

// Define CImg<T> member functions which return pixel values as st_RGB instances.
//--------------------------------------------------------------------------------
const st_RGB RGB_at(const int x, const int y=0, const int z=0) const {
  const int whz = width()*height()*depth();
  const T *const pR = data() + x + y*width() + z*width()*height(), *const pG = pR + whz, *const pB = pG + whz;
  return st_RGB(*pR,*pG,*pB);
}

st_RGB RGB_at(const int x, const int y=0, const int z=0) {
  const int whz = width()*height()*depth();
  T *const pR = data() + x + y*width() + z*width()*height(), *const pG = pR + whz, *const pB = pG + whz;
  return st_RGB(*pR,*pG,*pB);
}

//------------------------
// End of the plugin code
//------------------------
#endif
