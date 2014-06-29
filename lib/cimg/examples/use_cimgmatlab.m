/*-----------------------------------------------------------------------
  File : use_cimgmatlab.m

  Description: Example of use for the CImg plugin 'plugins/cimgmatlab.h'
  which allows to use CImg in order to develop matlab external
  functions (mex functions).
  User should be familiar with Matlab C/C++ mex function concepts,
  as this file is by no way a mex programming tutorial.

  This simple example implements a mex function that can be called
  as

  - v = cimgmatlab_cannyderiche(u,s)
  - v = cimgmatlab_cannyderiche(u,sx,sy)
  - v = cimgmatlab_cannyderiche(u,sx,sy,sz)

  The corresponding m-file is cimgmatlab_cannyderiche.m


  Copyright : Francois Lauze - http://www.itu.dk/people/francois
  This software is governed by the Gnu General Public License
  see http://www.gnu.org/copyleft/gpl.html

  The plugin home page is at
  http://www.itu.dk/people/francois/cimgmatlab.html

  for the compilation: using the mex utility provided with matlab, just
  remember to add the -I flags with paths to CImg.h and/or cimgmatlab.h.
  The default lcc cannot be used, it is a C compiler and not a C++ one!
--------------------------------------------------------------------------*/

function v = cimgmatlab_cannyderiche(u,sx,sy,sz)

