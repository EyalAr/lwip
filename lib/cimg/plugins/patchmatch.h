/*
 #
 #  File        : PatchMatch_plugin.h
 #                ( C++ header file - CImg plug-in )
 #
 #  Description : Plugin implementing the Patch Match algorithm to use
 #                with the CImg library.
 #                ( http://cimg.sourceforge.net )
 #
 #  Copyright   : Olivier D'Hondt
 #                (https://sites.google.com/site/dhondtolivier/)
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

// Visualize optical flow maps with HSV color coding.
CImg<float> get_vizFlow(const float cutVal = 0) const {
  CImg<float> res(width(),height(),1,3);

  // Normalizing offset magnitude
  CImg<float> mag = get_norm();
  if (cutVal) mag.cut(0,cutVal);
  mag/=mag.max();

  // Filling HSV values
  cimg_forXY(*this,x,y) {
    const float
      xx = -(*this)(x,y,0),
      yy = -(*this)(x,y,1),
      H = cimg::max(180*((std::atan2(yy,xx)/M_PI)+1.0),0.0),
      S = mag(x,y),
      V = 1.0f;
    res(x,y,0) = H;
    res(x,y,1) = S;
    res(x,y,2) = V;
  }

  return res.HSVtoRGB();
}

// Distance between two patches
static T distPatch(const CImg<T> &img0, const CImg<T> &img1,
                   const int x0, const int y0,
                   const int x1, const int y1,
                   const int pSize) {
  T d2 = 0;
  cimg_forC(img0,c) for (int y = 0; y < pSize; ++y) for (int x = 0; x < pSize; ++x) {
      const T d = (img0(x0 + x, y0 + y, c) - img1(x1 + x, y1 + y, c));
      d2 += d*d;
    }
  return d2;
}

// Patch match algorithm
template<typename Tt>
CImg<T>& patchMatch(const CImg<Tt> &img0, const CImg<Tt> &img1,
                    int patchSize, const int nIter = 2,
                    CImgDisplay *const disp=0) {
  if (img0.spectrum() != img1.spectrum())
    throw CImgInstanceException("Images must have the same number of channels.");

  if (!patchSize % 2){
    ++patchSize;
    cimg::warn("Input patch size is even, adding 1.");
  }
  const int
    w0 = img0.width(),
    h0 = img0.height(),
    w1 = img1.width(),
    h1 = img1.height(),
    nChannels = img0.spectrum();

  CImg<Tt> imgrec, visu0, visu1; // used only for display purpose
  if (disp) {
    imgrec.assign(img0.width(),img0.height(),1,img0.spectrum(),0);
    visu0 = img0.get_resize(-100,-100,1,3);
    visu1 = img1.get_resize(-100,-100,1,3);
  }

  const int P = patchSize, H = P/2;

  // Zero padding borders
  CImg<Tt> img0big(w0+2*H, h0+2*H, 1, nChannels, 0);
  CImg<Tt> img1big(w1+2*H, h1+2*H, 1, nChannels, 0);

  // Try to penalize border patches
  img0big.rand(0,255);
  img1big.rand(0,255);

  img0big.draw_image(H, H, 0, 0, img0);
  img1big.draw_image(H, H, 0, 0, img1);

  CImg<T> off(w0, h0, 1, 2, 0);
  CImg<Tt> minDist(w0, h0, 1, 1, 0);

  // Initialize with random offsets
  cimg_forXY(off, x0, y0){
    int x1 = ((w1-1) * cimg::rand());
    int y1 = ((h1-1) * cimg::rand());
    off(x0, y0, 0) = x1 - x0;
    off(x0, y0, 1) = y1 - y0;
    minDist(x0, y0) = distPatch(img0big, img1big, x0, y0, x1, y1, P);
  }

  int xStart, yStart, xFinish, yFinish, inc;
  for (int n = 0; n < nIter; ++n) {
    std::fprintf(stderr,"Iteration %d\n",n+1);

    // at odd iterations, reverse scan order
    if (!(n%2)) { xStart = yStart = inc = 1; xFinish = w0; yFinish = h0; }
    else { xStart = w0-2; yStart = h0-2; xFinish = yFinish = inc = -1; }

    for (int y = yStart; y != yFinish; y=y+inc)
      for (int x = xStart; x != xFinish; x=x+inc) {
        // Propagation
        Tt d2 = 0.0;
        int x1 = x+off(x-inc,y,0), y1 = y+off(x-inc,y,1);
        if(x1 >= 0 && x1 < w1 && y1 >= 0 && y1 < h1){ // propagate only if inside img1 bounds
          d2 = distPatch(img0big, img1big, x, y, x1, y1, P);
          if (d2<minDist(x, y)) {
            minDist(x, y) = d2;
            off(x, y, 0) = off(x-inc, y, 0);
            off(x, y, 1) = off(x-inc, y, 1);
          }
        }
        x1 = x+off(x, y-inc, 0);
        y1 = y+off(x, y-inc, 1);
        if (x1 >= 0 && x1 < w1 && y1 >= 0 && y1 < h1) { // propagate only if inside img1 bounds
          d2 = distPatch(img0big, img1big, x, y, x1, y1, P);
          if (d2<minDist(x, y)) {
            minDist(x, y) = d2;
            off(x, y, 0) = off(x, y-inc, 0);
            off(x, y, 1) = off(x, y-inc, 1);
          }
        }

        // Randomized search
        int wSizX = w1 - 1, wSizY = h1 - 1;
        const T offXCurr = off(x, y, 0), offYCurr = off(x, y, 1);
        do{
          const int
            wMinX = cimg::max(0, x+offXCurr-wSizX/2),
            wMaxX = cimg::min(w1-1, x+offXCurr+wSizX/2);
          x1 = (wMaxX-wMinX) * cimg::rand() + wMinX;

          const int
            wMinY = cimg::max(0, y+offYCurr-wSizY/2),
            wMaxY = cimg::min(h1-1, y+offYCurr+wSizY/2);
          y1 = (wMaxY-wMinY) * cimg::rand() + wMinY;
          d2 = distPatch(img0big, img1big, x, y, x1, y1, P);

          if (d2 < minDist(x, y)) {
            minDist(x, y) = d2;
            off(x, y, 0) = x1-x;
            off(x, y, 1) = y1-y;
          }
          wSizX /= 2;
          wSizY /= 2;
        } while (wSizX >= 1 && wSizY >= 1);
        // If a pointer to a CImgDisplay is passed as the last argument
        // the output of the algorithm is displayed as an animation
        // !! It slows down the algorithm a lot !!
        if (disp) {
          if (x%(w0-1)==0)
            disp->display((visu0,
                           imgrec.reconstruct(img1, off).get_normalize(0,255).resize(-100,-100,1,3),
                           off.get_vizFlow(100),
                           visu1)).set_title("Iteration %d",n+1);
          if (disp->is_resized()) disp->resize();
        }
      }
  }
  return off.move_to(*this);
}

// Reconstruct an image from an offset map and a query image
template<typename Tt>
CImg<T> & reconstruct(const CImg<T> &qimg, const CImg<Tt> &off) {
  if(spectrum() != qimg.spectrum())
    throw CImgInstanceException("Images must have the same number of channels.");
  if(width() != off.width() || height() != off.height())
    throw CImgInstanceException("Offset map must have the same dimensions as input image.");
  cimg_forXY(off,x,y) {
    int qx = x + off(x, y, 0);
    int qy = y + off(x, y, 1);
    cimg_forC(qimg,c) (*this)(x,y,c) = qimg(qx,qy, c);
  }
  return (*this);
}

template<typename Tt>
CImg<T> get_reconstruct(const CImg<T> &qimg, const CImg<Tt> &off) const {
  return CImg<T>(*this, false).reconstruct(qimg, off);
}
