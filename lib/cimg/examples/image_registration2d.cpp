/*
 #
 #  File        : image_registration2d.cpp
 #                ( C++ source file )
 #
 #  Description : Compute a motion field between two images,
 #                with a multiscale and variational algorithm.
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
 #   same conditions as regards security.
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
#undef min
#undef max

// animate_warp() : Create warping animation from two images and a motion field
//----------------
void animate_warp(const CImg<unsigned char>& src, const CImg<unsigned char>& dest, const CImg<>& U,
                  const bool morph, const bool imode, const char *filename,int nb, CImgDisplay& disp) {
  CImg<unsigned char> visu = (src,dest,src)>'x', warp(src);
  float t = 0;
  for (unsigned int iteration = 0; !disp || (!disp.is_closed() && !disp.is_keyQ()); ++iteration) {
    if (morph) cimg_forXYC(warp,x,y,k) {
      const float dx = U(x,y,0), dy = U(x,y,1),
        I1 = (float)src.linear_atXY(x-t*dx, y-t*dy, k),
        I2 = (float)dest.linear_atXY(x+(1-t)*dx,y+(1-t)*dy,k);
      warp(x,y,k) = (unsigned char)((1-t)*I1 + t*I2);
    } else cimg_forXYC(warp,x,y,k) {
      const float dx = U(x,y,0), dy = U(x,y,1), I1 = (float)src.linear_atXY(x-t*dx, y-t*dy, 0,k);
      warp(x,y,k) = (unsigned char)I1;
    }
    if (disp) visu.draw_image(2*src.width(),warp).display(disp.resize().wait(30));
    if (filename && *filename && (imode || (int)iteration<nb)) {
      std::fprintf(stderr,"\r  > frame %d           ",iteration);
      warp.save(filename,iteration);
    }
    t+=1.0f/nb;
    if (t<0) { t = 0; nb = -nb; }
    if (t>1) { t = 1; nb = -nb; if (filename && *filename) std::exit(0); }
  }
}

// optflow() : multiscale version of the image registration algorithm
//-----------
CImg<> optflow(const CImg<>& source, const CImg<>& target,
               const float smoothness, const float precision, const unsigned int nb_scales, CImgDisplay& disp) {
  const unsigned int iteration_max = 100000;
  const float _precision = (float)std::pow(10.0,-(double)precision);
  const CImg<>
    src  = source.get_resize(target,3).normalize(0,1),
    dest = target.get_normalize(0,1);
  CImg<> U;

  const unsigned int _nb_scales = nb_scales>0?nb_scales:(unsigned int)(2*std::log((double)(cimg::max(src.width(),src.height()))));
  for (int scale = _nb_scales-1; scale>=0; --scale) {
    const float factor = (float)std::pow(1.5,(double)scale);
    const unsigned int
      _sw = (unsigned int)(src.width()/factor), sw = _sw?_sw:1,
      _sh = (unsigned int)(src.height()/factor), sh = _sh?_sh:1;
    const CImg<>
      I1 = src.get_resize(sw,sh,1,-100,2),
      I2 = dest.get_resize(I1,2);
    std::fprintf(stderr," * Scale %d\n",scale);
    if (U) (U*=1.5f).resize(I2.width(),I2.height(),1,-100,3);
    else U.assign(I2.width(),I2.height(),1,2,0);

    float dt = 2, energy = cimg::type<float>::max();
    const CImgList<> dI = I2.get_gradient();

    for (unsigned int iteration = 0; iteration<iteration_max; ++iteration) {
      std::fprintf(stderr,"\r- Iteration %d - E = %g",iteration,energy); std::fflush(stderr);
      float _energy = 0;
      cimg_for3XY(U,x,y) {
        const float
          X = x + U(x,y,0),
          Y = y + U(x,y,1);

        float deltaI = 0;
        cimg_forC(I2,c) deltaI+=(float)(I1(x,y,c) - I2.linear_atXY(X,Y,c));

        float _energy_regul = 0;
        cimg_forC(U,c) {
          const float
            Ux  = 0.5f*(U(_n1x,y,c) - U(_p1x,y,c)),
            Uy  = 0.5f*(U(x,_n1y,c) - U(x,_p1y,c)),
            Uxx = U(_n1x,y,c) + U(_p1x,y,c),
            Uyy = U(x,_n1y,c) + U(x,_p1y,c);
          U(x,y,c) = (float)( U(x,y,c) + dt*(deltaI*dI[c].linear_atXY(X,Y) + smoothness* ( Uxx + Uyy )))/(1+4*smoothness*dt);
          _energy_regul+=Ux*Ux + Uy*Uy;
        }
        _energy+=deltaI*deltaI + smoothness*_energy_regul;
      }
      const float d_energy = (_energy - energy)/(sw*sh);
      if (d_energy<=0 && -d_energy<_precision) break;
      if (d_energy>0) dt*=0.5f;
      energy = _energy;
      if (disp) disp.resize();
      if (disp && disp.is_closed()) std::exit(0);
      if (disp && !(iteration%300)) {
        const unsigned char white[] = { 255,255,255 };
        CImg<unsigned char> tmp = I1.get_warp(U,true,true,1).normalize(0,200);
        tmp.resize(disp.width(),disp.height()).draw_quiver(U,white,0.7f,15,-14,true).display(disp);
      }
    }
    std::fprintf(stderr,"\n");
  }
  return U;
}

/*------------------------

  Main function

  ------------------------*/

int main(int argc,char **argv) {

  // Read command line parameters
  cimg_usage("Compute an optical flow between two 2D images, and create a warped animation");
  const char
    *name_i1 = cimg_option("-i",cimg_imagepath "sh0r.pgm","Input Image 1 (Destination)"),
    *name_i2 = cimg_option("-i2",cimg_imagepath "sh1r.pgm","Input Image 2 (Source)"),
    *name_o = cimg_option("-o",(const char*)NULL,"Output 2D flow (inrimage)"),
    *name_seq = cimg_option("-o2",(const char*)NULL,"Output Warping Sequence");
  const float
    smoothness = cimg_option("-s",0.1f,"Flow Smoothness"),
    precision = cimg_option("-p",6.0f,"Convergence precision");
  const unsigned int
    nb = cimg_option("-n",40,"Number of warped frames"),
    nb_scales = cimg_option("-scale",0,"Number of scales (0=auto)");
  const bool
    normalize = cimg_option("-equalize",true,"Histogram normalization of the images"),
    morph = cimg_option("-m",true,"Morphing mode"),
    imode = cimg_option("-c",true,"Complete interpolation (or last frame is missing)"),
    dispflag = !cimg_option("-novisu",false,"Visualization");

  // Init images and display
  std::fprintf(stderr," - Init images.\n");
  const CImg<>
    src(name_i1),
    dest(CImg<>(name_i2).resize(src,3)),
    src_blur  = normalize?src.get_blur(0.5f).equalize(256):src.get_blur(0.5f),
    dest_blur = normalize?dest.get_blur(0.5f).equalize(256):dest.get_blur(0.5f);

  CImgDisplay disp;
  if (dispflag) {
    unsigned int w = src.width(), h = src.height();
    const unsigned int dmin = cimg::min(w,h), minsiz = 512;
    if (dmin<minsiz) { w=w*minsiz/dmin; h=h*minsiz/dmin; }
    const unsigned int dmax = cimg::max(w,h), maxsiz = 1024;
    if (dmax>maxsiz) { w=w*maxsiz/dmax; h=h*maxsiz/dmax; }
    disp.assign(w,h,"Estimated Motion",0);
  }

  // Run Motion estimation algorithm
  std::fprintf(stderr," - Compute optical flow.\n");
  const CImg<> U = optflow(src_blur,dest_blur,smoothness,precision,nb_scales,disp);
  if (name_o) U.save(name_o);
  U.print("Computed flow");

  // Do morphing animation
  std::fprintf(stderr," - Create warped animation.\n");
  CImgDisplay disp2;
  if (dispflag) {
    unsigned int w = src.width(), h = src.height();
    const unsigned int dmin = cimg::min(w,h), minsiz = 100;
    if (dmin<minsiz) { w = w*minsiz/dmin; h=h*minsiz/dmin; }
    const unsigned int dmax = cimg::max(w,h), maxsiz = 1024/3;
    if (dmax>maxsiz) { w = w*maxsiz/dmax; h=h*maxsiz/dmax; }
    disp2.assign(3*w,h,"Source/Destination images and Motion animation",0);
  }

  animate_warp(src.get_normalize(0,255),dest.get_normalize(0,255),U,morph,imode,name_seq,nb,disp2);

  std::exit(0);
  return 0;
}
