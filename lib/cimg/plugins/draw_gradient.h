/*
 #
 #  File        : draw_gradient.h
 #                ( C++ header file - CImg plug-in )
 #
 #  Description : Plugin that can be used to draw color gradient on images.
 #                This file is a part of the CImg Library project.
 #                ( http://cimg.sourceforge.net )
 #
 #  Copyright   : Jerome Boulanger
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

#ifndef cimg_plugin_draw_gradient
#define cimg_plugin_draw_gradient

// Convert the couple (shape,profile) into a description string
static inline const char *get_gradient_str(const int shape, const int profile) {
  static char buf[128];
  switch(shape) {
  case 0: std::sprintf(buf,"linear shape and"); break;
  case 1: std::sprintf(buf,"spheric shape and"); break;
  case 2: std::sprintf(buf,"conic shape and"); break;
  case 3: std::sprintf(buf,"square shape and"); break;
  case 4: std::sprintf(buf,"rectangle (L1) shape and"); break;
  case 5: std::sprintf(buf,"rectangle (Linf) shape and"); break;
  case 6: std::sprintf(buf,"Gaussian shape and"); break;
  default: std::sprintf(buf,"undefined shape and"); break;
  }
  switch(profile) {
  case 0: std::strcat(buf," linear profile"); break;
  case 1: std::strcat(buf," wave profile"); break;
  case 2: std::strcat(buf," ring/bar profile"); break;
  case 3: std::strcat(buf," exponential"); break;
  case 4: std::strcat(buf," vanishing wave profile"); break;
  case 5: std::strcat(buf," vanishing ring/bar profile"); break;
  case 6: std::strcat(buf," circ diffraction (Airy) profile"); break;
  case 7: std::strcat(buf," rect diffraction (sinc2) profile"); break;
  default: std::strcat(buf," undefined profile"); break;
  }
  return buf;
}

template<typename tc>
void _draw_gradient_profile(T *const ptr, const float opacity, const float r,
                            const tc *const color0, const tc *const color1,
                            const int profile) {
  const unsigned int id = (color0?1:0) + (color1?2:0);
  const tc col0 = color0?*color0:0, col1 = color1?*color1:0;
  switch(profile) {
  case 0: { // linear
    switch(id) { // map the 3 cases
    case 3: *ptr = (T)((1-opacity)**ptr + opacity*(col0*(1.f-r)+col1*r)); break;
    case 1: if (r<1) *ptr = (T)((1-opacity*(1-r))**ptr + col0*opacity*(1-r)); break;
    case 2: if (r>0) *ptr = (T)((1-opacity*r)**ptr + col1*opacity*r); break;
    default: break;
    }  break;
  }
  case 1: { // waves
    const float f = (1 - (float)std::cos(4.5f*r*2.f*cimg::PI))/2;
    switch(id) { // map the 3 cases
    case 3: *ptr = (T)((1-opacity)**ptr + opacity*(col0*(1.f-f)+col1*f)); break;
    case 1: if (f<1) *ptr = (T)((1-opacity*(1-f))**ptr + col0*opacity*(1-f)); break;
    case 2: if (f>0) *ptr = (T)((1-opacity*f)**ptr + col1*opacity*f); break;
    default: break;
    } break;
  }
  case 2:{ // ring/bar
    const float f = (1 + (float)std::cos(r*2.f*cimg::PI))/2;
    switch(id) { // map the 3 cases
    case 3: *ptr = (T)((1-opacity)**ptr + opacity*(col0*(1.f-f)+col1*f)); break;
    case 1: if (f<1) *ptr = (T)((1-opacity*(1-f))**ptr + col0*opacity*(1-f)); break;
    case 2: if (f>0) *ptr = (T)((1-opacity*f)**ptr + col1*opacity*f); break;
    default: break;
    } break;
  }
  case 3: { // exponential
    const float f = 1 - (float)std::exp(-r);
    switch(id) { // map the 3 cases
    case 3: *ptr = (T)((1-opacity)**ptr + opacity*(col0*(1.f-f)+col1*f)); break;
    case 1: if (f<1) *ptr = (T)((1-opacity*(1-f))**ptr + col0*opacity*(1-f)); break;
    case 2: if (f>0) *ptr = (T)((1-opacity*f)**ptr + col1*opacity*f); break;
    default: break;
    } break;
  }
  case 4: { // vanishing wave
    const float f = (1 - (float)std::cos(4.5f*r*2.f*cimg::PI))/2, o = r<.9f?(float)std::exp(-.5*r*r*12.f):0;
    switch(id) { // map the 3 cases
    case 3: if (o>0) *ptr = (T)((1-o)**ptr + o*(col0*(1.f-f)+col1*f)); break;
    case 1: if (f<1) *ptr = (T)((1-o*(1-f))**ptr + col0*o*(1-f)); break;
    case 2: if (f>0) *ptr = (T)((1-o*f)**ptr + col1*o*f); break;
    default: break;
    } break;
  }
  case 5: { // vanishing ring/bar
    const float f = (1 + (float)std::cos(r*2.f*cimg::PI))/2, o = r<.9?(float)std::exp(-.5*r*r*12.f):0;
    switch(id) { // map the 3 cases
    case 3: if (o>0) *ptr = (T)((1-o)**ptr + o*(col0*(1.f-f)+col1*f)); break;
    case 1: if (f<1) *ptr = (T)((1-o*(1-f))**ptr + col0*o*(1-f)); break;
    case 2: if (f>0) *ptr = (T)((1-o*f)**ptr + col1*o*f); break;
    default: break;
    } break;
  }
  case 6: { // diffraction pattern of a circular aperture (Airy function)
#define myj1(x) (std::sin((x)<3?(x)*2.2/3:(x)-0.8)*std::exp(-std::pow((x)/5.0,1/3.0)))
    const float a = 10*(float)cimg::PI*r, tmp = a<0.2?.5f:((float)myj1(a)/a), f = 1-4*tmp*tmp;
#undef myj1
    switch(id) { // map the 3 cases
    case 3: *ptr = (T)((1-opacity)**ptr + opacity*(col0*(1.f-f)+col1*f)); break;
    case 1: if (f<1) *ptr = (T)((1-opacity*(1-f))**ptr + col0*opacity*(1-f)); break;
    case 2: if (f>0) *ptr = (T)((1-opacity*f)**ptr + col1*opacity*f); break;
    default: break;
    }
    break;
  }
  case 7: { // diffraction pattern of a rectangular function (sinc function)
    const float a = 10*(float)cimg::PI*r, tmp = a==0?1:(float)std::sin(a)/a, f = 1-tmp*tmp;
    switch(id) { // map the 3 cases
    case 3: *ptr = (T)((1-opacity)**ptr + opacity*(col0*(1.f-f)+col1*f)); break;
    case 1: if (f<1) *ptr = (T)((1-opacity*(1-f))**ptr + col0*opacity*(1-f)); break;
    case 2: if (f>0) *ptr = (T)((1-opacity*f)**ptr + col1*opacity*f); break;
    default: break;
    } break;
  }
  default:
    CImgArgumentException("CImg<%s>::draw_gradient : unknown profile parameter",pixel_type()); break;
  }
}

//! Draw a gradient with various shape and profile
/**
   \param x0 X-coordinate of the 1st control point
   \param y0 Y-coordinate of the 1st control point
   \param x1 X-coordinate of the 2nd control point
   \param y1 Y-coordinate of the 2nd control point
   \param color0 Array of dimv() values of type \c T, defining the 1st color.
   \param color1 Array of dimv() values of type \c T, defining the 2nd color.
   \param shape shape of the gradient (0,3)
   \param profile  select a profile function (0,7)
   \param opacity Drawing opacity.
   \note
   - if one color is NULL then the gradient is done to transparency
**/
template<typename tc>
CImg<T>& draw_gradient(const int x0, const int y0, const int x1, const int y1,
                       const tc *const color0, const tc *const color1,
                       const int shape=0, const int profile=0, const float opacity=1.0f){
  if (is_empty()) return *this;
  if (!color0 && !color1)
    throw CImgArgumentException("CImg<%s>::draw_gradient : The two specified colors are (null).",
                          pixel_type());
  if (profile<0 || profile>7) { // catch this case before entering in the for loop
    CImgArgumentException("CImg<%s>::draw_gradient : unknown profile parameter",pixel_type());
    return *this;
  }
  const float abx = (float)x1-x0, aby = (float)y1-y0, ab2 = abx*abx + aby*aby; // pt A=(x0,y0), B=(x1,y1)
  const tc *pcol0 = color0, *pcol1 = color1;
  T *ptr = data();

  switch(shape) {
  case 0: { // linear
    cimg_forC(*this,v) { cimg_forXYZ(*this,x,y,z) { // point M=(x,z)
      const float amx = (float)x-x0, amy = (float)y-y0, r = cimg::max(0.f,cimg::min(1.f,(amx*abx+amy*aby)/ab2));
      _draw_gradient_profile(ptr++,opacity,r,pcol0,pcol1,profile);
    } if (pcol0) ++pcol0; if (pcol1) ++pcol1; }} break;
  case 1:{ // radial
    cimg_forC(*this,v) { cimg_forXYZ(*this,x,y,z) {
      const float amx = (float)x-x0, amy = (float)y-y0, r = cimg::max(0.f,cimg::min(1.f,(amx*amx+amy*amy)/ab2));
      _draw_gradient_profile(ptr++,opacity,r,pcol0,pcol1,profile);
     } if (pcol0) ++pcol0; if (pcol1) ++pcol1; }} break;
  case 2:{ // radial cone
    cimg_forC(*this,v) { cimg_forXYZ(*this,x,y,z) {
      const float amx = (float)x-x0, amy = (float)y-y0, r = cimg::max(0.f,cimg::min(1.f,(float)std::sqrt((amx*amx+amy*amy)/ab2)));
      _draw_gradient_profile(ptr++,opacity,r,pcol0,pcol1,profile);
    } if (pcol0) ++pcol0; if (pcol1) ++pcol1; }} break;
  case 3:{ // square
    cimg_forC(*this,v) { cimg_forXYZ(*this,x,y,z) {
      const float amx = (float)x-x0, amy = (float)y-y0, r=cimg::max(0.f,cimg::min(1.f,(cimg::abs(amx*abx+amy*aby)+cimg::abs(amx*aby-amy*abx))/ab2));
      _draw_gradient_profile(ptr++,opacity,r,pcol0,pcol1,profile);
    } if (pcol0) ++pcol0; if (pcol1) ++pcol1; }} break;
  case 4:{ // rectangle (L1)
    cimg_forC(*this,v) { cimg_forXYZ(*this,x,y,z) {
      const float amx = (float)x-x0, amy = (float)y-y0,
        r = cimg::max(0.f,cimg::min(1.f,(cimg::abs(amx/abx)+cimg::abs(amy/aby))));
      _draw_gradient_profile(ptr++,opacity,r,pcol0,pcol1,profile);
    } if (pcol0) ++pcol0; if (pcol1) ++pcol1; }} break;
   case 5:{ // rectangle (Linf)
    cimg_forC(*this,v) { cimg_forXYZ(*this,x,y,z) {
      const float amx = (float)x-x0, amy = (float)y-y0,
        r=cimg::max(0.f,cimg::min(1.f,cimg::max(cimg::abs(amx/abx),cimg::abs(amy/aby))));
      _draw_gradient_profile(ptr++,opacity,r,pcol0,pcol1,profile);
    } if (pcol0) ++pcol0; if (pcol1) ++pcol1; }} break;
  case 6:{ // gaussian
    cimg_forC(*this,v) { cimg_forXYZ(*this,x,y,z) {
      const float amx = (float)x-x0, amy = (float)y-y0, r = cimg::max(0.f,cimg::min(1.f,1-(float)std::exp(-(amx*amx+amy*amy)/ab2)));
      _draw_gradient_profile(ptr++,opacity,r,pcol0,pcol1,profile);
    } if (pcol0) ++pcol0; if (pcol1) ++pcol1; }} break;
  default:
    CImgArgumentException("CImg<%s>::draw_gradient : unknown shape parameter",pixel_type()); break;
  }
  return *this;
}

template<typename tc>
CImg<T>& draw_gradient(const int x0, const int y0, const int x1, const int y1,
                       const tc *const color0, const int color1,
                       const int shape=0, const int profile=0, const float opacity=1.0f) {
  cimg::unused(color1);
  return (*this).draw_gradient(x0,y0,x1,y1,color0,(tc*)0,shape,profile,opacity);
}

template<typename tc>
CImg<T>& draw_gradient(const int x0, const int y0, const int x1, const int y1,
                       const int color0, const tc *const color1,
                       const int shape=0, const int profile=0, const float opacity=1.0f) {
  cimg::unused(color0);
  return (*this).draw_gradient(x0,y0,x1,y1,(tc*)0,color1,shape,profile,opacity);
}

#endif
