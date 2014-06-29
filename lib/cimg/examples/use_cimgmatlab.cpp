/*-----------------------------------------------------------------------

  File : use_cimgmatlab.cpp

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
  This software is governed by the Gnu Lesser General Public License
  see http://www.gnu.org/copyleft/lgpl.html

  The plugin home page is at
  http://www.itu.dk/people/francois/cimgmatlab.html

  for the compilation: using the mex utility provided with matlab, just
  remember to add the -I flags with paths to CImg.h and/or cimgmatlab.h.
  The default lcc cannot be used, it is a C compiler and not a C++ one!

--------------------------------------------------------------------------*/

#include <mex.h>
#define cimg_plugin "plugins/cimgmatlab.h"
#include <CImg.h>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
  if (nrhs < 2) mexErrMsgTxt("No enough input arguments.");
  if (nrhs > 4) mexErrMsgTxt("Too many input arguments.");
  cimg_library::CImg<> u(prhs[0],true);
  if (nrhs == 2) {
    const float s = (float)mxGetScalar(prhs[1]);
    plhs[0] = u.get_blur(s).toMatlab();
  } else if (nrhs == 3) {
    const float sx = (float)mxGetScalar(prhs[1]);
    const float sy = (float)mxGetScalar(prhs[2]);
    plhs[0] = u.get_blur(sx,sy,0).toMatlab();
  } else if (nrhs == 4) {
    const float sx = (float)mxGetScalar(prhs[1]);
    const float sy = (float)mxGetScalar(prhs[2]);
    const float sz = (float)mxGetScalar(prhs[3]);
    plhs[0] = u.get_blur(sx,sy,sz).toMatlab();
  }
}

/*------------------------------------------------------------------

  SPECIAL NOTE :
  -------------

  How to read a .mat file using plugin 'cimgmatlab.h' ?
  (contribution by Vo Duc Khanh/Denso IT Lab, Tokyo, Japan).

  #include <mex.h>
  #include <mat.h>
  #include <matrix.h>

  #define cimg_plugin "cimgmatlab.h"

  #include "CImg.h"
  #include <iostream>
  #include <string>

  .........

  using namespace cimg_library;
  using namespace std;

  // Load input images (125700 images) from training database 'BmpTrainingDb.mat'
  MATFile *pmat, *pmat_out;
  mxArray *pa, *pa_out;
  const char data_path[256] = ".\\BmpTrainingDb.mat\0";
  const char *var_name;

  pmat = matOpen(data_path, "r");
  if (pmat == NULL) {
    cout << "Error opening file " << data_path << endl;
    return (1);
  }

  pa = matGetNextVariable(pmat, &var_name);
  if (pa == NULL){
    cout << "Error reading in file " << data_path << endl;
    return (1);
  }

  CImg<unsigned char> train_db(pa,false);
  ........


  -----------------------------------------------------------------------------*/
