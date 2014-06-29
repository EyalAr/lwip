/*
 #
 #  File        : use_patchmatch.cpp
 #                ( C++ source file )
 #
 #  Description : Example of use for the CImg plugin 'plugins/patchmatch.h'.
 #                This file is a part of the CImg Library project.
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

#define cimg_plugin "plugins/patchmatch.h"
#include "CImg.h"

using namespace cimg_library;
using namespace std;

int main(int argc, char **argv) {
  cimg_usage("PatchMatch: approximate nearest neighbour patch matching between two images img0 and img1.");
  cimg_help("Input / Output parameters:\n--------------------------");
  const char *file0 = cimg_option("-i0", "img/sh0r.pgm", "Image 0");
  const char *file1 = cimg_option("-i1", "img/sh1r.pgm", "Image 1");
  const char *fileout = cimg_option("-o", (char*)0, "Output image of offsets (channel 0: x offset, channel 1: y offset)");
  const int P = cimg_option("-s", 7, "Patch size");
  const int N = cimg_option("-n", 5, "Number of iterations");
  const bool DISP = cimg_option("-d", true, "Real-time display (slows down the process)");
  const bool VERB = cimg_option("-v", true, "Verbose");

  if (!file0 || !file1) {
    std::fprintf(stderr,"Must specify images with -i0 and -i1 options (-h for help).\n");
    exit(0);
  }

  srand (time(NULL));


  CImg<int> img0(file0);
  CImg<int> img1(file1);
  CImg<int> imgrec(img0, "xyzc", 0);

  CImg<int> off;

  if(VERB){
    std::fprintf(stderr,
                 "\n--------\n"
                 "PatchMatch: approximate nearest neighbor search\n"
                 "From left to right: \n"
                 "(a) Image 0 \n"
                 "(b) Reconstructed version of image 0 from image 1 patches\n"
                 "(c) HSV displacement map (hue encodes the angle, saturation the magnitude)\n"
                 "(d) Image 1\n"
                 "--------\n\n");
  }

  // Optional display for real-time view
  CImgDisplay *disp = 0;
  if (DISP) disp = new CImgDisplay;
  off.patchMatch(img0, img1, P, N, disp);
  delete disp;
  std::fprintf(stderr,"Done!\n\n");

  (img0,
   imgrec.reconstruct(img1, off),
   off.get_vizFlow(100),
   img1).display("PatchMatch: final result.", false);


  if(fileout)
    off.save_cimg(fileout);
}
