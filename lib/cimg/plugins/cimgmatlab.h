/*************************************************************************
 * cimgmatlab.h
 * -------------
 *
 * cimgmatlab.h  is a "plugin" for the CImg library that allows to convert
 * CImg<T> images from/to MATLAB arrays, so that CImg can be used to write
 * MATLAB mex files.  It also swaps the "x" and "y" coordinates when going
 * from / to MATLAB array, i.e. the usual image-processing annoying MATLAB
 * behaviour of considering images as matrices.
 *
 * Added to the CImg<T> class are:
 *
 *  - a constructor : CImg(const mxArray *matlabArray, bool vdata = false)
 *    the vdata  serves  to  decide  whether a 3D matlab array should give
 *    rise to a 3D CImg object or a "2D vectorial" one.
 *
 *  - a assignment operator : CImg & operator=(const mxArray *matlabArray)
 *    (I use myself extremely seldom and might remove it in the future).
 *
 *  - a routine converting a CImg image to a matlab array:
 *    mxArray *toMatlab(mxClassID classID = mxDOUBLE_CLASS,
 *                      bool squeeze = false) const
 *    the squeeze argument serves the opposite purpose than the vdata from
 *    the constructor.
 *
 * For a  bit  more documentation, the manual is this header, see the more
 * detailed comments in the source code (i.e. RTFM)
 *
 *
 * Its usage should be straightforward:
 *
 * - file cimgmatlab.h must be in a directory that the compiler can locate.
 * - prior to include CImg.h, mex.h  must  be  included first, else it will
 *   result in a compiler error.
 * - after the inclusion of mex.h, one must define the macro cimg_plugin as
 *   "cimgmatlab.h"  or  <cimgmatlab.h> or  <CImg/plugins/cimgmatlab.h>  or
 *   a variation that  matches your  local installation of CImg package and
 *   plugins probably via the appropriate specification of the include path
 *   "-Ipath/to/cimg/and/plugins" at mex cmdline.
 *
 * You would probably have this kind of declaration:
 *
 * // The begining of my fantastic mex file code...
 * #include <mex.h>
 * ...
 * #define cimg_plugin  <cimgmatlab.h>
 * #include <CImg.h>
 * ...
 * // and now I can implement my new killer MATLAB function!
 * ....
 *
 *
 * Copyright (c) 2004-2008 Francois Lauze
 * Licence: the Gnu Lesser General Public License
 * http://www.gnu.org/licenses/lgpl.html
 *
 * MATLAB is copyright of The MathWorks, Inc, http://www.mathworks.com
 *
 * Any comments, improvements and potential bug corrections are welcome, so
 * write to  me at francois@diku.dk, or use CImg forums, I promise I'll try
 * to read them once in a while. BTW who modified the cpMatlabData with the
 * cimg::type<t>::is_float() test (good idea!)
 *
 ***************************************************************************/

#ifndef cimg_plugin_matlab
#define cimg_plugin_matlab

#define CIMGMATLAB_VER 0102
#ifndef mex_h
#error the file mex.h must be included prior to inclusion of cimgmatlab.h
#endif
#ifndef cimg_version
#error cimgmatlab.h requires that CImg.h is included!
#endif

/**********************************************************
 * introduction of mwSize and mwIndex types in relatively *
 * recent versions of matlab, 7.3.0 from what I gathered. *
 * here is hopefully a needed fix for older versions      *
 **********************************************************/
#if !defined(MX_API_VER) ||  MX_API_VER < 0x7030000
typedef int mwSize;
#endif

/*********************************************************
 * begin of included methods                             *
 * They are just added as member functions / constructor *
 * for the CImg<T> class.                                *
 *********************************************************/

private:

/**********************************************************************
 * internally used to transfer MATLAB array values to CImg<> objects,
 * check wether the array type is a "numerical" one (including logical)
 */
static int isNumericalClassID(mxClassID id) {
  // all these constants are defined in matrix.h included by mex.h
  switch (id) {
  case mxLOGICAL_CLASS:
  case mxDOUBLE_CLASS:
  case mxSINGLE_CLASS:
  case mxINT8_CLASS:
  case mxUINT8_CLASS:
  case mxINT16_CLASS:
  case mxUINT16_CLASS:
  case mxINT32_CLASS:
  case mxUINT32_CLASS:
  case mxINT64_CLASS:
  case mxUINT64_CLASS: return 1;
  default: return 0;
  }
}

/***************************************************
 * driving routine that will copy the content of
 * a MATLAB array to this->_data
 * The type names used are defined in matlab c/c++
 * header file tmwtypes.h
 */
void makeImageFromMatlabData(const mxArray *matlabArray, mxClassID classID) {
  if (classID==mxLOGICAL_CLASS) {
    // logical type works a bit differently than the numerical types
    mxLogical *mdata = mxGetLogicals(matlabArray);
    cpMatlabData((const mxLogical *)mdata);
  } else {
    void *mdata = (void*)mxGetPr(matlabArray);
    switch (classID) {
    case mxDOUBLE_CLASS : cpMatlabData((const real64_T*)mdata); break;
    case mxSINGLE_CLASS : cpMatlabData((const real32_T*)mdata); break;
    case mxINT8_CLASS : cpMatlabData((const int8_T*)mdata); break;
    case mxUINT8_CLASS : cpMatlabData((const uint8_T*)mdata); break;
    case mxINT16_CLASS : cpMatlabData((const int16_T*)mdata); break;
    case mxUINT16_CLASS : cpMatlabData((const uint16_T*)mdata); break;
    case mxINT32_CLASS : cpMatlabData((const int32_T*)mdata); break;
    case mxUINT32_CLASS : cpMatlabData((const uint32_T*)mdata); break;
    case mxINT64_CLASS : cpMatlabData((const int64_T*)mdata); break;
    case mxUINT64_CLASS : cpMatlabData((const uint64_T*)mdata); break;
    }
  }
}

/***********************************************************
 * the actual memory copy and base type conversion is then
 * performed by this routine that handles the annoying x-y
 * problem of MATLAB when dealing with images: we switch
 * line and column storage: the MATLAB A(x,y) becomes the
 * CImg img(y,x)
 */
template <typename t> void cpMatlabData(const t* mdata) {
  if (cimg::type<t>::is_float()) {
    cimg_forXYZC(*this,x,y,z,v) (*this)(x,y,z,v) = (T)(mdata[((v*_depth + z)*_width + x)*_height + y]);
  } else {
    cimg_forXYZC(*this,x,y,z,v) (*this)(x,y,z,v) = (T)(int)(mdata[((v*_depth + z)*_width + x)*_height + y]);
  }
}

public:

/******************************************************************
 * Consruct a CImg<T> object from a MATLAB mxArray.
 * The MATLAB array must be AT MOST 4-dimensional. The boolean
 * argument vdata is employed in the case the the input mxArray
 * has dimension 3, say M x N x K. In that case, if vdata is true,
 * the last dimension is assumed to be "vectorial" and the
 * resulting CImg<T> object has dimension N x M x 1 x K. Otherwise,
 * the resulting object has dimension N x M x K x 1.
 * When MATLAB array has dimension 2 or 4, vdata has no effects.
 * No shared memory mechanisms are used, it would be the easiest
 * to crash Matlab (from my own experience...)
 */
CImg(const mxArray *matlabArray, const bool vdata = false)
  : _is_shared(false) {
  mwSize nbdims = mxGetNumberOfDimensions(matlabArray);
  mxClassID classID = mxGetClassID(matlabArray);
  if (nbdims>4 || !isNumericalClassID(classID)) {
    _data = 0; _width = _height = _depth = _spectrum = 0;
#if cimg_debug>1
    cimg::warn("MATLAB array is more than 4D or/and not numerical, returning an empty image.");
#endif
  } else {
    const mwSize *dims = mxGetDimensions(matlabArray);
    _depth = _spectrum = 1;
    _width = (unsigned)dims[1];
    _height = (unsigned)dims[0];
    if (nbdims==4) { _depth = (unsigned)dims[2]; _spectrum = (unsigned)dims[3]; }
    else if (nbdims==3) {
      if (vdata) _spectrum = (unsigned)dims[2]; else _depth = (unsigned)dims[2];
    }
    _data = new T[size()];
    makeImageFromMatlabData(matlabArray,classID);
  }
}

/*******************************************************************
 * operator=(). Copy  mxMarray data mArray into the current image
 * Works as the previous constructor, but without the vdata stuff.
 * don't know if it is of any use...
 */
CImg & operator=(const mxArray *matlabArray) {
  int
    nbdims = (int)mxGetNumberOfDimensions(matlabArray),
    classID = mxGetClassID(matlabArray);
  if (nbdims>4 || !isNumericalClassID(classID)) {
    delete [] _data; _data = 0;
    _width = _height = _depth = _spectrum = 0;
#if cimg_debug>1
    cimg::warn("MATLAB array is more than 4D or/and not numerical, returning an empty image.");
#endif
  } else {
    const mwSize *dims = mxGetDimensions(matlabArray);
    _depth = _spectrum = 1;
    _width =  (unsigned)dims[1];
    _height = (unsigned)dims[0];
    if (nbdims>2) _depth = (unsigned)dims[2];
    else if (nbdims>3) _spectrum = (unsigned)dims[3];
    delete [] _data;
    _data = new T[size()];
    makeImageFromMatlabData(matlabArray,classID);
  }
}

private:

/*****************************************************************
 * private routines used for transfering a CImg<T> to a mxArray
 * here also, we have to exchange the x and y dims so we get the
 * expected MATLAB array.
 */
template <typename c> void populate_maltlab_array(c *const mdata) const {
  cimg_forXYZC(*this,x,y,z,v) mdata[((v*_depth + z)*_width + x)*_height + y] = (c)(*this)(x,y,z,v);
}

/*************************************************
 * the specialized version for "logical" entries
 */
void populate_maltlab_array(mxLogical *const mdata) const {
  cimg_forXYZC(*this,x,y,z,v) mdata[((v*_depth + z)*_width + x)*_height + y] = (mxLogical)((*this)(x,y,z,v)!=0);
}

public:

/******************************************
 * export a CImg image to a MATLAB array.
 **/
mxArray *toMatlab(mxClassID classID=mxDOUBLE_CLASS, const bool squeeze=false) const {
  if (!isNumericalClassID(classID)) {
#if cimg_debug>1
    cimg::warn("Invalid MATLAB Class Id Specified.");
#endif
    return 0;
  }
  mwSize dims[4];
  dims[0] = (mwSize)_height;
  dims[1] = (mwSize)_width;
  dims[2] = (mwSize)_depth;
  dims[3] = (mwSize)_spectrum;

  if (squeeze && _depth == 1) {
    dims[2] = (mwSize)_spectrum;
    dims[3] = (mwSize)1;
  }
  mxArray *matlabArray = mxCreateNumericArray((mwSize)4,dims,classID,mxREAL);
  if (classID==mxLOGICAL_CLASS) {
    mxLogical *mdata = mxGetLogicals(matlabArray);
    populate_maltlab_array(mdata);
  } else {
    void *mdata = mxGetPr(matlabArray);
    switch (classID) {
    case mxDOUBLE_CLASS : populate_maltlab_array((real64_T*)mdata); break;
    case mxSINGLE_CLASS : populate_maltlab_array((real32_T*)mdata); break;
    case mxINT8_CLASS : populate_maltlab_array((int8_T*)mdata); break;
    case mxUINT8_CLASS : populate_maltlab_array((uint8_T*)mdata); break;
    case mxINT16_CLASS : populate_maltlab_array((int16_T*)mdata); break;
    case mxUINT16_CLASS : populate_maltlab_array((uint16_T*)mdata); break;
    case mxINT32_CLASS : populate_maltlab_array((int32_T*)mdata); break;
    case mxUINT32_CLASS : populate_maltlab_array((uint32_T*)mdata); break;
    case mxINT64_CLASS : populate_maltlab_array((int64_T*)mdata); break;
    case mxUINT64_CLASS : populate_maltlab_array((uint64_T*)mdata); break;
    }
  }
  return matlabArray;
}

// end of cimgmatlab.h
#endif
