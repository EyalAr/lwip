/*
 #
 #  File        : jpeg_buffer.h
 #                ( C++ header file - CImg plug-in )
 #
 #  Description : This CImg plug-in provide functions to load and save jpeg images
 #                directly from/to memory buffers of JOCTET buffers, using the
 #                JPEG library (required to compile !)
 #                This file is a part of the CImg Library project.
 #                ( http://cimg.sourceforge.net )
 #
 #  Copyright   : Paolo Prete
 #                ( p4olo_prete@yahoo.it )
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

/*-----------------------------------------------------------------------------------

 IMPORTANT NOTE :

 You *need* to include the following lines in your own code to use this plugin :

 #include <cstdio>
 #include <jpeglib.h>
 #include <jerror.h>

 (see example file provided in examples/use_jpeg_buffer.cpp).

------------------------------------------------------------------------------------*/

#ifndef cimg_plugin_jpeg_buffer
#define cimg_plugin_jpeg_buffer

///////////////////////////////////////////////////////////////////////////////////////
//
//    extension of libjpeg (helper functions for loading images from JOCTET arrays)
//                                      hacked from
//      http://www.koders.com/cpp/fidB5A4549ABB5CB01824058F57A43D095D3F95AB40.aspx
//
///////////////////////////////////////////////////////////////////////////////////////

#define INPUT_BUF_SIZE 4096

struct my_source_mem {
  struct jpeg_source_mgr pub; // Public fields
  int indexinmem;
  JOCTET *inmem;              // Source stream
  JOCTET *buffer;             // Start of buffer
  int lenght;                 // Size of buffer in memory
  boolean start_of_file;      // Have we gotten any data yet?
};

struct my_source_mgr {
  struct jpeg_source_mgr pub; // public fields
  FILE *infile;               // source stream
  JOCTET *buffer;             // start of buffer
  boolean start_of_file;      // have we gotten any data yet?
};

typedef my_source_mem *my_src_mptr;
typedef my_source_mgr *my_src_ptr;

static boolean fill_minput_buffer(j_decompress_ptr cinfo) {
  my_src_mptr src = (my_src_mptr) cinfo->src;
  size_t nbytes;
  if (src->indexinmem+INPUT_BUF_SIZE>src->lenght) nbytes=src->lenght-src->indexinmem;
  else nbytes = INPUT_BUF_SIZE;
  std::memcpy(src->buffer,src->inmem,nbytes);
  src->inmem += nbytes;
  src->indexinmem += (int)nbytes;
  src->pub.next_input_byte = src->buffer;
  src->pub.bytes_in_buffer = INPUT_BUF_SIZE;
  src->start_of_file = FALSE;
  return TRUE;
}

static void skip_minput_data(j_decompress_ptr cinfo, long num_bytes) {
  my_src_ptr src = (my_src_ptr)cinfo->src;
  if (num_bytes > 0) {
    while (num_bytes > (long) src->pub.bytes_in_buffer) {
      num_bytes -= (long) src->pub.bytes_in_buffer;
      fill_minput_buffer(cinfo);
      // note we assume that fill_input_buffer will never return FALSE,
      // so suspension need not be handled.
      //
    }
    src->pub.next_input_byte += (size_t) num_bytes;
    src->pub.bytes_in_buffer -= (size_t) num_bytes;
  }
}

static void init_msource(j_decompress_ptr cinfo) {
  my_src_mptr src = (my_src_mptr)cinfo->src;
  src->start_of_file = TRUE;
}

static void term_source(j_decompress_ptr) {
  // no work necessary here
}

static void jpeg_mem_src(j_decompress_ptr cinfo, JOCTET * memptr,int lenght) {
  my_src_mptr src;

  // The source object and input buffer are made permanent so that a series
  //of JPEG images can be read from the same file by calling jpeg_stdio_src
  // only before the first one.  (If we discarded the buffer at the end of
  // one image, we'd likely lose the start of the next one.)
  // This makes it unsafe to use this manager and a different source
  // manager serially with the same JPEG object.  Caveat programmer.
  //

  // first time for this JPEG object?
  if (cinfo->src == NULL) {
    cinfo->src = (struct jpeg_source_mgr*)(*cinfo->mem->alloc_small)((j_common_ptr) cinfo, JPOOL_PERMANENT,sizeof(my_source_mem));
    src = (my_src_mptr) cinfo->src;
    src->buffer = (JOCTET *)(*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,INPUT_BUF_SIZE * sizeof(JOCTET));
  }

  src = (my_src_mptr) cinfo->src;
  src->pub.init_source = init_msource;
  src->pub.fill_input_buffer = fill_minput_buffer;
  src->pub.skip_input_data = skip_minput_data;
  //src->pub.resync_to_restart = jpeg_resync_to_restart; // use default method
  src->pub.term_source = term_source;
  src->inmem = memptr;
  src->indexinmem = 0;
  src->lenght = lenght;
  src->pub.bytes_in_buffer = 0; // forces fill_input_buffer on first read
  src->pub.next_input_byte = NULL; // until buffer loaded
}

// The following declarations and 5 functions are jpeg related
// functions used by put_jpeg_grey_memory and put_jpeg_yuv420p_memory
//
struct mem_destination_mgr {
  struct jpeg_destination_mgr pub;
  JOCTET *buf;
  size_t bufsize;
  size_t jpegsize;
};

typedef mem_destination_mgr *mem_dest_ptr;

static void init_destination(j_compress_ptr cinfo) {
  mem_dest_ptr dest = (mem_dest_ptr) cinfo->dest;
  dest->pub.next_output_byte = dest->buf;
  dest->pub.free_in_buffer = dest->bufsize;
  dest->jpegsize = 0;
}

static boolean empty_output_buffer(j_compress_ptr cinfo) {
  mem_dest_ptr dest = (mem_dest_ptr) cinfo->dest;
  dest->pub.next_output_byte = dest->buf;
  dest->pub.free_in_buffer = dest->bufsize;
  return FALSE;
  ERREXIT(cinfo, JERR_BUFFER_SIZE);
}

static void term_destination(j_compress_ptr cinfo) {
  mem_dest_ptr dest = (mem_dest_ptr) cinfo->dest;
  dest->jpegsize = dest->bufsize - dest->pub.free_in_buffer;
}

static void jpeg_mem_dest(j_compress_ptr cinfo, JOCTET* buf, size_t bufsize) {
  mem_dest_ptr dest;
  if (cinfo->dest == NULL) {
    cinfo->dest = (struct jpeg_destination_mgr *)
      (*cinfo->mem->alloc_small)((j_common_ptr)cinfo,JPOOL_PERMANENT,sizeof(mem_destination_mgr));
  }
  dest = (mem_dest_ptr) cinfo->dest;
  dest->pub.init_destination = init_destination;
  dest->pub.empty_output_buffer = empty_output_buffer;
  dest->pub.term_destination = term_destination;
  dest->buf = buf;
  dest->bufsize = bufsize;
  dest->jpegsize = 0;
}

static unsigned jpeg_mem_size(j_compress_ptr cinfo) {
  mem_dest_ptr dest = (mem_dest_ptr) cinfo->dest;
  return dest->jpegsize;
}

/////////////////////////////////////////////////////////////////
//
//    Define main CImg plugin functions.
//    (you should use these functions only in your own code)
//
/////////////////////////////////////////////////////////////////

//! Load image from a jpeg-coded memory buffer.
/**
   \param buffer Memory buffer containing the jpeg-coded image data.
   \param buffer_size Size of the memory buffer, in bytes.
**/
static CImg get_load_jpeg_buffer(const JOCTET *const buffer, const unsigned buffer_size) {
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);
  jpeg_mem_src(&cinfo,const_cast<JOCTET*>(buffer),buffer_size);
  jpeg_read_header(&cinfo,TRUE);
  jpeg_start_decompress(&cinfo);

  const unsigned int row_stride = cinfo.output_width * cinfo.output_components;
  JOCTET *buf = new JOCTET[cinfo.output_width*cinfo.output_height*cinfo.output_components];
  const JOCTET *buf2 = buf;
  JSAMPROW row_pointer[1];
  while (cinfo.output_scanline < cinfo.output_height) {
    row_pointer[0] = buf + cinfo.output_scanline*row_stride;
    jpeg_read_scanlines(&cinfo,row_pointer,1);
  }
  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);

  CImg<T> dest(cinfo.output_width,cinfo.output_height,1,cinfo.output_components);
  switch (dest.spectrum()) {
  case 1: {
    T *ptr_g = dest.data(0,0,0,0);
    cimg_foroff(dest,off) *(ptr_g++) = (T)*(buf2++);
  } break;
  case 3: {
    T
      *ptr_r = dest.data(0,0,0,0),
      *ptr_g = dest.data(0,0,0,1),
      *ptr_b = dest.data(0,0,0,2);
    cimg_forXY(dest,x,y) {
      *(ptr_r++) = (T)*(buf2++);
      *(ptr_g++) = (T)*(buf2++);
      *(ptr_b++) = (T)*(buf2++);
    }
  } break;
  case 4: {
    T
      *ptr_r = dest.data(0,0,0,0),
      *ptr_g = dest.data(0,0,0,1),
      *ptr_b = dest.data(0,0,0,2),
      *ptr_a = dest.data(0,0,0,3);
    cimg_forXY(dest,x,y) {
      *(ptr_r++) = (T)*(buf2++);
      *(ptr_g++) = (T)*(buf2++);
      *(ptr_b++) = (T)*(buf2++);
      *(ptr_a++) = (T)*(buf2++);
    }
  } break;
  }
  delete[] buf;

  return dest;
}

//! Load image from a jpeg-coded memory buffer (in-place version)
/**
   \param buffer Memory buffer containing the jpeg-coded image data.
   \param buffer_size Size of the memory buffer, in bytes.
**/
CImg& load_jpeg_buffer(const JOCTET *const buffer, const unsigned buffer_size) {
  return get_load_jpeg_buffer(buffer,buffer_size).move_to(*this);
}

//! Save image in a memory buffer, directly as a jpeg-coded file
/**
   \param buffer Memory buffer that will be written with the jpeg-coded image data.
   \param buffer_size Initial size of the memory buffer. When the function returns, the variable
   contains the effective length needed to fill the buffer.
   \param quality Quality of the jpeg compression.
**/
const CImg& save_jpeg_buffer(JOCTET *const buffer, unsigned int &buffer_size, const int quality=100) const {

  // Fill pixel buffer
  JOCTET *buf;
  unsigned int dimbuf=0;
  J_COLOR_SPACE colortype=JCS_RGB;
  switch (spectrum()) {
  case 1: {
    // Greyscale images
    JOCTET *buf2 = buf = new JOCTET[width()*height()*(dimbuf=1)];
    const T
      *ptr_g = data();
    colortype = JCS_GRAYSCALE;
    cimg_foroff(*this,off) *(buf2++) = (JOCTET)*(ptr_g++);
  } break;
  case 2:
  case 3: {
    // RGB images
    JOCTET *buf2 = buf = new JOCTET[width()*height()*(dimbuf=3)];
    const T
      *ptr_r = data(0,0,0,0),
      *ptr_g = data(0,0,0,1),
      *ptr_b = data(0,0,0,spectrum()>2?2:0);
    colortype = JCS_RGB;
    cimg_forXY(*this,x,y) {
      *(buf2++) = (JOCTET)*(ptr_r++);
      *(buf2++) = (JOCTET)*(ptr_g++);
      *(buf2++) = (JOCTET)*(ptr_b++);
    }
  } break;
  default: {
    // YCMYK images
    JOCTET *buf2 = buf = new JOCTET[width()*height()*(dimbuf=4)];
    const T
      *ptr_r = data(0,0,0,0),
      *ptr_g = data(0,0,0,1),
      *ptr_b = data(0,0,0,2),
      *ptr_a = data(0,0,0,3);
    colortype = JCS_CMYK;
    cimg_forXY(*this,x,y) {
      *(buf2++) = (JOCTET)*(ptr_r++);
      *(buf2++) = (JOCTET)*(ptr_g++);
      *(buf2++) = (JOCTET)*(ptr_b++);
      *(buf2++) = (JOCTET)*(ptr_a++);
    }
  } break;
  }

  // Call libjpeg functions
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&cinfo);
  jpeg_mem_dest(&cinfo, buffer, buffer_size);
  cinfo.image_width = width();
  cinfo.image_height = height();
  cinfo.input_components = dimbuf;
  cinfo.in_color_space = colortype;
  jpeg_set_defaults(&cinfo);
  jpeg_set_quality(&cinfo,quality<100?quality:100,TRUE);
  jpeg_start_compress(&cinfo,TRUE);

  const unsigned int row_stride = width()*dimbuf;
  JSAMPROW row_pointer[1];
  while (cinfo.next_scanline < cinfo.image_height) {
    row_pointer[0] = &buf[cinfo.next_scanline*row_stride];
    jpeg_write_scanlines(&cinfo,row_pointer,1);
  }
  jpeg_finish_compress(&cinfo);
  delete[] buf;
  buffer_size = jpeg_mem_size(&cinfo);
  jpeg_destroy_compress(&cinfo);
  return *this;
}

// End of the plug-in
//-------------------
#endif
