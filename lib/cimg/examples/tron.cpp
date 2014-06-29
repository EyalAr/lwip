/*
 #
 #  File        : tron.cpp
 #                ( C++ source file )
 #
 #  Description : A clone of the famous (and very simple) Tron game.
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

#include "CImg.h"
using namespace cimg_library;

// Main procedure
//----------------
int main(int argc, char **argv) {

  // Print usage, help and retrieve command line options
  //-----------------------------------------------------
  cimg_usage("A very simple Tron game, using the CImg Library");
  cimg_help("--- Quick help ----------------------------\n"
            " Player 1 (blue) :\n"
            " Use keys 'Z' (up), 'S' (down), 'Q' (left)\n"
            "     and 'D' (right) to control your player.\n"
            "     Right 'CONTROL' key enables turbospeed\n"
            " Player 2 (red) : \n"
            "     Use arrow keys to control your player.\n"
            "     'TAB' key enables turbospeed.\n"
            "-------------------------------------------");

  const char *geom      = cimg_option("-g","300x300","Size of the game board");
  const int delay       = cimg_option("-s",10,"Game speed (lower value means faster)");
  const bool twoplayers = !cimg_option("-1",false,"One player only");
  const int zoom        = cimg_option("-z",1,"Zoom factor");
  const bool full       = cimg_option("-f",false,"Fullscreen mode");
  unsigned int W = 400, H = 400;
  std::sscanf(geom,"%u%*c%u",&W,&H);

  // Define game colors and variables
  //----------------------------------
  const unsigned char blue[] = { 128,200,255}, red[] = { 255,0,0 }, white[] = { 255,255,255 };
  int score1=0, score2=0, round_over=0, ix1=-1, iy1=-1, x1=0, y1=0, u1=0, v1=0, ix2=-1, iy2=-1, x2=0, y2=0, u2=0, v2=0;
  bool start_round = true, turbo1 = false, turbo2 = false;

  // Create background image
  //--------------------------
  CImg<unsigned char> background, img;
  background.assign(64,64,1,3,0).noise(60).draw_plasma().resize(W,H).blur(2).normalize(0,128).draw_rectangle(0,0,W-1,H-1,white,1.0f,~0U);

  // Open display window
  //---------------------
  CImgDisplay disp(background,"* CImg-Tron *");
  if (zoom>1) disp.resize(-100*zoom,-100*zoom);
  if (full) disp.toggle_fullscreen().display(background);

  // Start main game loop
  //----------------------
  while (!disp.is_closed() && !disp.is_keyESC()) {

    // Init new game round if necessary
    //----------------------------------
    if (start_round) {

      // Init game variables
      round_over = 0;
      ix1=-1; iy1=-1; x1 = 10;   y1 = 10;   u1 = 1;  v1 = 0; turbo1 = false;
      ix2=-1; iy2=-1; x2 = W-11; y2 = H-11; u2 = -1; v2 = 0; turbo2 = false;
      img = background;
      start_round = false;

      // Display a simple pre-round page
      CImg<unsigned char> logo, pressakey;
      logo.draw_text(0,0," CImg-Tron ",white,0,1,33).resize(-100,-100,1,3);
      CImg<unsigned char> tmp = (+background).draw_image((W-logo.width())/2,(H-logo.height())/2-20,logo,logo.get_channel(0).dilate(6).normalize(0,1)).
        draw_text(W/2-60,H/2+10,"Blue ( %u )",blue,0,1,13,score1).
        draw_text(W/2+10,H/2+10,"Red ( %u )",red,0,1,13,score2);
      pressakey.draw_text(0,0,"* Press a key to start round *",white);
      for (float i = 0; i<1; i+=0.05f) ((+tmp)*=i).display(disp.wait(20));
      disp.flush();
      for (unsigned long t = 0; !disp.key() && !disp.is_closed(); ++t) {
        if (!(t%10)) { if (t%20) disp.display(tmp); else disp.display((+tmp).draw_image(W/2-70,H/2+50,pressakey,pressakey,1,255)); }
        if (disp.wait(20).is_resized()) disp.resize(disp);
      }
      if (disp.is_keyESC()) disp.flush();
    }

    // Test collision between players and borders
    if (x1<0 || x1>=img.width() || y1<0 || y1>=img.height() ||
        img(x1,y1,0)!=background(x1,y1,0) ||
        img(x1,y1,1)!=background(x1,y1,1) ||
        img(x1,y1,2)!=background(x1,y1,2) ||
        ((ix1>=0 || iy1>=0) && (img(ix1,iy1,0)!=background(ix1,iy1,0) ||  // Collision test for turbo mode
                                img(ix1,iy1,1)!=background(ix1,iy1,1) ||
                                img(ix1,iy1,2)!=background(ix1,iy1,2)))) { round_over=1; score2++; }
    if (twoplayers) {
      if (x2<0 || x2>=img.width() || y2<0 || y2>=img.height() ||
          img(x2,y2,0)!=background(x2,y2,0) ||
          img(x2,y2,1)!=background(x2,y2,1) ||
          img(x2,y2,2)!=background(x2,y2,2) ||
          ((ix2>=0 || iy2>=0) && (img(ix2,iy2,0)!=background(ix2,iy2,0) ||  // Collision test for turbo mode
                                  img(ix2,iy2,1)!=background(ix2,iy2,1) ||
                                  img(ix2,iy2,2)!=background(ix2,iy2,2)))) { round_over=2; score1++; }
    }

    // Draw new players positions
    img.draw_point(x1,y1,blue);
    if (ix1>=0 && iy1>=0) img.draw_point(ix1,iy1,blue);
    if (twoplayers) {
      img.draw_point(x2,y2,red);
      if (ix2>=0 && iy2>=0) img.draw_point(ix2,iy2,red);
    }
    if (disp.is_resized()) disp.resize(disp);
    img.display(disp);

    // Update players positions
    x1+=u1; y1+=v1;
    if (turbo1) { ix1 = x1; iy1 = y1; x1+=u1; y1+=v1; } else { ix1 = iy1 = -1; }
    if (twoplayers) {
      x2+=u2; y2+=v2;
      if (turbo2) { ix2 = x2; iy2 = y2; x2+=u2; y2+=v2; } else { ix2 = iy2 = -1; }
    }

    // Test keyboard events
    int nu1 = u1, nv1 = v1, nu2 = u2, nv2 = v2;
    if (disp.is_keyARROWLEFT())  { nu1 = -1; nv1 = 0; }
    if (disp.is_keyARROWRIGHT()) { nu1 = 1; nv1 = 0; }
    if (disp.is_keyARROWUP())    { nu1 = 0; nv1 = -1; }
    if (disp.is_keyARROWDOWN())  { nu1 = 0; nv1 = 1; }
    turbo1 = disp.is_keyCTRLRIGHT();
    if (twoplayers) {
      if (disp.is_keyQ()) { nu2 = -1; nv2 = 0; }
      if (disp.is_keyD()) { nu2 = 1; nv2 = 0; }
      if (disp.is_keyZ()) { nu2 = 0; nv2 = -1; }
      if (disp.is_keyS()) { nu2 = 0; nv2 = 1; }
      turbo2 = disp.is_keyTAB();
    }
    if (nu1!=-u1 && nv1!=-v1) { u1 = nu1; v1 = nv1; }
    if (nu2!=-u2 && nv2!=-v2) { u2 = nu2; v2 = nv2; }

    // Check if round is over.
    if (round_over) {
      const int xc = round_over==1?x1:x2, yc = round_over==1?y1:y2;
      for (int r=0; r<50; r+=3) img.draw_circle(xc,yc,r,round_over==1?blue:red,r/300.0f).display(disp.wait(20));
      for (int rr=0; rr<50; rr+=3)
        ((+img)*=(50-rr)/50.0f).draw_circle(xc,yc,(50+rr),round_over==1?blue:red,1/6.0f).display(disp.wait(20));
      start_round = true;
    }

    // Wait a small amount of time
    disp.wait(delay);
  }
  return 0;
}
