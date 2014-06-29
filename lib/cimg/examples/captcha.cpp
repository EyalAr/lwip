/*
 #
 #  File        : captcha.cpp
 #                ( C++ source file )
 #
 #  Description : Captcha images generator.
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

#ifndef cimg_debug
#define cimg_debug 1
#endif
#include "CImg.h"
using namespace cimg_library;
#undef min
#undef max

// Main procedure
//----------------
int main(int argc,char **argv) {

  // Read command line parameters
  //------------------------------
  cimg_usage("Simple captcha generator.");
  const char *file_o       = cimg_option("-o",(const char*)0,"Output image file");
  const bool add_border    = cimg_option("-b",true,"Add border to captcha image");
  const bool visu          = cimg_option("-visu",true,"Enable visualization if no output file");

  // Generate captcha text (6 char max).
  //------------------------------------
  const char *predef_words[] = {
    "aarrgh", "abacas", "abacus", "abakas", "abamps", "abased", "abaser", "abases", "abasia", "abated", "abater", "abates", "abatis", "abator",
    "baobab", "barbal", "barbed", "barbel", "barber", "barbes", "barbet", "barbie", "barbut", "barcas", "barded", "bardes", "bardic", "barege",
    "cavies", "cavils", "caving", "cavity", "cavort", "cawing", "cayman", "cayuse", "ceased", "ceases", "cebids", "ceboid", "cecity", "cedarn",
    "dicast", "dicers", "dicier", "dicing", "dicker", "dickey", "dickie", "dicots", "dictum", "didact", "diddle", "diddly", "didies", "didoes",
    "emails", "embalm", "embank", "embark", "embars", "embays", "embeds", "embers", "emblem", "embody", "emboli", "emboly", "embosk", "emboss",
    "fluffy", "fluids", "fluish", "fluked", "flukes", "flukey", "flumed", "flumes", "flumps", "flunks", "flunky", "fluors", "flurry", "fluted",
    "genome", "genoms", "genres", "genros", "gentes", "gentil", "gentle", "gently", "gentry", "geodes", "geodic", "geoids", "gerahs", "gerbil",
    "hotter", "hottie", "houdah", "hounds", "houris", "hourly", "housed", "housel", "houser", "houses", "hovels", "hovers", "howdah", "howdie",
    "inland", "inlays", "inlets", "inlier", "inmate", "inmesh", "inmost", "innage", "innate", "inners", "inning", "inpour", "inputs", "inroad",
    "joypop", "jubbah", "jubhah", "jubile", "judder", "judged", "judger", "judges", "judoka", "jugate", "jugful", "jugged", "juggle", "jugula",
    "knifer", "knifes", "knight", "knives", "knobby", "knocks", "knolls", "knolly", "knosps", "knotty", "knouts", "knower", "knowns", "knubby",
    "legate", "legato", "legend", "legers", "legged", "leggin", "legion", "legist", "legits", "legman", "legmen", "legong", "legume", "lehuas",
    "mammal", "mammas", "mammee", "mammer", "mammet", "mammey", "mammie", "mammon", "mamzer", "manage", "manana", "manats", "manche", "manege",
    "nihils", "nilgai", "nilgau", "nilled", "nimble", "nimbly", "nimbus", "nimmed", "nimrod", "ninety", "ninjas", "ninons", "ninths", "niobic",
    "offish", "offkey", "offset", "oftest", "ogdoad", "oghams", "ogival", "ogives", "oglers", "ogling", "ogress", "ogrish", "ogrism", "ohmage",
    "papaws", "papaya", "papers", "papery", "pappus", "papula", "papule", "papyri", "parade", "paramo", "parang", "paraph", "parcel", "pardah",
    "quasar", "quatre", "quaver", "qubits", "qubyte", "queans", "queasy", "queazy", "queens", "queers", "quelea", "quells", "quench", "querns",
    "raised", "raiser", "raises", "raisin", "raitas", "rajahs", "rakees", "rakers", "raking", "rakish", "rallye", "ralphs", "ramada", "ramate",
    "savory", "savour", "savoys", "sawers", "sawfly", "sawing", "sawlog", "sawney", "sawyer", "saxony", "sayeds", "sayers", "sayest", "sayids",
    "tondos", "toneme", "toners", "tongas", "tonged", "tonger", "tongue", "tonics", "tonier", "toning", "tonish", "tonlet", "tonner", "tonnes",
    "uredia", "uredos", "ureide", "uremia", "uremic", "ureter", "uretic", "urgent", "urgers", "urging", "urials", "urinal", "urines", "uropod",
    "villus", "vimina", "vinals", "vincas", "vineal", "vinery", "vinier", "vinify", "vining", "vinous", "vinyls", "violas", "violet", "violin",
    "webfed", "weblog", "wechts", "wedded", "wedder", "wedeln", "wedels", "wedged", "wedges", "wedgie", "weeded", "weeder", "weekly", "weened",
    "xystoi", "xystos", "xystus", "yabber", "yabbie", "yachts", "yacked", "yaffed", "yagers", "yahoos", "yairds", "yakked", "yakker", "yakuza",
    "zigged", "zigzag", "zillah", "zinced", "zincic", "zincky", "zinebs", "zinged", "zinger", "zinnia", "zipped", "zipper", "zirams", "zircon" };
  cimg::srand();
  const char *const captcha_text = predef_words[std::rand()%(sizeof(predef_words)/sizeof(char*))];

  // Create captcha image
  //----------------------

  // Write colored and distorted text
  CImg<unsigned char> captcha(256,64,1,3,0), color(3);
  char letter[2] = { 0 };
  for (unsigned int k = 0; k<6; ++k) {
    CImg<unsigned char> tmp;
    *letter = captcha_text[k];
    if (*letter) {
      cimg_forX(color,i) color[i] = (unsigned char)(128+(std::rand()%127));
      tmp.draw_text((int)(2+8*cimg::rand()),
                    (int)(12*cimg::rand()),
                    letter,color.data(),0,1,std::rand()%2?38:57).resize(-100,-100,1,3);
      const unsigned int dir = std::rand()%4, wph = tmp.width()+tmp.height();
      cimg_forXYC(tmp,x,y,v) {
        const int val = dir==0?x+y:(dir==1?x+tmp.height()-y:(dir==2?y+tmp.width()-x:tmp.width()-x+tmp.height()-y));
        tmp(x,y,v) = (unsigned char)cimg::max(0.0f,cimg::min(255.0f,1.5f*tmp(x,y,v)*val/wph));
      }
      if (std::rand()%2) tmp = (tmp.get_dilate(3)-=tmp);
      tmp.blur((float)cimg::rand()*0.8f).normalize(0,255);
      const float sin_offset = (float)cimg::crand()*3, sin_freq = (float)cimg::crand()/7;
      cimg_forYC(captcha,y,v) captcha.get_shared_row(y,0,v).shift((int)(4*std::cos(y*sin_freq+sin_offset)));
      captcha.draw_image(6+40*k,tmp);
    }
  }

  // Add geometric and random noise
  CImg<unsigned char> copy = (+captcha).fill(0);
  for (unsigned int l = 0; l<3; ++l) {
    if (l) copy.blur(0.5f).normalize(0,148);
    for (unsigned int k = 0; k<10; ++k) {
      cimg_forX(color,i) color[i] = (unsigned char)(128 + cimg::rand()*127);
      if (cimg::rand()<0.5f) copy.draw_circle((int)(cimg::rand()*captcha.width()),
                                              (int)(cimg::rand()*captcha.height()),
                                              (int)(cimg::rand()*30),
                                              color.data(),0.6f,~0U);
      else copy.draw_line((int)(cimg::rand()*captcha.width()),
                          (int)(cimg::rand()*captcha.height()),
                          (int)(cimg::rand()*captcha.width()),
                          (int)(cimg::rand()*captcha.height()),
                          color.data(),0.6f);
    }
  }
  captcha|=copy;
  captcha.noise(10,2);

  if (add_border) captcha.draw_rectangle(0,0,captcha.width()-1,captcha.height()-1,CImg<unsigned char>::vector(255,255,255).data(),1.0f,~0U);
  captcha = (+captcha).fill(255) - captcha;

  // Write output image and captcha text
  //-------------------------------------
  std::printf("%s\n",captcha_text);
  if (file_o) captcha.save(file_o);
  else if (visu) {
    CImgDisplay disp(CImg<unsigned char>(512,128,1,3,180).draw_image(128,32,captcha),captcha_text,0);
    while (!disp.is_closed() && !disp.key()) { disp.wait(); if (disp.is_resized()) disp.resize(disp).wait(100); }
  }
  return 0;
}

