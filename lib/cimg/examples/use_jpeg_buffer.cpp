/*
 #
 #  File        : use_jpeg_buffer.cpp
 #                ( C++ source file )
 #
 #  Description : Example of use for the CImg plugin 'plugins/jpeg_buffer.h'.
 #                This file is a part of the CImg Library project.
 #                ( http://cimg.sourceforge.net )
 #
 #  Copyright   : Paolo Prete
 #                ( p4olo_prete(at)yahoo.it )
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

// These includes are necessary to get the plug-in compile !
#include <cstdio>
#include <jpeglib.h>
#include <jerror.h>

// Define plugin and include the CImg Library.
#define cimg_plugin "plugins/jpeg_buffer.h"
#include "CImg.h"
using namespace cimg_library;

// Main procedure
//----------------
int main() {

  // Create a jpeg memory buffer from the content of a jpeg file.
  // (this is for testing purposes only)
  const char *filename_input = "foo.jpg";
  std::fprintf(stderr," - Reading file '%s'\n",filename_input);
  std::FILE *file_input = std::fopen(filename_input,"rb");
  if (!file_input) { std::fprintf(stderr,"Input JPEG file not found !"); std::exit(0); }

  std::fprintf(stderr," - Construct input JPEG-coded buffer\n");
  unsigned buf_size = 500000; // Put the file size here !
  JOCTET *buffer_input = new JOCTET[buf_size];
  if (std::fread(buffer_input,sizeof(JOCTET),buf_size,file_input)) std::fclose(file_input);
  // -> 'buffer_input' is now a valid jpeg-coded memory buffer.

  // Create a CImg instance from the jpeg-coded buffer using the plug-in function.
  std::fprintf(stderr," - Create CImg instance from JPEG-coded buffer\n");
  CImg<unsigned char> img;
  img.load_jpeg_buffer(buffer_input, buf_size);
  delete[] buffer_input;

  // Do you image processing stuff here ....
  // Here, we just mirror the image and write "hello".
  std::fprintf(stderr," - Do simple processing\n");
  const unsigned char purple[] = { 255, 0, 0 };
  const unsigned char black[] = { 0, 0, 0 };
  img.mirror('y').draw_text(0,0,"   Hello!   ",purple,black,1,57);

  // Display image to see if everything's fine.
  img.display("Using 'jpeg_buffer.h' plugin");

  // Define a new JOCTET array where the processed image has to be saved
  // (we don't know its dimension before compressing it, therefore we have to allocate enough memory )
  std::fprintf(stderr," - Construct output JPEG-coded buffer\n");
  JOCTET *buffer_output = new JOCTET[2*buf_size];

  // Save processed image into this JOCTET buffer, compressed as jpeg.
  // This is done again by using the plug-in function.
  img.save_jpeg_buffer(buffer_output,buf_size,60);
  // Note that here, the variable 'buf_size' contains the length of the
  // data which have been written in the given output buffer.

  // Copy the content of the above array into a new file
  // (it should give you a valid JPEG file then !)
  const char *filename_output = "foo_output.jpg";
  std::fprintf(stderr," - Save output file '%s'\n",filename_output);
  std::FILE* file_output = std::fopen(filename_output,"wb");
  std::fwrite(buffer_output, sizeof(JOCTET), buf_size, file_output);
  std::fclose(file_output);
  delete[] buffer_output;

  std::fprintf(stderr," - All done !\n");
  return 0;
}
