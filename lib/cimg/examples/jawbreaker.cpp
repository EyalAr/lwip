/*
 #
 #  File        : jawbreaker.cpp
 #                ( C++ source file )
 #
 #  Description : A funny game featuring small colored balls.
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
#undef min
#undef max

// Main procedure
//----------------
int main(int argc, char **argv) {

  // Display help (if option '-h' or '--help' specified) and retrieve program arguments
  cimg_usage("A small and funny game featuring colored balls.\n             (by David Tschumperle).");
  const char *score_file = cimg_option("-s",(char*)0,"Specify score file to use (0=default file).");
  cimg_help("\n"
            "** Quick Help *********************************************************\n\n"
            "Goal : Delete the board by clicking on groups of adjacent colored balls\n"
            "       (a group is made of at least two balls with the same color).\n"
            "       Suppressing large sets gives higher scores.\n\n"
            "In-game keys : - BACKSPACE or SPACE = Undo last move\n"
            "               - CTRL+F = Toggle fullscreen mode\n"
            "               - ESC   = Quit application\n"
            "               - Q     = End current game\n\n"
            "*********************************************************************");

  // Load score file if available
  CImgList<unsigned int> score_history;
  char filename_history[1024];
  std::sprintf(filename_history,"%s%s",score_file?"":cimg::temporary_path(),score_file?score_file:"/jawbreaker.score");
  std::FILE *file = std::fopen(filename_history,"r");
  if (file) { std::fclose(file); score_history = CImg<unsigned int>::get_load_dlm(filename_history)<'y'; }

  // Create ball graphics
  const unsigned int W = 12, H = 14, Wi = (W<<5), Hi = (H<<5);
  unsigned int score = 0, previous_score = 0, shape_score = 0,
    best_score = score_history?score_history.max():0U;

  const CImg<> colors(3,7,1,1, 255,255,255, 205,0,230, 0,235,0, 235,255,0, 235,0,0, 0,128,255, 450,350,300);
  const unsigned char
    white[] = { 255,255,255 }, orange[] = { 255,128,64 }, yellow[] = { 255,255,64 }, red[] = { 255,64,64 }, six = 6;
  CImgList<> balls0(7,32,32,1,3,0);
  cimglist_for(balls0,l) if (l) {
    balls0[l].draw_circle(16,16,14,colors.data(0,l));
    cimg_forXYC(balls0[l],x,y,k) if (balls0(l,x,y,k)) (balls0(l,x,y,k)*=(32-x+y)/60.0f)+=20;
    balls0[l].draw_circle(16,16,14,colors.data(0,l),0.5f,~0U).
      draw_circle(20,10,5,colors.data(),0.2f).draw_circle(22,8,2,colors.data(),0.4f).cut(0,255);
  }

  // Create background graphics
  CImgList<unsigned char> balls(balls0);
  CImg<unsigned char>
    mask =  balls[1].get_cut(0,1).channel(0).dilate(3),
    background = CImg<unsigned char>(Wi,Hi,1,3,0).
    noise(255,1).blur(6,20,0,true).equalize(100,0,255).blur(2,4,0,true);
  background.get_shared_channel(0)/=4; background.get_shared_channel(1)/=8; background.get_shared_channel(2)/=2;

  // Begin user-interaction loop.
  CImg<unsigned char> board, previous_board, selected_board, shape, img(background);
  CImgDisplay disp(img.width(),img.height(),"Jawbreaker",0);
  bool redraw = true, gameover = false, title = true;
  for (float opac = 0.0f; !disp.is_closed(); ) {

    // Init board
    if (!board) {
      (++((board.assign(W,H,1,1,5).noise(5,1))%=5)).get_shared_row(0).fill(0);
      opac = (float)(score = previous_score = shape_score = 0);
      gameover = false; redraw = title = true;
      previous_board = board;
    }

    // Draw graphical board
    if (redraw) {
      (img=background).draw_text(2,2,"Score : %u",yellow,0,0.7f,24,score).
        draw_text(Wi-90,2,"Best : %u",orange,0,0.9f,17,best_score);
      if (selected_board) {
        cimg_forXY(selected_board,x,y) if (selected_board(x,y))
          img.draw_image(x<<5,y<<5,balls[selected_board(x,y)],mask);
      } else cimg_forXY(board,x,y) if (board(x,y)) img.draw_image(x<<5,y<<5,balls[board(x,y)],mask);
      if (title) {
        CImg<unsigned char> text1, text2;
        text1.draw_text(0,0,"- Jawbreaker -",white,0,1,48).resize(-100,-100,1,3);
        text2.draw_text(0,0,"Press button to start",yellow,0,1,24).resize(-100,-100,1,3);
        (img/=2).draw_image((Wi-text1.width())/2,
                            (Hi-text1.height())/2,
                            text1,text1.get_dilate(7),1,255).
          draw_image((Wi-text2.width())/2,
                     (Hi+text1.height()+10)/2,
                     text2,text2.get_dilate(5),0.7f,255);
        for (float i = 1; i<10 && !disp.is_keyESC(); i+=0.25)
          disp.display(img.get_crop((int)(Wi*(0.5f-i*i/200.0f)),(int)(Hi*(0.5f-i*i*i*i/20000.0f)),
                                    (int)(Wi*(0.5f+i*i/200.0f)),(int)(Hi*(0.5f+i*i*i*i/20000.0f)))).wait(20);
      }
    }
    if ((opac-=0.06f)>0) disp.display((+img).draw_text(disp.mouse_x()-8,disp.mouse_y()-80+(int)(60*opac),"+%u",
                                                       white,0,(float)std::sqrt(opac),32,shape_score)).wait(20);
    else { if (redraw) { disp.display(img); redraw = false; } else disp.wait(); }

    // Handle key and window events
    if (disp.is_resized()) disp.resize(disp);
    if (disp.is_keyBACKSPACE() || disp.is_keySPACE()) {
      board = previous_board; score = previous_score; selected_board.assign(); redraw = true; disp.set_key();
    }
    if (disp.is_keyQ()) { gameover = true; disp.set_key(); }
    if (disp.is_keyESC()) disp.close();
    if (disp.is_keyCTRLLEFT() && disp.is_keyF()) disp.toggle_fullscreen().display(img);

    // Handle ball selection and removal
    const int x = disp.mouse_x()*board.width()/disp.width(), y = disp.mouse_y()*board.height()/disp.height();
    if (disp.button()&1 && x>=0 && y>=0) {
      if (title) { title = false; redraw = true; } else {
        if (!board(x,y)) { selected_board.assign(); redraw = true; }
        else {
          if (!selected_board || selected_board(x,y)!=6) {
            (selected_board=board).draw_fill(x,y,0,&six,1,shape);
            if ((shape_score=(unsigned int)shape.sum())<2) selected_board.assign();
            else { shape_score-=1; shape_score*=shape_score; opac = 1.0f; redraw = true; }
          } else {
            selected_board.assign();
            previous_board = board;
            previous_score = score;
            score += shape_score;
            board&=--shape;
            redraw = true;

            // Handle board modification due to ball removal
            for (int pmax = board.width(), p = 0; p<pmax; ++p) {
              for (int q = board.height()-1, qs = q; q>=0; --q) {
                while (qs>=0 && !board(p,qs)) --qs;
                board(p,q) = (qs>=0?board(p,qs--):0);
              }
              if (!board(p,board.height()-1)) {
                board.draw_image(p,board.get_crop(p,0,board.width()-1,board.height()-1).shift(-1));
                if (p<pmax) { p--; pmax--; }
              }
            }

            // Test possible end of the game
            gameover = true;
            cimg_forXY(board,x,y)
              if (board(x,y) && ((y && board(x,y)==board(x,y-1)) || (x && board(x,y)==board(x-1,y)))) gameover = false;
          }
        }
      }
      disp.set_button();
    }

    // If game is over...
    if (gameover && opac<=0) {
      CImg<unsigned char> text1, text2, text3, text4, text5, text6;
      text1.draw_text(0,0,"Game Over !",white,0,1,48).resize(-100,-100,1,3);
      const unsigned int remaining_balls = (unsigned int)board.get_cut(0,1).sum();
      if (remaining_balls<8) {
        const unsigned int bonus = (22-2*remaining_balls)*10;
        score += bonus;
        text2.draw_text(0,0,"Jawbreaker Bonus : +%u",white,0,1,24,bonus);
      }
      score_history.insert(CImg<unsigned int>::vector(score));
      text3.draw_text(0,0,"Final score : %u",yellow,0,1,24,score).resize(-100,-100,1,3);
      text4.draw_text(0,0,score>best_score?"** New record ! **":"Best score : %u",
                      orange,0,1,24,score>best_score?score:best_score).resize(-100,-100,1,3);
      text5.draw_text(0,0,"Average score : %u",red,0,1,24,
                      score_history?(unsigned int)(score_history>'x').mean():0U).resize(-100,-100,1,3);
      text6.draw_text(0,0,"Games played : %u",red,0,1,24,score_history.size()).resize(-100,-100,1,3);
      if (score>best_score) best_score = score;

      unsigned int yt = (Hi-text1.height())/2-20;
      (img/=2).draw_image((Wi-text1.width())/2,yt,text1,text1.get_dilate(7),1,255); yt+=80;
      if (text2) { img.draw_image((Wi-text2.width())/2,yt,text2,text2.get_dilate(5),1,255); yt+=25; }
      img.draw_image((Wi-text3.width())/2,yt,text3,text3.get_dilate(5),1,255).
        draw_image((Wi-text4.width())/2,yt+25,text4,text4.get_dilate(5),1,255).
        draw_image((Wi-text5.width())/2,yt+50,text5,text5.get_dilate(5),1,255).
        draw_image((Wi-text6.width())/2,yt+75,text6,text6.get_dilate(5),1,255).display(disp);
      for (disp.flush(); !disp.is_closed() && !disp.key() && !disp.button(); disp.wait())
        if (disp.is_resized()) disp.resize(disp);
      disp.flush();
      board.assign();
      for (float i = 10; i>0 && !disp.is_keyESC(); i-=0.25)
        disp.display(img.get_crop((int)(Wi*(0.5f-i*i*i*i/20000.0f)),(int)(Hi*(0.5f-i*i/200.0f)),
                                  (int)(Wi*(0.5f+i*i*i*i/20000.0f)),(int)(Hi*(0.5f+i*i/200.0f)))).wait(20);
    }
  }

  // Save score history if possible, and exit.
  if (score_history) {
    file = std::fopen(filename_history,"w");
    if (file) { std::fclose(file); (score_history>'y').save_dlm(filename_history); }
  }

  return 0;
}
