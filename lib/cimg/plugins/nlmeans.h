/*
 #
 #  File        : nlmeans.h
 #                ( C++ header file - CImg plug-in )
 #
 #  Description : CImg plugin that implements the non-local mean filter.
 #                This file is a part of the CImg Library project.
 #                ( http://cimg.sourceforge.net )
 #
 #  [1] Buades, A.; Coll, B.; Morel, J.-M.: A non-local algorithm for image denoising
 #      IEEE Computer Society Conference on Computer Vision and Pattern Recognition, 2005. CVPR 2005.
 #      Volume 2,  20-25 June 2005 Page(s):60 - 65 vol. 2
 #
 #  [2] Buades, A. Coll, B. and Morel, J.: A review of image denoising algorithms, with a new one.
 #      Multiscale Modeling and Simulation: A SIAM Interdisciplinary Journal 4 (2004) 490-530
 #
 #  [3] Gasser, T. Sroka,L. Jennen Steinmetz,C. Residual variance and residual pattern nonlinear regression.
 #      Biometrika 73 (1986) 625-659
 #
 #  Copyright   : Jerome Boulanger
 #                ( http://www.irisa.fr/vista/Equipe/People/Jerome.Boulanger.html )
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

#ifndef cimg_plugin_nlmeans
#define cimg_plugin_nlmeans

//! NL-Means denoising algorithm.
/**
   This is the in-place version of get_nlmean().
**/
CImg<T>& nlmeans(int patch_size=1, double lambda=-1, double alpha=3, double sigma=-1, int sampling=1){
  if (!is_empty()){
    if (sigma<0) sigma = std::sqrt(variance_noise()); // noise variance estimation
    const double np = (2*patch_size+1)*(2*patch_size+1)*spectrum()/(double)sampling;
    if (lambda<0) {// Bandwidth estimation
      if (np<100)
        lambda =(((((( 1.1785e-12*np -5.1827e-10)*np+ 9.5946e-08)*np -9.7798e-06)*np+ 6.0756e-04)*np -0.0248)*np+ 1.9203)*np +7.9599;
      else
        lambda = (-7.2611e-04*np+ 1.3213)*np+ 15.2726;
    }
#if cimg_debug>=1
    std::fprintf(stderr,"Size of the patch                              : %dx%d \n",
                 2*patch_size+1,2*patch_size+1);
    std::fprintf(stderr,"Size of window where similar patch are looked for : %dx%d \n",
                 (int)(alpha*(2*patch_size+1)),(int)(alpha*(2*patch_size+1)));
    std::fprintf(stderr,"Bandwidth of the kernel                                : %fx%f^2 \n",
                 lambda,sigma);
    std::fprintf(stderr,"Noise standard deviation estimated to          : %f \n",
                 sigma);
#endif

    CImg<T> dest(width(),height(),depth(),spectrum(),0);
    double *uhat = new double[spectrum()];
    const double h2 = -.5/(lambda*sigma*sigma); // [Kervrann] notations
    if (depth()!=1){ // 3D case
      const CImg<> P = (*this).get_blur(1); // inspired from Mahmoudi&Sapiro SPletter dec 05
      const int n_simu = 64;
      CImg<> tmp(n_simu,n_simu,n_simu);
      const double sig = std::sqrt(tmp.fill(0.f).noise(sigma).blur(1).pow(2.).sum()/(n_simu*n_simu*n_simu));
      const int
        patch_size_z = 0,
        pxi = (int)(alpha*patch_size),
        pyi = (int)(alpha*patch_size),
        pzi = 2; //Define the size of the neighborhood in z
      for (int zi = 0; zi<depth(); ++zi) {
#if cimg_debug>=1
        std::fprintf(stderr,"\rProcessing : %3d %%",(int)((float)zi/(float)depth()*100.));fflush(stdout);
#endif
        for (int yi = 0; yi<height(); ++yi)
          for (int xi = 0; xi<width(); ++xi) {
            cimg_forC(*this,v) uhat[v] = 0;
            float sw = 0, wmax = -1;
            for (int zj = cimg::max(0,zi-pzi); zj<cimg::min(depth(),zi+pzi+1); ++zj)
              for (int yj = cimg::max(0,yi-pyi); yj<cimg::min(height(),yi+pyi+1); ++yj)
                for (int xj = cimg::max(0,xi-pxi); xj<cimg::min(width(),xi+pxi+1); ++xj)
                  if (cimg::abs(P(xi,yi,zi) - P(xj,yj,zj))/sig<3) {
                    double d = 0;
                    int n = 0;
                    if (xi!=xj && yi!=yj && zi!=zj){
                      for (int kz = -patch_size_z; kz<patch_size_z+1; kz+=sampling) {
                        int
                          zj_ = zj+kz,
                          zi_ = zi+kz;
                        if (zj_>=0 && zj_<depth() && zi_>=0 && zi_<depth())
                          for (int ky = -patch_size; ky<=patch_size; ky+=sampling) {
                            int
                              yj_ = yj+ky,
                              yi_ = yi+ky;
                            if (yj_>=0 && yj_<height() && yi_>=0 && yi_<height())
                              for (int kx = -patch_size; kx<=patch_size; kx+=sampling) {
                                int
                                  xj_ = xj+kx,
                                  xi_ = xi+kx;
                                if (xj_>=0 && xj_<width() && xi_>=0 && xi_<width())
                                  cimg_forC(*this,v) {
                                    double d1 = (*this)(xj_,yj_,zj_,v) - (*this)(xi_,yi_,zi_,v);
                                    d+=d1*d1;
                                    ++n;
                                  }
                              }
                          }
                      }
                      float w = (float)std::exp(d*h2);
                      wmax = w>wmax?w:wmax;
                      cimg_forC(*this,v) uhat[v]+=w*(*this)(xj,yj,zj,v);
                      sw+=w;
                    }
                  }
            // add the central pixel
            cimg_forC(*this,v) uhat[v]+=wmax*(*this)(xi,yi,zi,v);
            sw+=wmax;
            if (sw) cimg_forC(*this,v) dest(xi,yi,zi,v) = (T)(uhat[v]/=sw);
            else cimg_forC(*this,v) dest(xi,yi,zi,v) = (*this)(xi,yi,zi,v);
          }
      }
    }
    else { // 2D case
      const CImg<> P = (*this).get_blur(1); // inspired from Mahmoudi&Sapiro SPletter dec 05
      const int n_simu = 512;
      CImg<> tmp(n_simu,n_simu);
      const double sig = std::sqrt(tmp.fill(0.f).noise(sigma).blur(1).pow(2.).sum()/(n_simu*n_simu));
      const int
        pxi = (int)(alpha*patch_size),
        pyi = (int)(alpha*patch_size); //Define the size of the neighborhood
      for (int yi = 0; yi<height(); ++yi) {
#if cimg_debug>=1
        std::fprintf(stderr,"\rProcessing : %3d %%",(int)((float)yi/(float)height()*100.));fflush(stdout);
#endif
        for (int xi = 0; xi<width(); ++xi) {
          cimg_forC(*this,v) uhat[v] = 0;
          float sw = 0, wmax = -1;
          for (int yj = cimg::max(0,yi-pyi); yj<cimg::min(height(),yi+pyi+1); ++yj)
            for (int xj = cimg::max(0,xi-pxi); xj<cimg::min(width(),xi+pxi+1); ++xj)
              if (cimg::abs(P(xi,yi) - P(xj,yj))/sig<3.) {
                double d = 0;
                int n = 0;
                if (!(xi==xj && yi==yj)) //{
                  for (int ky = -patch_size; ky<patch_size+1; ky+=sampling) {
                    int
                      yj_ = yj+ky,
                      yi_ = yi+ky;
                    if (yj_>=0 && yj_<height() && yi_>=0 && yi_<height())
                      for (int kx = -patch_size; kx<patch_size+1; kx+=sampling) {
                        int
                          xj_ = xj+kx,
                          xi_ = xi+kx;
                        if (xj_>=0 && xj_<width() && xi_>=0 && xi_<width())
                          cimg_forC(*this,v) {
                            double d1 = (*this)(xj_,yj_,v) - (*this)(xi_,yi_,v);
                            d+=d1*d1;
                            n++;
                          }
                      }
                    //}
                float w = (float)std::exp(d*h2);
                cimg_forC(*this,v) uhat[v]+=w*(*this)(xj,yj,v);
                wmax = w>wmax?w:wmax; // Store the maximum of the weights
                sw+=w; // Compute the sum of the weights
                }
              }
          // add the central pixel with the maximum weight
          cimg_forC(*this,v) uhat[v]+=wmax*(*this)(xi,yi,v);
          sw+=wmax;

          // Compute the estimate for the current pixel
          if (sw) cimg_forC(*this,v) dest(xi,yi,v) = (T)(uhat[v]/=sw);
          else cimg_forC(*this,v) dest(xi,yi,v) = (*this)(xi,yi,v);
        }
      } // main loop
    } // 2d
    delete [] uhat;
    dest.move_to(*this);
#if cimg_debug>=1
    std::fprintf(stderr,"\n"); // make a new line
#endif
  } // is empty
  return *this;
}

//! Get the result of the NL-Means denoising algorithm.
/**
   \param patch_size = radius of the patch (1=3x3 by default)
   \param lambda = bandwidth ( -1 by default : automatic selection)
   \param alpha  = size of the region where similar patch are searched (3 x patch_size = 9x9 by default)
   \param sigma = noise standard deviation (-1 = estimation)
   \param sampling = sampling of the patch (1 = uses all point, 2 = uses one point on 4, etc)
   If the image has three dimensions then the patch is only in  2D and the neighborhood extent in time is only 5.
   If the image has several channel (color images), the distance between the two patch is computed using
   all the channels.
   The greater the patch is the best is the result.
   Lambda parameter is function of the size of the patch size. The automatic Lambda parameter is taken
   in the Chi2 table at a significiance level of 0.01. This diffear from the original paper [1]. The weighted average becomes then:
   \f$$ \hat{f}(x,y) = \sum_{x',y'} \frac{1}{Z} exp(\frac{P(x,y)-P(x',y')}{2 \lambda \sigma^2}) f(x',y') $$\f
   where \f$ P(x,y) $\f denotes the patch in (x,y) location.

   An a priori is also used to increase the speed of the algorithm in the spirit of Sapiro et al. SPletter dec 05

   This very basic version of the Non-Local Means algorithm provides an output image which contains
   some residual noise with a relatively small variance (\f$\sigma<5$\f).

   [1] A non-local algorithm for image denoising
   Buades, A.; Coll, B.; Morel, J.-M.;
   Computer Vision and Pattern Recognition, 2005. CVPR 2005. IEEE Computer Society Conference on
   Volume 2,  20-25 June 2005 Page(s):60 - 65 vol. 2
**/
CImg<T> get_nlmeans( int patch_size=1,  double lambda=-1, double alpha=3 ,double sigma=-1, int sampling=1) const  {
  return CImg<T>(*this).nlmeans(patch_size,lambda,alpha,sigma,sampling);
}

#endif
