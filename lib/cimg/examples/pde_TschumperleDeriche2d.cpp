/*
 #
 #  File        : pde_TschumperleDeriche2d.cpp
 #                ( C++ source file )
 #
 #  Description : Implementation of the Tschumperle-Deriche's Regularization
 #                PDE, for 2D multivalued images, as described in the articles below.
 #                This file is a part of the CImg Library project.
 #                ( http://cimg.sourceforge.net )
 #
 #  (1) PDE-Based Regularization of Multivalued Images and Applications.
 #               (D. Tschumperle). PhD Thesis. University of Nice-Sophia Antipolis, December 2002.
 #  (2) Diffusion PDE's on Vector-valued Images : Local Approach and Geometric Viewpoint.
 #               (D. Tschumperle and R. Deriche). IEEE Signal Processing Magazine, October 2002.
 #  (3) Vector-Valued Image Regularization with PDE's : A Common Framework for Different Applications.
 #               (D. Tschumperle and R. Deriche). CVPR'2003, Computer Vision and Pattern Recognition, Madison, United States, June 2003.
 #
 #  This code can be used to perform image restoration, inpainting, magnification or flow visualization.
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
#undef min
#undef max

// Main procedure
//----------------
int main(int argc,char **argv) {

  // Read command line arguments
  //-----------------------------
  cimg_usage("Tschumperle-Deriche's flow for 2D Image Restoration, Inpainting, Magnification or Flow visualization");
  const char *file_i  = cimg_option("-i",cimg_imagepath "milla.bmp","Input image");
  const char *file_m  = cimg_option("-m",(char*)NULL,"Mask image (if Inpainting)");
  const char *file_f  = cimg_option("-f",(char*)NULL,"Flow image (if Flow visualization)");
  const char *file_o  = cimg_option("-o",(char*)NULL,"Output file");
  const double zoom   = cimg_option("-zoom",1.0,"Image magnification");

  const unsigned int nb_iter  = cimg_option("-iter",100000,"Number of iterations");
  const double dt     = cimg_option("-dt",20.0,"Adapting time step");
  const double alpha  = cimg_option("-alpha",0.0,"Gradient smoothing");
  const double sigma  = cimg_option("-sigma",0.5,"Structure tensor smoothing");
  const float a1      = cimg_option("-a1",0.5f,"Diffusion limiter along minimal variations");
  const float a2      = cimg_option("-a2",0.9f,"Diffusion limiter along maximal variations");
  const double noiseg = cimg_option("-ng",0.0,"Add gauss noise before aplying the algorithm");
  const double noiseu = cimg_option("-nu",0.0,"Add uniform noise before applying the algorithm");
  const double noises = cimg_option("-ns",0.0,"Add salt&pepper noise before applying the algorithm");
  const bool stflag   = cimg_option("-stats",false,"Display image statistics at each iteration");
  const unsigned int save = cimg_option("-save",0,"Iteration saving step");
  const unsigned int visu = cimg_option("-visu",10,"Visualization step (0=no visualization)");
  const unsigned int init = cimg_option("-init",3,"Inpainting initialization (0=black, 1=white, 2=noise, 3=unchanged)");
  const unsigned int skip = cimg_option("-skip",1,"Step of image geometry computation");
  bool view_t         = cimg_option("-d",false,"View tensor directions (useful for debug)");
  double xdt = 0;

  // Variable initialization
  //-------------------------
  CImg<> img, flow;
  CImg<int> mask;

  if (file_i) {
    img = CImg<>(file_i).resize(-100,-100,1,-100);
    if (file_m) mask = CImg<unsigned char>(file_m).resize(img.width(),img.height(),1,1);
    else if (zoom>1) {
      mask = CImg<int>(img.width(),img.height(),1,1,-1).resize((int)(img.width()*zoom),(int)(img.height()*zoom),1,1,4)+1;
      img.resize((int)(img.width()*zoom),(int)(img.height()*zoom),1,-100,3);
    }
  } else {
    if (file_f) {
      flow = CImg<>(file_f);
      img = CImg<>((int)(flow.width()*zoom),(int)(flow.height()*zoom),1,1,0).noise(100,2);
      flow.resize(img.width(),img.height(),1,2,3);
    } else throw CImgException("You need to specify at least one input image (option -i), or one flow image (option -f)");
  }
  img.noise(noiseg,0).noise(noiseu,1).noise(noises,2);
  float initial_min, initial_max = img.max_min(initial_min);
  if (mask && init!=3)
    cimg_forXYC(img,x,y,k) if (mask(x,y))
      img(x,y,k) = (float)((init?
                            (init==1?initial_max:((initial_max-initial_min)*cimg::rand())):
                            initial_min));

  CImgDisplay disp;
  if (visu) disp.assign(img,"Iterated Image");
  CImg<> G(img.width(),img.height(),1,3,0), T(G), veloc(img), val(2), vec(2,2);

  // PDE main iteration loop
  //-------------------------
  for (unsigned int iter = 0; iter<nb_iter &&
         (!disp || (!disp.is_closed() && !disp.is_keyQ() && !disp.is_keyESC())); ++iter) {
    std::printf("\riter %u , xdt = %g               ",iter,xdt); std::fflush(stdout);
    if (stflag) img.print();
    if (disp && disp.is_keySPACE()) { view_t = !view_t; disp.set_key(); }

    if (!(iter%skip)) {
      // Compute the tensor field T, used to drive the diffusion
      //---------------------------------------------------------

      // When using PDE for flow visualization
      if (flow) cimg_forXY(flow,x,y) {
        const float
          u = flow(x,y,0,0),
          v = flow(x,y,0,1),
          n = (float)std::sqrt((double)(u*u + v*v)),
          nn = (n!=0)?n:1;
        T(x,y,0) = u*u/nn;
        T(x,y,1) = u*v/nn;
        T(x,y,2) = v*v/nn;
      } else {

        // Compute structure tensor field G
        CImgList<> grad = img.get_gradient();
        if (alpha!=0) cimglist_for(grad,l) grad[l].blur((float)alpha);
        G.fill(0);
        cimg_forXYC(img,x,y,k) {
          const float ix = grad[0](x,y,k), iy = grad[1](x,y,k);
          G(x,y,0) += ix*ix;
          G(x,y,1) += ix*iy;
          G(x,y,2) += iy*iy;
        }
        if (sigma!=0) G.blur((float)sigma);

        // When using PDE for image restoration, inpainting or zooming
        T.fill(0);
        if (!mask) cimg_forXY(G,x,y) {
          G.get_tensor_at(x,y).symmetric_eigen(val,vec);
          const float
            l1 = (float)std::pow(1.0f + val[0] + val[1],-a1),
            l2 = (float)std::pow(1.0f + val[0] + val[1],-a2),
            ux = vec(1,0),
            uy = vec(1,1);
          T(x,y,0) = l1*ux*ux + l2*uy*uy;
          T(x,y,1) = l1*ux*uy - l2*ux*uy;
          T(x,y,2) = l1*uy*uy + l2*ux*ux;
        }
        else cimg_forXY(G,x,y) if (mask(x,y)) {
          G.get_tensor_at(x,y).symmetric_eigen(val,vec);
          const float
            ux = vec(1,0),
            uy = vec(1,1);
          T(x,y,0) = ux*ux;
          T(x,y,1) = ux*uy;
          T(x,y,2) = uy*uy;
        }
      }
    }

    // Compute the PDE velocity and update the iterated image
    //--------------------------------------------------------
    CImg_3x3(I,float);
    veloc.fill(0);
    cimg_forC(img,k) cimg_for3x3(img,x,y,0,k,I,float) {
      const float
        a = T(x,y,0),
        b = T(x,y,1),
        c = T(x,y,2),
        ixx = Inc+Ipc-2*Icc,
        iyy = Icn+Icp-2*Icc,
        ixy = 0.25f*(Ipp+Inn-Ipn-Inp);
      veloc(x,y,k) = a*ixx + 2*b*ixy + c*iyy;
    }
    if (dt>0) {
      float m, M = veloc.max_min(m);
      xdt = dt/cimg::max(cimg::abs(m),cimg::abs(M));
    } else xdt=-dt;
    img+=veloc*xdt;
    img.cut((float)initial_min,(float)initial_max);

    // Display and save iterations
    if (disp && !(iter%visu)) {
      if (!view_t) img.display(disp);
      else {
        const unsigned char white[3] = {255,255,255};
        CImg<unsigned char> visu = img.get_resize(disp.width(),disp.height()).normalize(0,255);
        CImg<> isophotes(img.width(),img.height(),1,2,0);
        cimg_forXY(img,x,y) if (!mask || mask(x,y)) {
          T.get_tensor_at(x,y).symmetric_eigen(val,vec);
          isophotes(x,y,0) = vec(0,0);
          isophotes(x,y,1) = vec(0,1);
        }
        visu.draw_quiver(isophotes,white,0.5f,10,9,0).display(disp);
      }
    }
    if (save && file_o && !(iter%save)) img.save(file_o,iter);
    if (disp) disp.resize().display(img);
  }

  // Save result and exit.
  if (file_o) img.save(file_o);
  return 0;
}
