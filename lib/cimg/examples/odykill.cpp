/*
 #
 #  File        : odykill.cpp
 #                ( C++ source file )
 #
 #  Description : Simple shoot-em-up game featuring the Robotvis/Odyssee Team !
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

#include "img/odykill.h"
#include "CImg.h"
using namespace cimg_library;

// Main procedure
//----------------
int main(int argc,char **argv) {

  // Create game graphics
  CImg<unsigned char> graphics[21] = {
    CImg<unsigned char>(data_tomato,100,100,1,3,false),
    CImg<unsigned char>(data_heart,100,100,1,3,false),
    CImg<unsigned char>(data_dynamite,100,100,1,3,false),
    CImg<unsigned char>(data_brain,100,100,1,3,false),
    CImg<unsigned char>(data_cdrom,100,100,1,3,false),
    CImg<unsigned char>(data_enemy,113,150,1,3,false),
    CImg<unsigned char>(data_enemy2,116,155,1,3,false),
    CImg<unsigned char>(data_enemy3,104,134,1,3,false),
    CImg<unsigned char>(data_enemy4,141,151,1,3,false),
    CImg<unsigned char>(data_enemy5,140,152,1,3,false),
    CImg<unsigned char>(data_enemy6,131,156,1,3,false),
    CImg<unsigned char>(data_enemy7,114,125,1,3,false),
    CImg<unsigned char>(data_enemy8,97,125,1,3,false),
    CImg<unsigned char>(data_enemy9,143,134,1,3,false),
    CImg<unsigned char>(data_enemy10,158,214,1,3,false),
    CImg<unsigned char>(data_enemy11,131,168,1,3,false),
    CImg<unsigned char>(data_enemy12,114,138,1,3,false),
    CImg<unsigned char>(data_enemy13,144,144,1,3,false),
    CImg<unsigned char>(data_enemy14,132,153,1,3,false),
    CImg<unsigned char>(data_enemy15,152,151,1,3,false),
    CImg<unsigned char>(data_enemy16,139,185,1,3,false),
  };
  CImg<> masks[21];
  const unsigned char black[] = { 0,0,0 }, white[] = { 255,255,255 };

  // Display weapon selection menu
  CImg<unsigned char> back0(640,480,1,3), title(data_title,294,94,1,3,true), choose(data_choose,524,49,1,3,true);
  back0.fill(0).draw_image(back0.width()/2-title.width()/2,30,title).draw_image(back0.width()/2-choose.width()/2,150,choose);
  CImgDisplay disp(back0,"OdyKill");
  int weapon=-1;

  while (!disp.is_closed() && !disp.button()) {
    weapon = -1;
    for (int k=0; k<5; k++) {
      const int mx = disp.mouse_x(), my = disp.mouse_y();
      if (!((mx-40)/110==k && my>250 && my<350)) back0.draw_image(40+k*110,250,graphics[k]/2.0);
      else back0.draw_image(40+k*110,250,graphics[weapon=k]);
    }
    CImg<unsigned char> tmp = CImg<unsigned char>().draw_text(0,0,
                                                              weapon==0?" Tomato   ":
                                                              weapon==1?"   Heart   ":
                                                              weapon==2?" Dynamite ":
                                                              weapon==3?"   Brain    ":
                                                              weapon==4?"  CD-Rom  ":
                                                              "          ",white,black,1,32).resize(-100,-100,1,1),
      tmp2 = tmp.get_blur(6).normalize(0,255).draw_image(tmp,0.5f);
    cimg_forC(back0,k) back0.draw_image(250,390,0,k,tmp2);

    disp.resize(disp).display(back0).wait();
    if (disp.is_keyCTRLLEFT() && disp.is_keyF()) disp.toggle_fullscreen();
    if (disp.is_closed() || disp.is_keyQ() || disp.is_keyESC()) std::exit(0);
  }
  disp.hide_mouse();

  /*---------------------------------

  Go !

  --------------------------------*/

  const CImg<unsigned char>
    background = CImg<unsigned char>(100,100,1,3,0).noise(100,2).draw_plasma().
    resize(back0.width(),back0.height(),1,3,5)/2.5;
  { for (unsigned int k=0; k<21; k++) {
    CImg<> tmp = graphics[k].resize(k<5?32:164,k<5?32:164,1,3);
    cimg_forXY(tmp,x,y) tmp(x,y)  = (tmp(x,y,0)==255 && tmp(x,y,1)==255 && tmp(x,y,2)==255)?0.0f:1.0f;
    masks[k]=tmp.get_shared_channel(0);
    graphics[k].resize(k<5?32:164,k<5?32:164,1,3,5);
  }}

  CImg<unsigned char> canvas(background);
  int n = 5+((int)(200*cimg::rand())%16);
  CImg<unsigned char> tomato = graphics[weapon], enemy = graphics[n];
  CImg<> m_tomato = masks[weapon], m_enemy = masks[n];

  double angle=0;
  int tomato_x=0,tomato_y=0,shooted=0;
  double enemy_x=-1000, enemy_y=-1000, enemy_z=-1000, tomato_z = 0, vx = 0, vy = 0, vz = 0, va = 0;
  double speed = cimg_option("-speed",5.0,"Speed");
  int timeleft = 2000, score = 0;
  CImg<unsigned char> r_enemy;

  // Main loop
  while (timeleft && !disp.is_closed() && !disp.is_keyESC() && !disp.is_keyQ()) {
    --timeleft;
    const int mx = disp.mouse_x()*back0.width()/disp.width(), my = disp.mouse_y()*back0.height()/disp.height();

    // Handle object motion
    if (tomato_z>0) {
      tomato_z+=0.07; tomato_y -= (int)(20*std::cos(cimg::PI/7 + tomato_z*cimg::PI));
      if (tomato_z>=1) { tomato_z=0; tomato_x = mx; tomato_y = my; }
    }
    if (!shooted) { enemy_x +=vx; enemy_y +=vy; enemy_z +=vz; }
    else {
      va = 10;
      enemy_y += vy;
      vy += 2;
      tomato_z = 0;
      if (enemy_y>5*canvas.height()/4) {
        shooted = 0;
        int n = 5 + ((int)(200*cimg::rand())%16);
        enemy = graphics[n];
        m_enemy = masks[n];
        enemy_x=cimg::crand()*1e8; enemy_y=cimg::crand()*1e8; enemy_z=cimg::crand()*1e8;
        va = angle = 0;
      }
    }

    if (enemy_x<0) { enemy_x=0; vx = speed*cimg::crand(); }
    if (enemy_x>canvas.width()) { enemy_x=canvas.width(); vx = speed*cimg::crand(); }
    if (enemy_y<0) { enemy_y=0; vy = speed*cimg::crand(); }
    if (!shooted && enemy_y>canvas.height()) { enemy_y=canvas.height(); vy = speed*cimg::crand(); }
    if (enemy_z<0.1) { enemy_z = 0.1; vz = speed*0.01*cimg::crand(); }
    if (enemy_z>0.7) { enemy_z = 0.7; vz = speed*0.01*cimg::crand(); }
    angle+=va;

    // Handle mouse interaction
    if (!disp.button()) {
      if (tomato_z==0) {
        tomato_x = mx; tomato_y = my;
      }
    } else tomato_z +=0.0001;

    // Detect shooting
    if (cimg::abs(tomato_z-enemy_z)<0.1) {
      if (tomato_x>enemy_x-r_enemy.width()/2 && tomato_x<enemy_x+r_enemy.width()/2 &&
      tomato_y>enemy_y-r_enemy.height()/2 && tomato_y<enemy_y+r_enemy.height()/2) {
        score++;
        shooted = 1;
      }
    }

    // Draw into canvas
    canvas = background;
    r_enemy = enemy.get_resize((int)(8+enemy.width()*(1-enemy_z)),(int)(8+enemy.height()*(1-enemy_z)),-100,-100);
    CImg<> rm_enemy = m_enemy.get_resize(r_enemy.width(),r_enemy.height());
    CImg<unsigned char> r_tomato  = tomato.get_resize((int)(8+tomato.width()*(1-tomato_z)),(int)(8+tomato.height()*(1-tomato_z)),-100,-100);
    CImg<> rm_tomato = m_tomato.get_resize(r_tomato.width(),r_tomato.height());

    if (angle!=0) { r_enemy.rotate((float)angle,0,0); rm_enemy.rotate((float)angle,0,0); cimg_forXY(r_enemy,x,y) r_enemy(x,y,0) = (r_enemy(x,y,0)+255)/2; }
    r_enemy*=(1-(enemy_z-0.1)/1.6);
    r_tomato*=(1-tomato_z/1.6);
    rm_enemy*=(1-(enemy_z-0.1)/1.6);

    if (enemy_z>tomato_z) {
      canvas.draw_image((int)(enemy_x - r_enemy.width()/2),
                        (int)(enemy_y - r_enemy.height()/2),
                        r_enemy,rm_enemy);
      if (tomato_x>=0) canvas.draw_image(tomato_x - r_tomato.width()/2,
                                         tomato_y - r_tomato.height()/2,
                                         r_tomato,rm_tomato);
    }
    else {
      if (tomato_x>=0) canvas.draw_image(tomato_x - r_tomato.width()/2,
                                         tomato_y - r_tomato.height()/2,
                                         r_tomato,rm_tomato);
      canvas.draw_image((int)(enemy_x - r_enemy.width()/2),
                        (int)(enemy_y - r_enemy.height()/2),
                        r_enemy,rm_enemy);
    }
    canvas.draw_text(1,1," Time left %d, Score = %d",white,0,0.5f,24,timeleft,score);
    disp.resize(disp).display(canvas).wait(25);
    if (disp.is_keyCTRLLEFT() && disp.is_keyF()) disp.toggle_fullscreen();
  }

  std::fprintf(stderr,"\n\n YOUR SCORE : %d\n\n\n",score);

  return 0;
}
