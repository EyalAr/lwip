/*
 #
 #  File        : tetris.cpp
 #                ( C++ source file )
 #
 #  Description : A CImg version of the famous Tetris game.
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

#include "img/tetris.h"
#include "CImg.h"
using namespace cimg_library;

// Main procedure
//----------------
int main(int argc,char **argv) {

  // Read command line argument (if any)
  cimg_usage("An implementation of the well known 'Tetris' game with CImg.");
  unsigned int
    blocdim = cimg_option("-blocdim",18,"Sprite bloc size"),
    speed   = cimg_option("-speed",20,"Initial speed"),
    level   = cimg_option("-level",0,"Level");
  const char *geometry = cimg_option("-g","12x20","Size of the board");
  unsigned int bwidth = 12,bheight = 20;
  std::sscanf(geometry,"%u%*c%u",&bwidth,&bheight);

  const CImg<unsigned char> dlogo = CImg<unsigned char>(data_logo,128,96,1,3,true);
  if (cimg::dialog("CImg Tetris",
                   "Welcome to the CImg version of Tetris.\n"
                   "( by David Tschumperle )\n\n"
                   "Press 'Start' when you are ready to play !","Start","Quit",0,0,0,0,dlogo,true)) std::exit(0);

  // Create sprite, background graphics and initial board data
  const CImgList<unsigned char> pieces = CImgList<unsigned char>().
    insert(CImg<unsigned char>(3,2).fill(1,1,1,0,0,1)).
    insert(CImg<unsigned char>(3,2).fill(2,2,2,2,0,0)).
    insert(CImg<unsigned char>(2,2).fill(3,3,3,3)).
    insert(CImg<unsigned char>(4,1).fill(4,4,4,4)).
    insert(CImg<unsigned char>(3,2).fill(5,5,0,0,5,5)).
    insert(CImg<unsigned char>(3,2).fill(0,6,6,6,6,0)).
    insert(CImg<unsigned char>(3,3).fill(0,7,0,7,7,7,0,7,0)).
    insert(CImg<unsigned char>(2,1).fill(8,8)).
    insert(CImg<unsigned char>(3,2).fill(9,9,9,0,9,0)).
    insert(CImg<unsigned char>(2,2).fill(10,10,0,10)).
    insert(CImg<unsigned char>(3,1).fill(11,11,11));

  CImg<unsigned char> board(bwidth,bheight,1,1,0), background(board.width()*blocdim,board.height()*blocdim,1,3,0);
  (background.noise(30).draw_plasma().noise(30).deriche(5,0,'y').shift(0,-background.height()/2,0,0,2).deriche(5,0,'y'))/=1.5f;
  if (level) (board.get_shared_rows(board.height()-level,board.height()-1,0,0).noise(100))%=pieces.size()+1;

  // Create a set of small gradient-colored blocs used to draw the pieces.
  CImgList<unsigned char> blocs(pieces.size(),blocdim,blocdim,1,3);
  cimglist_for(blocs,l) {
    CImg<unsigned char> color = CImg<unsigned char>(3,1,1,1,128).noise(127,1).cut(120,255);
    float val;
    cimg_forXYC(blocs[l],x,y,k) blocs[l](x,y,k) = (unsigned char)((val=(color[k]*0.7f*(x+y+5)/blocdim))>255?255:val);
    blocs[l].draw_line(0,0,0,blocdim-1,(color>>1).data()).draw_line(0,blocdim-1,blocdim-1,blocdim-1,(color>>1).data());
    color = (CImg<unsigned int>(color)*=2).cut(0,255);
    blocs[l].draw_line(0,0,(int)blocdim-1,0,color.data()).draw_line(blocdim-1,0,blocdim-1,blocdim-1,color.data());
  }

  // Initialize window display and enter the main event loop
  CImgDisplay disp(background,"CImg Tetris",0,false,true);
  disp.move((CImgDisplay::screen_width() - disp.width())/2,
            (CImgDisplay::screen_height() - disp.height())/2).hide_mouse();
  const unsigned char white[3]={ 255, 255, 255 };
  CImg<unsigned char> visu, nboard, piece, next, next_mask;
  int cx = -1, cy = -1, cn = -1, nn = rand()%pieces.size(), time = 0, score = 0;
  bool gameover = false, pause = false;

  while (!gameover && !disp.is_closed() && !disp.is_keyESC() && !disp.is_keyQ()) {

    if (!pause) {

      // Draw the board on the display window.
      nboard = board; visu = background;
      if (cx>=0 && cy>=0) cimg_forXY(piece,x,y) if (piece(x,y)) nboard(cx-piece.width()/2+x,cy-piece.height()/2+y)=piece(x,y);
      cimg_forXY(board,xx,yy) if (nboard(xx,yy)) visu.draw_image(xx*blocdim,yy*blocdim,blocs[nboard(xx,yy)-1]);
      visu.draw_text(5,5,"Lines : %d",white,0,1,13,score,nn).draw_text(visu.width()-75,5,"Next :",white,0,1,13);
      if (next) visu.draw_image(visu.width()-next.width()-2,10-next.height()/2,next,next_mask).display(disp.wait(20));

      if (cn<0) {

        // Introduce a new piece on the board (if necessary) and create representation of the next piece
        board = nboard;
        piece = pieces[cn=nn];
        nn = rand()%pieces.size();
        cx = board.width()/2;
        cy = piece.height()/2;
        next = CImg<unsigned char>(pieces[nn].width()*blocdim,pieces[nn].height()*blocdim,1,3,0);
        cimg_forXY(pieces[nn],xi,yi) if (pieces[nn](xi,yi)) next.draw_image(xi*blocdim,yi*blocdim,blocs[pieces[nn](xi,yi)-1]);
        next_mask = next.resize(-50,-50).get_norm().threshold(0);

        // Detect tetris lines and do line removal animation if found.
        cimg_forY(board,yyy) {
          int Y = yyy*blocdim, line = 1;
          cimg_forX(board,xxx) if (!board(xxx,yyy)) line=0;
          if (line) {
            board.draw_image(0,1,board.get_crop(0,0,board.width()-1,yyy-1));
            if (!((++score)%1) && speed>1) --speed;
            for (float alpha=0; alpha<=1; alpha+=0.07f)
              CImg<unsigned char>(visu).draw_image(0,Y,background.get_crop(0,Y,visu.width()-1,Y+blocdim-1),alpha).display(disp.wait(20));
            visu.draw_image(0,Y,background.get_crop(0,Y,visu.width()-1,Y+blocdim-1));
          }
        }
      }

      // Handle motion & collisions
      const int ox = cx, oy = cy;
      bool rotated = false, collision;
      switch (disp.key()) {
      case cimg::keyP: pause = true; break;
      case cimg::keyARROWUP: piece.rotate(90); rotated = true; disp.set_key(); break;
      case cimg::keyARROWLEFT: --cx;  disp.set_key(); break;
      case cimg::keyARROWRIGHT: ++cx;  disp.set_key(); break;
      }
      if (cx - piece.width()/2<0) cx = piece.width()/2;
      if (cy - piece.height()/2<0) cy = piece.height()/2;
      if (cx + (piece.width()-1)/2>=board.width()) cx = board.width() - 1 - (piece.width()-1)/2;

      // Detect collision along the X axis
      collision = false; cimg_forXY(piece,i,j) if (piece(i,j) && board(cx-piece.width()/2+i,cy-piece.height()/2+j)) collision = true;
      if (collision) { cx=ox; if (rotated) piece.rotate(-90); }

      if (disp.key()==cimg::keyARROWDOWN || !((++time)%speed)) { ++cy; disp.set_key(); }
      // Detect collisiong along the Y axis
      collision = false;
      cimg_forXY(piece,ii,jj)
        if (piece(ii,jj) && (cy-piece.height()/2+jj>=board.height() || board(cx-piece.width()/2+ii,cy-piece.height()/2+jj))) collision = true;
      if (collision || cy+(piece.height()-1)/2>=board.height()) { cy = oy; cn = -1; }
      if (collision && cy==piece.height()/2) gameover = true;
    } else {

      // If game is paused (key 'P'), do a little text animation
      float A = 0, B = 0;
      CImg<float> pauselogo = CImg<unsigned char>().draw_text(0,0,"Game Paused\nPress a key",white);
      disp.set_key(); while (!disp.is_closed() && !disp.key()) {
        const CImg<float> pauserotated = pauselogo.get_rotate((float)(30*std::sin(A)),1,0).
          resize((int)(-150-80*std::sin(B)),(int)(-150-80*std::sin(B)));
        A+=0.08f; B+=0.043f;
        CImg<unsigned char>(background).
          draw_image((background.width()-pauserotated.width())/2,
                     (background.height()-pauserotated.height())/2,
                     pauserotated.get_resize(-100,-100,1,3,2),pauserotated,1,255).display(disp.wait(20));
        if (disp.is_resized()) disp.resize();
      }
      disp.set_key();
      pause = false;
    }
    background.shift(0,-20/(int)speed,0,0,2);
    if (disp.is_resized()) disp.resize();
  }

  // End of game reached, display the score and do a 'game over' animation
  cimg_forXYC(visu,x,y,k) if (x%2 || y%2) visu(x,y,k) = 0;
  visu.display(disp);
  char tmp[1024];
  std::sprintf(tmp,"Game Over !\n\nYour score : %d",score);
  cimg::dialog("CImg Tetris",tmp,"Quit");
  return 0;
}
