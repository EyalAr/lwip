/*
 #
 #  File        : image2ascii.cpp
 #                ( C++ source file )
 #
 #  Description : A basic image to ASCII-art converter.
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

// Tell CImg not to use display capabilities.
#undef cimg_display
#define cimg_display 0
#include "CImg.h"
using namespace cimg_library;

/*---------------------------

  Main procedure

  --------------------------*/
int main(int argc,char **argv) {
  cimg_usage("A simple image to ASCII-art converter.\n\nUsage : image2ascii [options] image");

  // Read command line parameters
  const char *geom    = cimg_option("-g","79x40","Output size");
  const int alphabet  = cimg_option("-a",0,"Alphabet type (0=full, 1=numbers, 2=letters, 3=signs, 4=minimal");
  const bool invert   = cimg_option("-invert",false,"Invert image intensities");
  const float contour = (float)cimg_option("-contour",0.0f,"Use image contours higher than specified threshold");
  const float blur    = (float)cimg_option("-blur",0.8f,"Image pre-blur");
  const float sigma   = (float)cimg_option("-sigma",10.0f,"Font pre-blur");
  const char *file_i  = cimg_argument1(0,"-invert");
  int w = 79, h = 40;
  std::sscanf(geom,"%d%*c%d",&w,&h);
  if (cimg_option("-h",false,0)) std::exit(0);

  // Init fonts
  CImgList<> font_full = CImgList<>::font(13,false);
  font_full.remove(0,255);
  const int fw = font_full['A'].width(), fh = font_full['A'].height();
  CImgList<> font, font_blur;
  CImgList<unsigned char> font_code;

  switch (alphabet) {
  case 1: {
    font_code.insert(CImg<>::vector(' '));
    for (unsigned char l='0'; l<='9'; l++) font_code.insert(CImg<>::vector(l));
  } break;
  case 2: {
    font_code.insert(CImg<>::vector(' '));
    for (unsigned char l='A'; l<='Z'; l++) font_code.insert(CImg<>::vector(l));
  } break;
  case 3: {
    font_code.insert(CImg<>::vector(' '));
    font_code.insert(CImg<>::vector('-'));
    font_code.insert(CImg<>::vector('_'));
    font_code.insert(CImg<>::vector('|'));
    font_code.insert(CImg<>::vector('/'));
    font_code.insert(CImg<>::vector('\\'));
    font_code.insert(CImg<>::vector('+'));
    font_code.insert(CImg<>::vector('.'));
    font_code.insert(CImg<>::vector('*'));
    font_code.insert(CImg<>::vector('='));
    font_code.insert(CImg<>::vector(']'));
    font_code.insert(CImg<>::vector('['));
    font_code.insert(CImg<>::vector('('));
    font_code.insert(CImg<>::vector(')'));
    font_code.insert(CImg<>::vector('{'));
    font_code.insert(CImg<>::vector('}'));
    font_code.insert(CImg<>::vector('"'));
    font_code.insert(CImg<>::vector('!'));
    font_code.insert(CImg<>::vector('$'));
    } break;
  case 4: {
    font_code.insert(CImg<>::vector(' '));
    font_code.insert(CImg<>::vector('.'));
    font_code.insert(CImg<>::vector('/'));
    font_code.insert(CImg<>::vector('\\'));
    font_code.insert(CImg<>::vector('_'));
    font_code.insert(CImg<>::vector('_'));
    font_code.insert(CImg<>::vector('|'));
    } break;
  default: { for (unsigned char l=' '; l<='~'; l++) font_code.insert(CImg<>::vector(l)); } break;
  }
  cimglist_for(font_code,l) {
    font.insert(font_full(font_code[l](0)));
    font_blur.insert(font[l].get_resize(fw,fh,1,1).blur(sigma).normalize(0,255));
  }

  // Init images
  CImg<> img;
  if (!file_i) { float white[3] = { 255,255,255 }; img.assign().draw_text(0,0," CImg\nRocks !",white); }
  else img.assign(file_i);
  img.norm().resize(fw*w,fh*h);
  if (blur) img.blur(blur);
  if (contour>0) {
    CImgList<> grad = img.get_gradient("xy",4);
    img = (grad[0].pow(2) + grad[1].pow(2)).sqrt().normalize(0,100).threshold(contour);
  }
  img.normalize(0,255);
  if (invert) img = 255.0f-img;
  CImg<unsigned char> dest(w,h,1,1,0);

  // Render ASCII-art image, using a simple correlation method.
  CImg<> neigh;
  cimg_forY(dest,y) { cimg_forX(dest,x) {
    neigh = img.get_crop(x*fw,y*fh,(x+1)*fw,(y+1)*fh);
    float scoremin = 2e28f;
    unsigned int best = 0;
    cimglist_for(font_code,l) {
      const CImg<>& letter = font_blur[l];
      const float score = (float)((letter-neigh).pow(2).sum());
      if (score<scoremin) { scoremin = score; best = l; }
    }
    dest(x,y) = best;
    std::fprintf(stdout,"%c",font_code[dest(x,y)](0));
  }
  std::fprintf(stdout,"\n");
  }

  std::exit(0);
  return 0;
}
