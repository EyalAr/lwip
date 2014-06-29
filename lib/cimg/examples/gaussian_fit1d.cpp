/*
 #
 #  File        : gaussian_fit1d.cpp
 #                ( C++ source file )
 #
 #  Description : Fit a gaussian function on a set of sample points,
 #                using the Levenberg-Marquardt algorithm.
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

#ifndef cimg_plugin
#define cimg_plugin "examples/gaussian_fit1d.cpp"
#include "CImg.h"
using namespace cimg_library;
#undef min
#undef max

// Main procedure
//----------------
int main(int argc,char **argv) {
  cimg_usage("Fit gaussian function on sample points, using Levenberg-Marquardt algorithm.");

  // Read command line arguments.
  const char *s_params = cimg_option("-p","10,3,4","Amplitude, Mean and Std of the ground truth");
  const unsigned int s_nb = cimg_option("-N",40,"Number of sample points");
  const float s_noise = cimg_option("-n",10.0f,"Pourcentage of noise on the samples points");
  const char *s_xrange = cimg_option("-x","-10,10","X-range allowed for the sample points");
  const char *f_params = cimg_option("-p0",(char*)0,"Amplitude, Mean and Std of the first estimate");
  const float f_lambda0 = cimg_option("-l",100.0f,"Initial damping factor");
  const float f_dlambda = cimg_option("-dl",0.9f,"Damping attenuation");
  float s_xmin = -10, s_xmax = 10, s_amp = 1, s_mean = 1, s_std = 1;
  std::sscanf(s_xrange,"%f%*c%f",&s_xmin,&s_xmax);
  std::sscanf(s_params,"%f%*c%f%*c%f",&s_amp,&s_mean,&s_std);

  // Create noisy samples of a Gaussian function.
  const float s_std2 = 2*s_std*s_std, s_fact = s_amp/((float)std::sqrt(2*cimg::PI)*s_std);
  CImg<> samples(s_nb,2);
  cimg_forX(samples,i) {
    const float
      x = (float)(s_xmin + (s_xmax-s_xmin)*cimg::rand()),
      y = s_fact*(float)(1 + s_noise*cimg::grand()/100)*std::exp(-cimg::sqr(x-s_mean)/s_std2);
    samples(i,0) = x;
    samples(i,1) = y;
  }

  // Fit Gaussian function on the sample points and display curve iterations.
  CImgDisplay disp(640,480,"Levenberg-Marquardt Gaussian Fitting",0);
  float f_amp = 1, f_mean = 1, f_std = 1, f_lambda = f_lambda0;
  if (f_params) std::sscanf(f_params,"%f%*c%f%*c%f",&f_amp,&f_mean,&f_std);
  else {
    const float& vmax = samples.get_shared_row(1).max();
    float cmax = 0; samples.contains(vmax,cmax);
    f_mean = samples((int)cmax,0);
    f_std = (s_xmax-s_xmin)/10;
    f_amp = vmax*(float)std::sqrt(2*cimg::PI)*f_std;
  }
  CImg<> beta = CImg<>::vector(f_amp,f_mean,f_std);
  for (unsigned int iter = 0; !disp.is_closed() && !disp.is_keyQ() && !disp.is_keyESC(); ++iter) {

    // Do one iteration of the Levenberg-Marquardt algorithm.
    CImg<> YmF(1,s_nb), J(beta.height(),s_nb);
    const float
      f_amp = beta(0), f_mean = beta(1), f_std = beta(2),
      f_std2 = 2*f_std*f_std, f_fact = (float)std::sqrt(2*cimg::PI)*f_std;
    float f_error = 0;
    cimg_forY(J,i) {
      const float x = samples(i,0), f_exp = std::exp(-cimg::sqr(x-f_mean)/f_std2), delta = samples(i,1) - f_amp*f_exp/f_fact;
      YmF(i) = delta;
      J(0,i) = f_exp/f_fact;
      J(1,i) = f_amp*f_exp/f_fact*(x-f_mean)*2/f_std2;
      J(2,i) = f_amp*f_exp/f_fact*(cimg::sqr(x-f_mean)*2/f_std2-1);
      f_error+=cimg::sqr(delta);
    }
    CImg<> Jt = J.get_transpose(), M = Jt*J;
    cimg_forX(M,x) M(x,x)*=1 + f_lambda;
    beta+=M.get_invert()*Jt*YmF;
    if (beta(0)<=0) beta(0) = 0.1f;
    if (beta(2)<=0) beta(2) = 0.1f;
    f_lambda*=f_dlambda;

    // Display fitting curves.
    const unsigned char black[] = { 0,0,0 }, gray[] = { 228,228,228 };
    CImg<unsigned char>(disp.width(),disp.height(),1,3,255).draw_gaussfit(samples,beta(0),beta(1),beta(2),s_amp,s_mean,s_std).
      draw_rectangle(5,7,150,100,gray,0.9f).draw_rectangle(5,7,150,100,black,1,~0U).
      draw_text(10,10,"Iteration : %d",black,0,1,13,iter).
      draw_text(10,25,"Amplitude : %.4g (%.4g)",black,0,1,13,beta(0),s_amp).
      draw_text(10,40,"Mean : %.4g (%.4g)",black,0,1,13,beta(1),s_mean).
      draw_text(10,55,"Std : %.4g (%.4g)",black,0,1,13,beta(2),s_std).
      draw_text(10,70,"Error : %.4g",black,0,1,13,std::sqrt(f_error)).
      draw_text(10,85,"Lambda : %.4g",black,0,1,13,f_lambda).
      display(disp.resize(false).wait(20));
  }

  return 0;
}

#else

// Draw sample points, ideal and fitted gaussian curves on the instance image.
// (defined as a CImg plug-in function).
template<typename t>
CImg<T>& draw_gaussfit(const CImg<t>& samples,
                       const float f_amp, const float f_mean, const float f_std,
                       const float i_amp, const float i_mean, const float i_std) {
  if (is_empty()) return *this;
  const unsigned char black[] = { 0,0,0 }, green[] = { 10,155,20 }, orange[] = { 155,20,0 }, purple[] = { 200,10,200 };
  float
    xmin, xmax = samples.get_shared_row(0).max_min(xmin), deltax = xmax - xmin,
    ymin, ymax = samples.get_shared_row(1).max_min(ymin), deltay = ymax - ymin;
  xmin-=0.2f*deltax; xmax+=0.2f*deltax; ymin-=0.2f*deltay; ymax+=0.2f*deltay;
  deltax = xmax-xmin; deltay = ymax-ymin;
  draw_grid(64,64,0,0,false,false,black,0.3f,0x55555555,0x55555555).draw_axes(xmin,xmax,ymax,ymin,black,0.8f);
  CImg<> nsamples(samples);
  (nsamples.get_shared_row(0)-=xmin)*=width()/deltax;
  (nsamples.get_shared_row(1)-=ymax)*=-height()/deltay;
  cimg_forX(nsamples,i) draw_circle((int)nsamples(i,0),(int)nsamples(i,1),3,orange,1,~0U);
  CImg<int> truth(width(),2), fit(width(),2);
  const float
    i_std2 = 2*i_std*i_std, i_fact = i_amp/((float)std::sqrt(2*cimg::PI)*i_std),
    f_std2 = 2*f_std*f_std, f_fact = f_amp/((float)std::sqrt(2*cimg::PI)*f_std);
  cimg_forX(*this,x) {
    const float
      x0 = xmin + x*deltax/width(),
      ys0 = i_fact*std::exp(-cimg::sqr(x0-i_mean)/i_std2),
      yf0 = f_fact*std::exp(-cimg::sqr(x0-f_mean)/f_std2);
    fit(x,0) = truth(x,0) = x;
    truth(x,1) = (int)((ymax-ys0)*height()/deltay);
    fit(x,1) = (int)((ymax-yf0)*height()/deltay);
  }
  return draw_line(truth,green,0.7f,0xCCCCCCCC).draw_line(fit,purple);
}

#endif

