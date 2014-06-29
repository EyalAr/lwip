/*
 #
 #  File        : chlpca.cpp
 #                ( C++ source file )
 #
 #  Description : Example of use for the CImg plugin 'plugins/chlpca.h'.
 #                This file is a part of the CImg Library project.
 #                ( http://cimg.sourceforge.net )
 #
 #  Copyright  : Jerome Boulanger
 #               ( http://www.irisa.fr/vista/Equipe/People/Jerome.Boulanger.html )
 #
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

// Define some useful macros.

//! Some loops
#define cimg_for_step1(bound,i,step) for (int i = 0; i<(int)(bound); i+=step)
#define cimg_for_stepX(img,x,step) cimg_for_step1((img)._width,x,step)
#define cimg_for_stepY(img,y,step) cimg_for_step1((img)._height,y,step)
#define cimg_for_stepZ(img,z,step) cimg_for_step1((img)._depth,z,step)
#define cimg_for_stepXY(img,x,y,step) cimg_for_stepY(img,y,step) cimg_for_stepX(img,x,step)
#define cimg_for_stepXYZ(img,x,y,step) cimg_for_stepZ(img,z,step) cimg_for_stepY(img,y,step) cimg_for_stepX(img,x,step)

//! Loop for point J(xj,yj) in the neighborhood of a point I(xi,yi) of size (2*rx+1,2*ry+1)
/**
   Point J is kept inside the boundaries of the image img.
   example of summing the pixels values in a neighborhood 11x11
   cimg_forXY(img,xi,yi) cimg_for_windowXY(img,xi,yi,xj,yj,5,5) dest(yi,yi) += src(xj,yj);
**/
#define cimg_forXY_window(img,xi,yi,xj,yj,rx,ry)                        \
for (int yi0=cimg::max(0,yi-ry), yi1=cimg::min(yi+ry,(int)img.height()-1), yj=yi0;yj<=yi1;++yj) \
for (int xi0=cimg::max(0,xi-rx), xi1=cimg::min(xi+rx,(int)img.width()-1), xj=xi0;xj<=xi1;++xj)

#define cimg_forXYZ_window(img,xi,yi,zi,xj,yj,zj,rx,ry,rz)                                      \
for (int zi0=cimg::max(0,zi-rz), zi1=cimg::min(zi+rz,(int)img.depth()-1) , zj=zi0;zj<=zi1;++zj) \
for (int yi0=cimg::max(0,yi-ry), yi1=cimg::min(yi+ry,(int)img.height()-1), yj=yi0;yj<=yi1;++yj) \
for (int xi0=cimg::max(0,xi-rx), xi1=cimg::min(xi+rx,(int)img.width()-1) , xj=xi0;xj<=xi1;++xj)

//! Crop a patch in the image around position x,y,z and return a column vector
/**
   \param x x-coordinate of the center of the patch
   \param y y-coordinate of the center of the patch
   \param z z-coordinate of the center of the patch
   \param px the patch half width
   \param px the patch half height
   \param px the patch half depth
   \return img.get_crop(x0,y0,z0,x1,y1,z1).unroll('y');
**/
CImg<T> get_patch(int x, int y, int z,
                  int px, int py, int pz) const {
  if (depth() == 1){
    const int x0 = x - px, y0 = y - py, x1 = x + px, y1 = y + py;
    return get_crop(x0, y0, x1, y1).unroll('y');
  } else {
    const int
      x0 = x - px, y0 = y - py, z0 = z - pz,
      x1 = x + px, y1 = y + py, z1 = z + pz;
    return get_crop(x0, y0, z0, x1, y1, z1).unroll('y');
  }
}

//! Extract a local patch dictionnary around point xi,yi,zi
CImg<T> get_patch_dictionnary(const int xi, const int yi, const int zi,
                              const int px, const int py, const int pz,
                              const int wx, const int wy, const int wz,
                              int & idc) const {
  const int
    n = (2*wx+1) * (2*wy+1) * (2 * (depth()==1?0:wz) + 1),
    d = (2*px+1) * (2*py+1) * (2 * (depth()==1?0:px) + 1) * spectrum();
  CImg<> S(n, d);
  int idx = 0;
  if (depth() == 1) {
    cimg_forXY_window((*this), xi, yi, xj, yj, wx, wy){
      CImg<T> patch = get_patch(xj, yj, 0, px, py, 1);
      cimg_forY(S,y) S(idx,y) = patch(y);
      if (xj==xi && yj==yi) idc = idx;
      idx++;
    }
  } else  {
    cimg_forXYZ_window((*this), xi,yi,zi,xj,yj,zj,wx,wy,wz){
      CImg<T> patch = get_patch(xj, yj, zj, px, py, pz);
      cimg_forY(S,y) S(idx,y) = patch(y);
      if (xj==xi && yj==yi && zj==zi) idc = idx;
      idx++;
    }
  }
  S.columns(0, idx - 1);
  return S;
}

//! Add a patch to the image
/**
   \param x x-coordinate of the center of the patch
   \param y y-coordinate of the center of the patch
   \param z z-coordinate of the center of the patch
   \param img the patch as a 1D column vector
   \param px the patch half width
   \param px the patch half height
   \param px the patch half depth
**/
CImg<T> & add_patch(const int xi, const int yi, const int zi,
                    const CImg<T> & patch,
                    const int px, const int py, const int pz) {
  const int
    x0 = xi - px, y0 = yi - py, z0 = (depth() == 1 ? 0 : zi - pz),
    sx = 2 * px + 1, sy = 2 * py + 1, sz = (depth() == 1 ? 1 : 2 * pz +1);
  draw_image(x0, y0, z0, 0, patch.get_resize(sx, sy, sz, spectrum(), -1), -1);
  return (*this);
}

//! Add a constant patch to the image
/**
   \param x x-coordinate of the center of the patch
   \param y y-coordinate of the center of the patch
   \param z z-coordinate of the center of the patch
   \param value in the patch
   \param px the patch half width
   \param px the patch half height
   \param px the patch half depth
**/
CImg<T> & add_patch(const int xi, const int yi, const int zi, const T value,
                    const int px, const int py, const int pz) {
  const int
    x0 = xi - px, y0 = yi - py, z0 = (depth() == 1 ? 0 : zi - pz),
    x1 = xi + px, y1 = yi + py, z1 = (depth() == 1 ? 0 : zi + pz);
  draw_rectangle(x0, y0, z0, 0, x1, y1, z1, spectrum()-1, value, -1);
return (*this);
}

//! CHLPCA denoising from the PhD thesis of Hu Haijuan
/**
   \param px the patch half width
   \param px the patch half height
   \param px the patch half depth
   \param wx the training region half width
   \param wy the training region half height
   \param wz the training region half depth
   \param nstep the subsampling of the image domain
   \param nsim the number of patches used for training as a factor of the patch size
   \param lambda_min the threshold on the eigen values of the PCA for dimension reduction
   \param threshold the threshold on the value of the coefficients
   \param pca_use_svd if true use the svd approach to perform the pca otherwise use the covariance method
   \note please cite the PhD thesis of Hu Haijuan http://www.univ-ubs.fr/soutenance-de-these-hu-haijuan-337653.kjsp?RH=1318498222799
 **/
CImg<T> get_chlpca(const int px, const int py, const int pz,
                       const int wx, const int wy, const int wz,
                       const int nstep, const float nsim,
                       const float lambda_min, const float threshold,
                       const float noise_std,  const bool pca_use_svd) const {
  const int
    nd = (2*px+1) * (2*py+1) * (depth()==1?1:2*pz+1) * spectrum(),
    K = nsim * nd;
#ifdef DEBUG
  fprintf(stderr,"chlpca: p:%dx%dx%d,w:%dx%dx%d,nd:%d,K:%d\n",
          2*px+1,2*py+1,2*pz+1,2*wx+1,2*wy+1,2*wz+1,nd,K);
#endif
  float sigma;
  if (noise_std < 0) sigma = std::sqrt(variance_noise());
  else sigma = noise_std;
  CImg<T> dest(*this), count(*this);
  dest.fill(0);
  count.fill(0);
  cimg_for_stepZ(*this,zi,(depth()==1||pz==0)?1:nstep){
#ifdef cimg_use_openmp
#pragma omp parallel for
#endif
    cimg_for_stepXY((*this),xi,yi,nstep){
      // extract the training region X
      int idc = 0;
      CImg<T> S = get_patch_dictionnary(xi,yi,zi,px,py,pz,wx,wy,wz,idc);
      // select the K most similar patches within the training set
      CImg<T> Sk(S);
      CImg<unsigned int> index(S.width());
      if (K < Sk.width() - 1){
        CImg<T> mse(S.width());
        CImg<unsigned int> perms;
        cimg_forX(S,x){mse(x) = S.get_column(idc).MSE(S.get_column(x)); }
        mse.sort(perms,true);
        cimg_foroff(perms,i) {
          cimg_forY(S,j) Sk(i,j) = S(perms(i),j);
          index(perms(i)) = i;
        }
        Sk.columns(0, K);
        perms.threshold(K);
      } else {
        cimg_foroff(index,i) index(i)=i;
      }
      // centering the patches
      CImg<T> M(1, Sk.height(), 1, 1, 0);
      cimg_forXY(Sk,x,y) { M(y) += Sk(x,y); }
      M /= (T)Sk.width();
      cimg_forXY(Sk,x,y) { Sk(x,y) -= M(y); }
      // compute the principal component of the training set S
      CImg<T> P, lambda;
      if (pca_use_svd) {
        CImg<T> V;
        Sk.get_transpose().SVD(V,lambda,P,100);
      } else {
        (Sk * Sk.get_transpose()).symmetric_eigen(lambda, P);
        lambda.sqrt();
      }
      // dimension reduction
      int s = 0;
      const T tx = std::sqrt((double)Sk.width()-1.0) * lambda_min * sigma;
      while((lambda(s) > tx) && (s < ((int)lambda.size() - 1))) { s++; }
      P.columns(0,s);
      // project all the patches on the basis (compute scalar product)
      Sk = P.get_transpose() * Sk;
      // threshold the coefficients
      if (threshold > 0) { Sk.threshold(threshold, 1); }
      // project back to pixel space
      Sk =  P * Sk;
      // recenter the patches
      cimg_forXY(Sk,x,y) { Sk(x,y) += M(y); }
      int j = 0;
      cimg_forXYZ_window((*this),xi,yi,zi,xj,yj,zj,wx,wy,wz){
        const int id = index(j);
        if (id < Sk.width()) {
          dest.add_patch(xj, yj, zj, Sk.get_column(id), px, py, pz);
          count.add_patch(xj, yj, zj, (T)1, px, py, pz);
        }
        j++;
      }
    }
  }
  cimg_foroff(dest, i) {
    if(count(i) != 0) { dest(i) /= count(i); }
    else { dest(i) = (*this)(i); }
  }
  return dest;
}

//! CHLPCA denoising from the PhD thesis of Hu Haijuan
/**
   \param px the patch half width
   \param px the patch half height
   \param px the patch half depth
   \param wx the training region half width
   \param wy the training region half height
   \param wz the training region half depth
   \param nstep the subsampling of the image domain
   \param nsim the number of patches used for training as a factor of the patch size
   \param lambda_min the threshold on the eigen values of the PCA for dimension reduction
   \param threshold the threshold on the value of the coefficients
   \param pca_use_svd if true use the svd approach to perform the pca otherwise use the covariance method
   \note please cite the PhD thesis of Hu Haijuan http://www.univ-ubs.fr/soutenance-de-these-hu-haijuan-337653.kjsp?RH=1318498222799
 **/
CImg<T> & chlpca(const int px, const int py, const int pz,
                 const int wx, const int wy, const int wz,
                 const int nstep, const float nsim,
                 const float lambda_min, const float threshold,
                 const float noise_std,  const bool pca_use_svd)  {
  (*this) = get_chlpca(px, py, pz, wx, wy, wz, nstep, nsim, lambda_min,
                               threshold, noise_std, pca_use_svd);
  return (*this);
}

//! CHLPCA denoising from the PhD thesis of Hu Haijuan
/**
   \param p the patch half size
   \param w the training region half size
   \param nstep the subsampling of the image domain
   \param nsim the number of patches used for training as a factor of the patch size
   \param lambda_min the threshold on the eigen values of the PCA for dimension reduction
   \param threshold the threshold on the value of the coefficients
   \param pca_use_svd if true use the svd approach to perform the pca otherwise use the covariance method
   \note please cite the PhD thesis of Hu Haijuan http://www.univ-ubs.fr/soutenance-de-these-hu-haijuan-337653.kjsp?RH=1318498222799
 **/
CImg<T> get_chlpca(const int p=3, const int w=10,
                   const int nstep=5, const float nsim=10,
                   const float lambda_min=2, const float threshold = -1,
                   const float noise_std=-1, const bool pca_use_svd=true) const {
  if (depth()==1) return get_chlpca(p, p, 0, w, w, 0, nstep, nsim, lambda_min,
                                    threshold, noise_std, pca_use_svd);
  else return get_chlpca(p, p, p, w, w, w, nstep, nsim, lambda_min,
                         threshold, noise_std, pca_use_svd);
}

CImg<T> chlpca(const int p=3, const int w=10,
               const int nstep=5, const float nsim=10,
               const float lambda_min=2, const float threshold = -1,
               const float noise_std=-1, const bool pca_use_svd=true) {
  (*this) = get_chlpca(p, w, nstep, nsim, lambda_min,
                       threshold, noise_std, pca_use_svd);
  return (*this);
}
