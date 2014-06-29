/*
 #
 #  File        : radon_transform2d.cpp
 #                ( C++ source file )
 #
 #  Description : An implementation of the Radon Transform.
 #                This file is a part of the CImg Library project.
 #                ( http://cimg.sourceforge.net )
 #
 #  Copyright   : David G. Starkweather
 #                ( starkdg@sourceforge.net - starkweatherd@cox.net )
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

#define ROUNDING_FACTOR(x) (((x) >= 0) ? 0.5 : -0.5)

CImg<double> GaussianKernel(double rho);
CImg<float> ApplyGaussian(CImg<unsigned char> im,double rho);
CImg<unsigned char> RGBtoGrayScale(CImg<unsigned char> &im);
int GetAngle(int dy,int dx);
CImg<unsigned char> CannyEdges(CImg<float> im, double T1, double T2,bool doHysteresis);
CImg<> RadonTransform(CImg<unsigned char> im,int N);

// Main procedure
//----------------
int main(int argc,char **argv) {
  cimg_usage("Illustration of the Radon Transform");

  const char *file = cimg_option("-f",cimg_imagepath "parrot_original.ppm","path and file name");
  const double sigma = cimg_option("-r",1.0,"blur coefficient for gaussian low pass filter (lpf)"),
    thresh1 = cimg_option("-t1",0.50,"lower threshold for canny edge detector"),
    thresh2 = cimg_option("-t2",1.25,"upper threshold for canny edge detector");;
  const int N = cimg_option("-n",64,"number of angles to consider in the Radon transform - should be a power of 2");

  //color to draw lines
  const unsigned char green[] = {0,255,0};
  CImg<unsigned char> src(file);

  int rhomax = (int)std::sqrt((double)(src.width()*src.width() + src.height()*src.height()))/2;

  if (cimg::dialog(cimg::basename(argv[0]),
                   "Instructions:\n"
                   "Click on space bar or Enter key to display Radon transform of given image\n"
                   "Click on anywhere in the transform window to display a \n"
                   "corresponding green line in the original image\n",
                   "Start", "Quit",0,0,0,0,
                   src.get_resize(100,100,1,3),true)) std::exit(0);

  //retrieve a grayscale from the image
  CImg<unsigned char> grayScaleIm;
  if ((src.spectrum() == 3) && (src.width() > 0) && (src.height() > 0) && (src.depth() == 1))
    grayScaleIm = (CImg<unsigned char>)src.get_norm(0).quantize(255,false);
  else if ((src.spectrum() == 1)&&(src.width() > 0) && (src.height() > 0) && (src.depth() == 1))
    grayScaleIm = src;
  else { // image in wrong format
    if (cimg::dialog(cimg::basename("wrong file format"),
                     "Incorrect file format\n","OK",0,0,0,0,0,
                     src.get_resize(100,100,1,3),true)) std::exit(0);
  }

  //blur the image with a Gaussian lpf to remove spurious edges (e.g. noise)
  CImg<float> blurredIm = ApplyGaussian(grayScaleIm,sigma);

  //use canny edge detection algorithm to get edge map of the image
  //- the threshold values are used to perform hysteresis in the edge detection process
  CImg<unsigned char> cannyEdgeMap = CannyEdges(blurredIm,thresh1,thresh2,false);
  CImg<unsigned char> radonImage = *(new CImg<unsigned char>(500,400,1,1,0));

  //display the two windows
  CImgDisplay dispImage(src,"original image");
  dispImage.move(CImgDisplay::screen_width()/8,CImgDisplay::screen_height()/8);
  CImgDisplay dispRadon(radonImage,"Radon Transform");
  dispRadon.move(CImgDisplay::screen_width()/4,CImgDisplay::screen_height()/4);
  CImgDisplay dispCanny(cannyEdgeMap,"canny edges");
  //start main display loop
  while (!dispImage.is_closed() && !dispRadon.is_closed() &&
         !dispImage.is_keyQ() && !dispRadon.is_keyQ() &&
         !dispImage.is_keyESC() && !dispRadon.is_keyESC()) {

    CImgDisplay::wait(dispImage,dispRadon);

    if (dispImage.is_keySPACE() || dispRadon.is_keySPACE()) {
      radonImage = (CImg<unsigned char>)RadonTransform(cannyEdgeMap,N).quantize(255,false).resize(500,400);
      radonImage.display(dispRadon);
    }

    //when clicking on dispRadon window, draw line in original image window
    if (dispRadon.button())
      {
        const double rho = dispRadon.mouse_y()*rhomax/dispRadon.height(),
          theta = (dispRadon.mouse_x()*N/dispRadon.width())*2*cimg::PI/N,
          x = src.width()/2 + rho*std::cos(theta),
          y = src.height()/2 + rho*std::sin(theta);
        const int x0 = (int)(x + 1000*std::cos(theta + cimg::PI/2)),
          y0 = (int)(y + 1000*std::sin(theta + cimg::PI/2)),
          x1 = (int)(x - 1000*std::cos(theta + cimg::PI/2)),
          y1 = (int)(y - 1000*std::sin(theta + cimg::PI/2));
        src.draw_line(x0,y0,x1,y1,green,1.0f,0xF0F0F0F0).display(dispImage);
      }
  }
  return 0;
}
/**
 * PURPOSE: create a 5x5 gaussian kernel matrix
 * PARAM rho - gaussiam equation parameter (default = 1.0)
 * RETURN CImg<double> the gaussian kernel
 **/

CImg<double> GaussianKernel(double sigma = 1.0)
{
  CImg<double> resultIm(5,5,1,1,0);
  int midX = 3, midY = 3;
  cimg_forXY(resultIm,X,Y) {
    resultIm(X,Y) = std::ceil(256.0*(std::exp(-(midX*midX + midY*midY)/(2*sigma*sigma)))/(2*cimg::PI*sigma*sigma));
  }
  return resultIm;
}
/*
 * PURPOSE: convolve a given image with the gaussian kernel
 * PARAM CImg<unsigned char> im - image to be convolved upon
 * PARAM double sigma - gaussian equation parameter
 * RETURN CImg<float> image resulting from the convolution
 * */
CImg<float> ApplyGaussian(CImg<unsigned char> im,double sigma)
{
  CImg<float> smoothIm(im.width(),im.height(),1,1,0);

  //make gaussian kernel
  CImg<float> gk = GaussianKernel(sigma);
  //apply gaussian

  CImg_5x5(I,int);
  cimg_for5x5(im,X,Y,0,0,I,int) {
    float sum = 0;
    sum += gk(0,0)*Ibb + gk(0,1)*Ibp + gk(0,2)*Ibc + gk(0,3)*Ibn + gk(0,4)*Iba;
    sum += gk(1,0)*Ipb + gk(1,1)*Ipp + gk(1,2)*Ipc + gk(1,3)*Ipn + gk(1,4)*Ipa;
    sum += gk(2,0)*Icb + gk(2,1)*Icp + gk(2,2)*Icc + gk(2,3)*Icn + gk(2,4)*Ica;
    sum += gk(3,0)*Inb + gk(3,1)*Inp + gk(3,2)*Inc + gk(3,3)*Inn + gk(3,4)*Ina;
    sum += gk(4,0)*Iab + gk(4,1)*Iap + gk(4,2)*Iac + gk(4,3)*Ian + gk(4,4)*Iaa;
    smoothIm(X,Y) = sum/256;
  }
  return smoothIm;
}
/**
 * PURPOSE: convert a given rgb image to a MxNX1 single vector grayscale image
 * PARAM: CImg<unsigned char> im - rgb image to convert
 * RETURN: CImg<unsigned char> grayscale image with MxNx1x1 dimensions
 **/

CImg<unsigned char> RGBtoGrayScale(CImg<unsigned char> &im)
{
  CImg<unsigned char> grayImage(im.width(),im.height(),im.depth(),1,0);
  if (im.spectrum() == 3) {
    cimg_forXYZ(im,X,Y,Z) {
      grayImage(X,Y,Z,0) = (unsigned char)(0.299*im(X,Y,Z,0) + 0.587*im(X,Y,Z,1) + 0.114*im(X,Y,Z,2));
    }
  }
  grayImage.quantize(255,false);
  return grayImage;
}
/**
 * PURPOSE: aux. function used by CannyEdges to quantize an angle theta given by gradients, dx and dy
 *  into 0 - 7
 * PARAM: dx,dy - gradient magnitudes
 * RETURN int value between 0 and 7
 **/
int GetAngle(int dy,int dx)
{
  double angle = cimg::abs(std::atan2((double)dy,(double)dx));
  if ((angle >= -cimg::PI/8)&&(angle <= cimg::PI/8))//-pi/8 to pi/8 => 0
    return 0;
  else if ((angle >= cimg::PI/8)&&(angle <= 3*cimg::PI/8))//pi/8 to 3pi/8 => pi/4
    return 1;
  else if ((angle > 3*cimg::PI/8)&&(angle <= 5*cimg::PI/8))//3pi/8 to 5pi/8 => pi/2
    return 2;
  else if ((angle > 5*cimg::PI/8)&&(angle <= 7*cimg::PI/8))//5pi/8 to 7pi/8 => 3pi/4
    return 3;
  else if (((angle > 7*cimg::PI/8) && (angle <= cimg::PI))  || ((angle <= -7*cimg::PI/8)&&(angle >= -cimg::PI))) //-7pi/8 to -pi OR 7pi/8 to pi => pi
    return 4;
  else return 0;
}
/**
 * PURPOSE: create an edge map of the given image with hysteresis using thresholds T1 and T2
 * PARAMS: CImg<float> im the image to perform edge detection on
 *                 T1 lower threshold
 *         T2 upper threshold
 * RETURN CImg<unsigned char> edge map
 **/
CImg<unsigned char> CannyEdges(CImg<float> im, double T1, double T2, bool doHysteresis=false)
{
  CImg<unsigned char> edges(im);
  CImg<float> secDerivs(im);
  secDerivs.fill(0);
  edges.fill(0);
  CImgList<float> gradients = im.get_gradient("xy",1);
  int image_width = im.width();
  int image_height = im.height();

  cimg_forXY(im,X,Y) {
    double Gr = std::sqrt(std::pow((double)gradients[0](X,Y),2.0) + std::pow((double)gradients[1](X,Y),2.0));
    double theta = GetAngle(Y,X);
    //if Gradient magnitude is positive and X,Y within the image
    //take the 2nd deriv in the appropriate direction
    if ((Gr > 0)&&(X < image_width-2)&&(Y < image_height - 2)) {
      if (theta == 0)
        secDerivs(X,Y) = im(X+2,Y) - 2*im(X+1,Y) + im(X,Y);
      else if (theta == 1)
        secDerivs(X,Y) = im(X+2,Y+2) - 2*im(X+1,Y+1) + im(X,Y);
      else if (theta == 2)
        secDerivs(X,Y) = im(X,Y+2) - 2*im(X,Y+1) + im(X,Y);
      else if (theta == 3)
        secDerivs(X,Y) = im(X+2,Y+2) - 2*im(X+1,Y+1) + im(X,Y);
      else if (theta == 4)
        secDerivs(X,Y) = im(X+2,Y) - 2*im(X+1,Y) + im(X,Y);
    }
  }
  //for each 2nd deriv that crosses a zero point and magnitude passes the upper threshold.
  //Perform hysteresis in the direction of the gradient, rechecking the gradient
  //angle for each pixel that meets the threshold requirement.  Stop checking when
  //the lower threshold is not reached.
  CImg_5x5(I,float);
  cimg_for5x5(secDerivs,X,Y,0,0,I,float) {
    if (   (Ipp*Ibb < 0) ||
           (Ipc*Ibc < 0)||
           (Icp*Icb < 0)   ) {
      double Gr = std::sqrt(std::pow((double)gradients[0](X,Y),2.0) + std::pow((double)gradients[1](X,Y),2.0));
      int dir = GetAngle(Y,X);
      int Xt = X, Yt = Y, delta_x = 0, delta_y=0;
      double GRt = Gr;
      if (Gr >= T2)
        edges(X,Y) = 255;
      //work along the gradient in one direction
      if (doHysteresis) {
        while ((Xt > 0) && (Xt < image_width-1) && (Yt > 0) && (Yt < image_height-1)) {
          switch (dir){
          case 0 : delta_x=0;delta_y=1;break;
          case 1 : delta_x=1;delta_y=1;break;
          case 2 : delta_x=1;delta_y=0;break;
          case 3 : delta_x=1;delta_y=-1;break;
          case 4 : delta_x=0;delta_y=1;break;
          }
          Xt += delta_x;
          Yt += delta_y;
          GRt = std::sqrt(std::pow((double)gradients[0](Xt,Yt),2.0) + std::pow((double)gradients[1](Xt,Yt),2.0));
          dir = GetAngle(Yt,Xt);
          if (GRt >= T1)
            edges(Xt,Yt) = 255;
        }
        //work along gradient in other direction
        Xt = X; Yt = Y;
        while ((Xt > 0) && (Xt < image_width-1) && (Yt > 0) && (Yt < image_height-1)) {
          switch (dir){
          case 0 : delta_x=0;delta_y=1;break;
          case 1 : delta_x=1;delta_y=1;break;
          case 2 : delta_x=1;delta_y=0;break;
          case 3 : delta_x=1;delta_y=-1;break;
          case 4 : delta_x=0;delta_y=1;break;
          }
          Xt -= delta_x;
          Yt -= delta_y;
          GRt = std::sqrt(std::pow((double)gradients[0](Xt,Yt),2.0) + std::pow((double)gradients[1](Xt,Yt),2.0));
          dir = GetAngle(Yt,Xt);
          if (GRt >= T1)
            edges(Xt,Yt) = 255;
        }
      }
    }
  }
  return edges;
}
/**
 * PURPOSE: perform radon transform of given image
 * PARAM: CImg<unsigned char> im - image to detect lines
 *                int N - number of angles to consider (should be a power of 2)
 *                (the values of N will be spread over 0 to 2PI)
 * RETURN CImg<unsigned char> - transform of given image of size, N x D
 *                              D = rhomax = sqrt(dimx*dimx + dimy*dimy)/2
 **/
CImg<> RadonTransform(CImg<unsigned char> im,int N) {
  int image_width = im.width();
  int image_height = im.height();

  //calc offsets to center the image
  float xofftemp = image_width/2.0f - 1;
  float yofftemp = image_height/2.0f - 1;
  int xoffset = (int)std::floor(xofftemp + ROUNDING_FACTOR(xofftemp));
  int yoffset = (int)std::floor(yofftemp + ROUNDING_FACTOR(yofftemp));
  float dtemp = (float)std::sqrt((double)(xoffset*xoffset + yoffset*yoffset));
  int D = (int)std::floor(dtemp + ROUNDING_FACTOR(dtemp));

  CImg<> imRadon(N,D,1,1,0);

  //for each angle k to consider
  for (int k= 0 ; k < N; k++) {
    //only consider from PI/8 to 3PI/8 and 5PI/8 to 7PI/8
    //to avoid computational complexity of a steep angle
    if (k == 0){k = N/8;continue;}
    else if (k == (3*N/8 + 1)){ k = 5*N/8;continue;}
    else if (k == 7*N/8 + 1){k = N;     continue;}

    //for each rho length, determine linear equation and sum the line
    //sum is to sum the values along the line at angle k2pi/N
    //sum2 is to sum the values along the line at angle k2pi/N + N/4
    //The sum2 is performed merely by swapping the x,y axis as if the image were rotated 90 degrees.
    for (int d=0; d < D; d++) {
      double theta = 2*k*cimg::PI/N;//calculate actual theta
      double alpha = std::tan(theta+cimg::PI/2);//calculate the slope
      double beta_temp = -alpha*d*std::cos(theta) + d*std::sin(theta);//y-axis intercept for the line
      int beta = (int)std::floor(beta_temp + ROUNDING_FACTOR(beta_temp));
      //for each value of m along x-axis, calculate y
      //if the x,y location is within the boundary for the respective image orientations, add to the sum
      unsigned int sum1 = 0,
        sum2 = 0;
      int M = (image_width >= image_height) ? image_width : image_height;
      for (int m=0;m < M; m++) {
        //interpolate in-between values using nearest-neighbor approximation
        //using m,n as x,y indices into image
        double n_temp = alpha*(m-xoffset) + beta;
        int n = (int)std::floor(n_temp + ROUNDING_FACTOR(n_temp));
        if ((m < image_width) && (n + yoffset >= 0) && (n + yoffset < image_height))
          {
            sum1 += im(m, n + yoffset);
          }
        n_temp = alpha*(m-yoffset) + beta;
        n = (int)std::floor(n_temp + ROUNDING_FACTOR(n_temp));
        if ((m < image_height)&&(n + xoffset >= 0)&&(n + xoffset < image_width))
          {
            sum2 += im(-(n + xoffset) + image_width - 1, m);
          }
      }
      //assign the sums into the result matrix
      imRadon(k,d) = (float)sum1;
      //assign sum2 to angle position for theta+PI/4
      imRadon(((k + N/4)%N),d) = (float)sum2;
    }
  }
  return imRadon;
}
/* references:
 * 1. See Peter Toft's thesis on the Radon transform: http://petertoft.dk/PhD/index.html
 * While I changed his basic algorithm, the main idea is still the same and provides an excellent explanation.
 *
 * */
