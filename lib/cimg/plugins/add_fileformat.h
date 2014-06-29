/*
 #
 #  File        : add_fileformat.h
 #                ( C++ header file - CImg plug-in )
 #
 #  Description : CImg plug-in that adds loading/saving support for a personalized
 #                file format (determined by its extension, here ".foo").
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

#ifndef cimg_plugin_addfileformat
#define cimg_plugin_addfileformat

// These functions load ".foo" filenames
//---------------------------------------
static CImg<T> get_load_foo(const char *filename) {
  std::fprintf(stderr,"Load '%s' here..\n",filename);
  return CImg<T>(512,512,1,3,0).noise(30);
}

CImg& load_foo(const char *filename) {
  return get_load_foo(filename).swap(*this);
}

// This function saves the instance image into a ".foo" file.
//-----------------------------------------------------------
const CImg& save_foo(const char *filename) const {
  std::fprintf(stderr,"Save '%s' here..\n",filename);
  return *this;
}

// The code below allows to add the support for the specified extension.
//---------------------------------------------------------------------
#ifndef cimg_load_plugin
#define cimg_load_plugin(filename) \
  if (!cimg::strncasecmp(cimg::split_filename(filename),"foo",3)) return load_foo(filename);
#endif
#ifndef cimg_save_plugin
#define cimg_save_plugin(filename) \
  if (!cimg::strncasecmp(cimg::split_filename(filename),"foo",3)) return save_foo(filename);
#endif

// End of the plugin.
//-------------------
#endif
