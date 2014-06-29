/*
#
#  File        : tiff_stream.h
#                ( C++ header file - CImg plug-in )
#
#  Description : This CImg plug-in provide functions to load and save tiff images
#                from std::istream/ to std::ostream
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

/*-----------------------------------------------------------------------------------

IMPORTANT NOTE :

You *need* to include the following lines in your own code to use this plugin:

#include "tiffio.h"
#include "tiffio.hxx"

You *need* to link your code with the libtiff and libtiffxx libraries as well.

------------------------------------------------------------------------------------*/

#ifndef cimg_use_tiff
error cimg_use_tiff not defined
#endif

#ifndef cimg_plugin_tiff_stream
#define cimg_plugin_tiff_stream

#include <ios>

/////////////////////////////////////////////////////////////////
//
//    Define main CImg plugin functions.
//    (you should use these functions only in your own code)
//
/////////////////////////////////////////////////////////////////

//! Save image as a TIFF file.
/**
   \param tiffOutStream std::ostream, where to write the image to
   \param compression_type Type of data compression. Can be <tt>{ 1=None | 2=CCITTRLE | 3=CCITTFAX3 | 4=CCITTFAX4 | 5=LZW | 6=JPEG }</tt>.
   \note
   - libtiff support is enabled by defining the precompilation
   directive \c cimg_use_tiff.
   - When libtiff is enabled, 2D and 3D (multipage) several
   channel per pixel are supported for
   <tt>char,uchar,short,ushort,float</tt> and \c double pixel types.
**/
const CImg<T>& save_tiff(std::ostream *tiffOutStream, const unsigned int compression_type=0) const {
  if (!tiffOutStream->good())
    {
      throw CImgArgumentException(_cimg_instance
                                  "save_tiff(): tiffstream is not good!",
                                  cimg_instance);
    }

  if (is_empty())
    {
      throw CImgArgumentException(_cimg_instance
                                  "Not allowed to write empty images to stream",
                                  cimg_instance
                                  );
    }

  TIFF *tif = TIFFStreamOpen("MemTiff", tiffOutStream);
  if (tif)
    {
      cimg_forZ(*this,z) get_slice(z)._save_tiff(tif,z,compression_type);
      tiffOutStream->flush();
      TIFFClose(tif);
    }
  else
    {
      throw CImgIOException(_cimg_instance
                            "save_tiff(): Failed to stream for writing.",
                            cimg_instance);
    }

  return *this;
}

//! Load images from a TIFF file.
/**
   \param tiffInStream std::istream to read data from.
   \param first_frame Index of first image frame to read.
   \param last_frame Index of last image frame to read.
   \param step_frame Step applied between each frame.
**/
CImg<T>& load_tiff(std::istream* tiffInStream,
                   const unsigned int first_frame=0, const unsigned int last_frame=~0U,
                   const unsigned int step_frame=1)
{
  const unsigned int
    nfirst_frame = first_frame<last_frame?first_frame:last_frame,
    nstep_frame = step_frame?step_frame:1;
  unsigned int nlast_frame = first_frame<last_frame?last_frame:first_frame;

  TIFF *tif = TIFFStreamOpen("MemTiff", tiffInStream);
  if (tif)
    {
      unsigned int nb_images = 0;
      do {
        ++nb_images;
      } while (TIFFReadDirectory(tif));

      if (nfirst_frame>=nb_images || (nlast_frame!=~0U && nlast_frame>=nb_images))
        {
          cimg::warn("load_tiff(): Invalid specified frame range is [%u,%u] (step %u) since stream contains %u image(s).",
                     nfirst_frame,nlast_frame,nstep_frame,nb_images);
        }

      if (nfirst_frame>=nb_images)
        {
          return assign();
        }

      if (nlast_frame>=nb_images)
        {
          nlast_frame = nb_images-1;
        }
      TIFFSetDirectory(tif,0);
      CImg<T> frame;
      for (unsigned int l = nfirst_frame; l<=nlast_frame; l+=nstep_frame) {
        frame._load_tiff(tif,l);
        if (l==nfirst_frame) assign(frame._width,frame._height,1+(nlast_frame-nfirst_frame)/nstep_frame,frame._spectrum);
        if (frame._width>_width || frame._height>_height || frame._spectrum>_spectrum)
          resize(cimg::max(frame._width,_width),cimg::max(frame._height,_height),-100,cimg::max(frame._spectrum,_spectrum),0);
        draw_image(0,0,(l-nfirst_frame)/nstep_frame,frame);
      }
      TIFFClose(tif);
    }
  else
    {
      throw CImgIOException(_cimg_instance
                            "load_tiff(): Failed to read data from stream",
                            cimg_instance);
    }

  return *this;
}

//! Load a multi-page TIFF file \newinstance.
static CImg<T> get_load_tiff(std::istream* tiffInStream,
                             const unsigned int first_frame=0, const unsigned int last_frame=~0U,
                             const unsigned int step_frame=1)
{
  return CImg<T>().load_tiff(tiffInStream,first_frame,last_frame,step_frame);
}

// End of the plug-in
//-------------------
#endif
