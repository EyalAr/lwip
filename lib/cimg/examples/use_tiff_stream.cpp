/*
 #
 #  File        : use_tiff_stream.cpp
 #                ( C++ source file )
 #
 #  Description : Example of use for the CImg plugin 'plugins/jpeg_buffer.h'.
 #                This file is a part of the CImg Library project.
 #                ( http://cimg.sourceforge.net )
 #
 #  Copyright   : Wolf Blecher
 #                ( Wolf.Blecher(at)sirona.com )
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


#include <fstream>
// These includes are necessary to get the plug-in compile ! Don't forget to link with 'libtiff' and 'libtiffxx' !
#include "tiffio.h"
#include "tiffio.hxx"

// Define plugin and include the CImg Library.
#define cimg_plugin "plugins/tiff_stream.h"
#include "CImg.h"
using namespace cimg_library;

// Main procedure
//----------------
int main() {

  std::ifstream inFile("input.tif", std::ifstream::in | std::ifstream::binary);
  std::ofstream outFile("outFile.tif", std::ofstream::out | std::ifstream::binary);

  if (!inFile.good())
  {
    std::cout << "Error Reading from infile" << std::endl;
  }

  cimg_library::CImg<unsigned short> imgIn;
  imgIn.load_tiff(&inFile);
  imgIn.display();
  CImg<unsigned short> imgOut = imgIn.save_tiff(&outFile, 2U);
  imgOut.display();

  inFile.close();
  outFile.close();

  inFile.open("outFile.tif", std::ifstream::in | std::ifstream::binary);
  imgIn.load_tiff(&inFile);
  imgIn.display();
  inFile.close();
  return 0;
}
