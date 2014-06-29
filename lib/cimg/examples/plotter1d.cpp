/*
 #
 #  File        : plotter1d.cpp
 #                ( C++ source file )
 #
 #  Description : A simple math formula plotter.
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

// Include CImg library file and use its main namespace
#include "CImg.h"
using namespace cimg_library;

// Main procedure
//----------------
int main(int argc,char **argv) {

  // Read command line argument.
  cimg_usage("Simple plotter of mathematical formulas");
  const char *const formula = cimg_option("-f","sin(x/8) % cos(2*x)","Formula to plot");
  const float x0 = cimg_option("-x0",-5.0f,"Minimal X-value");
  const float x1 = cimg_option("-x1",5.0f,"Maximal X-value");
  const int resolution = cimg_option("-r",1024,"Plot resolution");
  const unsigned int nresolution = resolution>1?resolution:1024;
  const unsigned int plot_type = cimg_option("-p",1,"Plot type");
  const unsigned int vertex_type = cimg_option("-v",1,"Vertex type");

  // Create plot data.
  CImg<double> values(4,nresolution,1,1,0);
  const unsigned int r = nresolution - 1;
  cimg_forY(values,X) values(0,X) = x0 + X*(x1-x0)/r;
  cimg::eval(formula,values).move_to(values);

  // Display interactive plot window.
  values.display_graph(formula,plot_type,vertex_type,"X-axis",x0,x1,"Y-axis");

  // Quit.
  return 0;
}
