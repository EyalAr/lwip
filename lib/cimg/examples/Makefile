#
#  File        : Makefile
#                ( Makefile for GNU 'make' utility )
#
#  Description : Makefile for compiling CImg-based code on Unix.
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

#-------------------------------------------------------
# Define the list of files to be compiled
# (name of the source files without the .cpp extension)
#-------------------------------------------------------

# Files which do not necessarily require external libraries to run.
CIMG_FILES = CImg_demo \
	     captcha \
             curve_editor2d \
	     dtmri_view3d \
	     edge_explorer2d \
	     fade_images \
	     gaussian_fit1d \
             generate_loop_macros \
	     hough_transform2d \
	     image_registration2d \
	     image2ascii \
	     image_surface3d \
	     jawbreaker \
	     mcf_levelsets2d \
	     mcf_levelsets3d \
	     odykill \
	     pde_heatflow2d \
	     pde_TschumperleDeriche2d \
	     plotter1d \
	     radon_transform2d \
	     scene3d \
	     spherical_function3d \
	     tetris \
	     tron \
	     tutorial \
	     wavelet_atrous \
	     use_chlpca \
	     use_draw_gradient \
	     use_nlmeans \
             use_patchmatch \
	     use_skeleton \
	     use_RGBclass \

# Files which requires external libraries to run.
CIMG_EXTRA_FILES = use_tiff_stream use_jpeg_buffer

#---------------------------------
# Set correct variables and paths
#---------------------------------
CIMG_VERSION = 1.5.9_rolling140625
X11PATH      = /usr/X11R6
CC           = g++
EXEPFX       =
CCVER       = $(CC)
ifeq ($(CC),g++)
CCVER        = `$(CC) -v 2>&1 | tail -n 1`
endif
ifeq ($(CC),clang++)
CCVER        = `$(CC) -v 2>&1 | head -n 1`
endif
ifeq ($(CC),icc)
CCVER        = "icc \( `$(CC) -v 2>&1`\)"
CFLAGS       = -I..
LDFLAGS      =
else
CFLAGS       = -I.. -Wall -W
LDFLAGS      = -lm
endif

#--------------------------------------------------
# Set compilation flags allowing to customize CImg
#--------------------------------------------------

# Flags to enable strict code standards
ifeq ($(CC),icc)
CIMG_ANSI_CFLAGS = -ansi
else
CIMG_ANSI_CFLAGS = -ansi -pedantic
endif

# Flags to enable code debugging.
CIMG_DEBUG_CFLAGS = -Dcimg_verbosity=3 -Dcimg_strict_warnings -g

# Flags to enable color output messages.
# (requires a VT100 compatible terminal)
CIMG_VT100_CFLAGS = -Dcimg_use_vt100

# Flags to enable code optimization by the compiler.
ifeq ($(CC),icc)
CIMG_OPT_CFLAGS = -O3 -ipo -no-prec-div
else
CIMG_OPT_CFLAGS = -O3 -fno-tree-pre
endif

# Flags to enable OpenMP support.
ifeq ($(CC),icc)
CIMG_OPENMP_CFLAGS = -Dcimg_use_openmp -openmp -i-static
else
CIMG_OPENMP_CFLAGS = -Dcimg_use_openmp -fopenmp
endif

# Flags to enable OpenCV support.
CIMG_OPENCV_CFLAGS = -Dcimg_use_opencv -I/usr/include/opencv
CIMG_OPENCV_LDFLAGS = -lopencv_core -lopencv_highgui
#CIMG_OPENCV_LDFLAGS = -lcv -lhighgui    #-> Use this for OpenCV < 2.2.0

# Flags used to disable display capablities of CImg
CIMG_NODISPLAY_CFLAGS = -Dcimg_display=0

# Flags to enable the use of the X11 library.
# (X11 is used by CImg to handle display windows)
# !!! For 64bits systems : replace -L$(X11PATH)/lib by -L$(X11PATH)/lib64 !!!
CIMG_X11_CFLAGS = -I$(X11PATH)/include
CIMG_X11_LDFLAGS = -L$(X11PATH)/lib -lpthread -lX11

# Flags to enable fast image display, using the XSHM library (when using X11).
# !!! Seems to randomly crash when used on MacOSX and 64bits systems, so use it only when necessary !!!
CIMG_XSHM_CFLAGS = # -Dcimg_use_xshm
CIMG_XSHM_LDFLAGS = # -lXext

# Flags to enable GDI32 display (Windows native).
CIMG_GDI32_CFLAGS = -mwindows
CIMG_GDI32_LDFLAGS = -lgdi32

# Flags to enable screen mode switching, using the XRandr library (when using X11).
# ( http://www.x.org/wiki/Projects/XRandR )
# !!! Not supported by the X11 server on MacOSX, so do not use it on MacOSX !!!
CIMG_XRANDR_CFLAGS = -Dcimg_use_xrandr
CIMG_XRANDR_LDFLAGS = -lXrandr

# Flags to enable native support for PNG image files, using the PNG library.
# ( http://www.libpng.org/ )
CIMG_PNG_CFLAGS = -Dcimg_use_png
CIMG_PNG_LDFLAGS = -lpng -lz

# Flags to enable native support for JPEG image files, using the JPEG library.
# ( http://www.ijg.org/ )
CIMG_JPEG_CFLAGS = -Dcimg_use_jpeg
CIMG_JPEG_LDFLAGS = -ljpeg

# Flags to enable native support for TIFF image files, using the TIFF library.
# ( http://www.libtiff.org/ )
CIMG_TIFF_CFLAGS = -Dcimg_use_tiff
CIMG_TIFF_LDFLAGS = -ltiff

# Flags to enable native support for MINC2 image files, using the MINC2 library.
# ( http://en.wikibooks.org/wiki/MINC/Reference/MINC2.0_Users_Guide )
CIMG_MINC2_CFLAGS = -Dcimg_use_minc2 -I${HOME}/local/include
CIMG_MINC2_LDFLAGS = -lminc_io -lvolume_io2 -lminc2 -lnetcdf -lhdf5 -lz -L${HOME}/local/lib

# Flags to enable native support for EXR image files, using the OpenEXR library.
# ( http://www.openexr.com/ )
CIMG_EXR_CFLAGS = -Dcimg_use_openexr -I/usr/include/OpenEXR
CIMG_EXR_LDFLAGS = -lIlmImf -lHalf

# Flags to enable native support for various video files, using the FFMPEG library.
# ( http://www.ffmpeg.org/ )
CIMG_FFMPEG_CFLAGS = -Dcimg_use_ffmpeg -D__STDC_CONSTANT_MACROS -I/usr/include/libavcodec -I/usr/include/libavformat -I/usr/include/libswscale -I/usr/include/ffmpeg
CIMG_FFMPEG_LDFLAGS = -lavcodec -lavformat -lswscale

# Flags to enable native support for compressed .cimgz files, using the Zlib library.
# ( http://www.zlib.net/ )
CIMG_ZLIB_CFLAGS = -Dcimg_use_zlib
CIMG_ZLIB_LDFLAGS = -lz

# Flags to enable native support of most classical image file formats, using the Magick++ library.
# ( http://www.imagemagick.org/Magick++/ )
CIMG_MAGICK_CFLAGS = -Dcimg_use_magick `Magick++-config --cppflags` `Magick++-config --cxxflags`
CIMG_MAGICK_LDFLAGS = `Magick++-config --ldflags` `Magick++-config --libs`

# Flags to enable faster Discrete Fourier Transform computation, using the FFTW3 library
# ( http://www.fftw.org/ )
CIMG_FFTW3_CFLAGS = -Dcimg_use_fftw3
ifeq ($(OSTYPE),msys)
CIMG_FFTW3_LDFLAGS = -lfftw3-3
else
CIMG_FFTW3_LDFLAGS = -lfftw3 -lfftw3_threads
endif

# Flags to enable the use of LAPACK routines for matrix computation
# ( http://www.netlib.org/lapack/ )
CIMG_LAPACK_CFLAGS = -Dcimg_use_lapack
CIMG_LAPACK_LDFLAGS = -lblas -lg2c -llapack

# Flags to enable the use of the Board library
# ( http://libboard.sourceforge.net/ )
CIMG_BOARD_CFLAGS = -Dcimg_use_board -I/usr/include/board
CIMG_BOARD_LDFLAGS = -lboard

# Flags to compile on Sun Solaris
CIMG_SOLARIS_LDFLAGS = -R$(X11PATH)/lib -lrt -lnsl -lsocket

# Flags to compile GIMP plug-ins.
ifeq ($(MSYSTEM),MINGW32)
CIMG_GIMP_CFLAGS = -mwindows
endif

#-------------------------
# Define Makefile entries
#-------------------------
.cpp:
	@echo
	@echo "** Compiling '$* ($(CIMG_VERSION))' with '$(CCVER)'"
	@echo
	$(CC) -o $(EXEPFX)$* $< $(CFLAGS) $(CONF_CFLAGS) $(LDFLAGS) $(CONF_LDFLAGS)
ifeq ($(STRIP_EXE),true)
ifeq ($(MSYSTEM),MINGW32)
	strip $(EXEPFX)$*.exe
else
	strip $(EXEPFX)$*
endif
endif
menu:
	@echo
	@echo "CImg Library $(CIMG_VERSION) : Examples"
	@echo "-----------------------------"
	@echo "  > linux    : Linux/BSD target, X11 display, optimizations disabled."
	@echo "  > dlinux   : Linux/BSD target, X11 display, debug mode."
	@echo "  > olinux   : Linux/BSD target, X11 display, optimizations enabled."
	@echo "  > mlinux   : Linus/BSD target, no display, minimal features, optimizations enabled."
	@echo "  > Mlinux   : Linux/BSD target, X11 display, maximal features, optimizations enabled."
	@echo
	@echo "  > solaris  : Sun Solaris target, X11 display, optimizations disabled."
	@echo "  > dsolaris : Sun Solaris target, X11 display, debug mode."
	@echo "  > osolaris : Sun Solaris target, X11 display, optimizations enabled."
	@echo "  > msolaris : Sun Solaris target, no display, minimal features, optimizations enabled."
	@echo "  > Msolaris : Sun Solaris target, X11 display, maximal features, optimizations enabled."
	@echo
	@echo "  > macosx   : MacOSX target, X11 display, optimizations disabled."
	@echo "  > dmacosx  : MacOSX target, X11 display, debug mode."
	@echo "  > omacosx  : MacOSX target, X11 display, optimizations enabled."
	@echo "  > mmacosx  : MacOSX target, no display, minimal features, optimizations enabled."
	@echo "  > Mmacosx  : MacOSX target, X11 display, maximal features, optimizations enabled."
	@echo
	@echo "  > windows  : Windows target, GDI32 display, optimizations disabled."
	@echo "  > dwindows : Windows target, GDI32 display, debug mode."
	@echo "  > owindows : Windows target, GDI32 display, optimizations enabled."
	@echo "  > mwindows : Windows target, no display, minimal features, optimizations enabled."
	@echo "  > Mwindows : Windows target, GDI32 display, maximal features, optimizations enabled."
	@echo
	@echo "  > clean    : Clean generated files."
	@echo
	@echo "Choose your option :"
	@read CHOICE; echo; $(MAKE) $$CHOICE; echo; echo "> Next time, you can bypass the menu by typing directly 'make $$CHOICE'"; echo;

all: $(CIMG_FILES)

clean:
	rm -rf *.exe *.o *~ \#* $(CIMG_FILES) $(CIMG_EXTRA_FILES)
ifneq ($(EXEPFX),)
	rm -f $(EXEPFX)*
endif

# Custom user-defined target
custom:
	@$(MAKE) \
"CONF_CFLAGS = \
$(CIMG_ANSI_CFLAGS) \
$(CIMG_VT100_CFLAGS) \
$(CIMG_TIFF_CFLAGS) \
$(CIMG_X11_CFLAGS) \
$(CIMG_XSHM_CFLAGS)" \
"CONF_LDFLAGS = \
$(CIMG_X11_LDFLAGS) \
$(CIMG_TIFF_LDFLAGS) \
$(CIMG_XSHM_LDFLAGS)" \
all $(CIMG_EXTRA_FILES)

# Linux/BSD/Mac OSX targets, with X11 display.
linux:
	@$(MAKE) \
"CONF_CFLAGS = \
$(CIMG_ANSI_CFLAGS) \
$(CIMG_VT100_CFLAGS) \
$(CIMG_X11_CFLAGS) \
$(CIMG_XSHM_CFLAGS)" \
"CONF_LDFLAGS = \
$(CIMG_X11_LDFLAGS) \
$(CIMG_XSHM_LDFLAGS)" \
all

dlinux:
	@$(MAKE) \
"CONF_CFLAGS = \
$(CIMG_ANSI_CFLAGS) \
$(CIMG_DEBUG_CFLAGS) \
$(CIMG_VT100_CFLAGS) \
$(CIMG_X11_CFLAGS) \
$(CIMG_XSHM_CFLAGS)" \
"CONF_LDFLAGS = \
$(CIMG_X11_LDFLAGS) \
$(CIMG_XSHM_LDFLAGS)" \
all

olinux:
	@$(MAKE) \
"CONF_CFLAGS = \
$(CIMG_ANSI_CFLAGS) \
$(CIMG_OPT_CFLAGS) \
$(CIMG_OPENMP_CFLAGS) \
$(CIMG_VT100_CFLAGS) \
$(CIMG_X11_CFLAGS) \
$(CIMG_XSHM_CFLAGS)" \
"CONF_LDFLAGS = \
$(CIMG_X11_LDFLAGS) \
$(CIMG_XSHM_LDFLAGS)" \
"STRIP_EXE=true" \
all

mlinux:
	@$(MAKE) \
"CONF_CFLAGS = \
$(CIMG_ANSI_CFLAGS) \
$(CIMG_NODISPLAY_CFLAGS) \
$(CIMG_OPT_CFLAGS)" \
"STRIP_EXE=true" \
all

Mlinux:
	@$(MAKE) \
"CONF_CFLAGS = \
$(CIMG_OPT_CFLAGS) \
$(CIMG_VT100_CFLAGS) \
$(CIMG_X11_CFLAGS) \
$(CIMG_XSHM_CFLAGS) \
$(CIMG_XRANDR_CFLAGS) \
$(CIMG_TIFF_CFLAGS) \
$(CIMG_EXR_CFLAGS) \
$(CIMG_PNG_CFLAGS) \
$(CIMG_JPEG_CFLAGS) \
$(CIMG_ZLIB_CFLAGS) \
$(CIMG_OPENCV_CFLAGS) \
$(CIMG_MAGICK_CFLAGS) \
$(CIMG_FFTW3_CFLAGS)" \
"CONF_LDFLAGS = \
$(CIMG_X11_LDFLAGS) \
$(CIMG_XSHM_LDFLAGS) \
$(CIMG_XRANDR_LDFLAGS) \
$(CIMG_TIFF_LDFLAGS) -ltiffxx \
$(CIMG_EXR_LDFLAGS) \
$(CIMG_PNG_LDFLAGS) \
$(CIMG_JPEG_LDFLAGS) \
$(CIMG_ZLIB_LDFLAGS) \
$(CIMG_OPENCV_LDFLAGS) \
$(CIMG_MAGICK_LDFLAGS) \
$(CIMG_FFTW3_LDFLAGS)" \
"STRIP_EXE=true" \
all $(CIMG_EXTRA_FILES)

# Sun Solaris targets, with X11 display.
solaris:
	@$(MAKE) \
"CONF_CFLAGS = \
$(CIMG_ANSI_CFLAGS) \
$(CIMG_VT100_CFLAGS) \
$(CIMG_X11_CFLAGS) \
$(CIMG_XSHM_CFLAGS)" \
"CONF_LDFLAGS = \
$(CIMG_SOLARIS_LDFLAGS) \
$(CIMG_X11_LDFLAGS) \
$(CIMG_XSHM_LDFLAGS)" \
all

dsolaris:
	@$(MAKE) \
"CONF_CFLAGS = \
$(CIMG_ANSI_CFLAGS) \
$(CIMG_DEBUG_CFLAGS) \
$(CIMG_VT100_CFLAGS) \
$(CIMG_X11_CFLAGS) \
$(CIMG_XSHM_CFLAGS)" \
"CONF_LDFLAGS = \
$(CIMG_SOLARIS_LDFLAGS) \
$(CIMG_X11_LDFLAGS) \
$(CIMG_XSHM_LDFLAGS)" \
all

osolaris:
	@$(MAKE) \
"CONF_CFLAGS = \
$(CIMG_ANSI_CFLAGS) \
$(CIMG_OPT_CFLAGS) \
$(CIMG_VT100_CFLAGS) \
$(CIMG_X11_CFLAGS) \
$(CIMG_XSHM_CFLAGS)" \
"CONF_LDFLAGS = \
$(CIMG_SOLARIS_LDFLAGS) \
$(CIMG_X11_LDFLAGS) \
$(CIMG_XSHM_LDFLAGS)" \
"STRIP_EXE=true" \
all

msolaris:
	@$(MAKE) \
"CONF_CFLAGS = \
$(CIMG_ANSI_CFLAGS) \
$(CIMG_NODISPLAY_CFLAGS) \
$(CIMG_OPT_CFLAGS)" \
"STRIP_EXE=true" \
all

Msolaris:
	@$(MAKE) \
"CONF_CFLAGS = \
$(CIMG_OPT_CFLAGS) \
$(CIMG_VT100_CFLAGS) \
$(CIMG_X11_CFLAGS) \
$(CIMG_XSHM_CFLAGS) \
$(CIMG_XRANDR_CFLAGS) \
$(CIMG_TIFF_CFLAGS) \
$(CIMG_MINC2_CFLAGS) \
$(CIMG_EXR_CFLAGS) \
$(CIMG_PNG_CFLAGS) \
$(CIMG_JPEG_CFLAGS) \
$(CIMG_ZLIB_CFLAGS) \
$(CIMG_OPENCV_CFLAGS) \
$(CIMG_MAGICK_CFLAGS) \
$(CIMG_FFTW3_CFLAGS)" \
"CONF_LDFLAGS = \
$(CIMG_SOLARIS_LDFLAGS) \
$(CIMG_X11_LDFLAGS) \
$(CIMG_XSHM_LDFLAGS) \
$(CIMG_XRANDR_LDFLAGS) \
$(CIMG_TIFF_LDFLAGS) \
$(CIMG_MINC2_LDFLAGS) \
$(CIMG_EXR_LDFLAGS) \
$(CIMG_PNG_LDFLAGS) \
$(CIMG_JPEG_LDFLAGS) \
$(CIMG_ZLIB_LDFLAGS) \
$(CIMG_OPENCV_LDFLAGS) \
$(CIMG_MAGICK_LDFLAGS) \
$(CIMG_FFTW3_LDFLAGS)" \
"STRIP_EXE=true" \
all $(CIMG_EXTRA_FILES)

# MacOsX targets, with X11 display.
macosx:
	@$(MAKE) \
"CONF_CFLAGS = \
$(CIMG_ANSI_CFLAGS) \
$(CIMG_VT100_CFLAGS) \
$(CIMG_X11_CFLAGS)" \
"CONF_LDFLAGS = \
$(CIMG_X11_LDFLAGS)" \
all

dmacosx:
	@$(MAKE) \
"CONF_CFLAGS = \
$(CIMG_ANSI_CFLAGS) \
$(CIMG_DEBUG_CFLAGS) \
$(CIMG_VT100_CFLAGS) \
$(CIMG_X11_CFLAGS)" \
"CONF_LDFLAGS = \
$(CIMG_X11_LDFLAGS)" \
all

omacosx:
	@$(MAKE) \
"CONF_CFLAGS = \
$(CIMG_ANSI_CFLAGS) \
$(CIMG_OPT_CFLAGS) \
$(CIMG_VT100_CFLAGS) \
$(CIMG_X11_CFLAGS)" \
"CONF_LDFLAGS = \
$(CIMG_X11_LDFLAGS)" \
all

mmacosx:
	@$(MAKE) \
"CONF_CFLAGS = \
$(CIMG_ANSI_CFLAGS) \
$(CIMG_NODISPLAY_CFLAGS) \
$(CIMG_OPT_CFLAGS)" \
all

Mmacosx:
	@$(MAKE) \
"CONF_CFLAGS = \
$(CIMG_OPT_CFLAGS) \
$(CIMG_VT100_CFLAGS) \
$(CIMG_X11_CFLAGS) \
$(CIMG_TIFF_CFLAGS) \
$(CIMG_MINC2_CFLAGS) \
$(CIMG_EXR_CFLAGS) \
$(CIMG_PNG_CFLAGS) \
$(CIMG_JPEG_CFLAGS) \
$(CIMG_ZLIB_CFLAGS) \
$(CIMG_OPENCV_CFLAGS) \
$(CIMG_MAGICK_CFLAGS) \
$(CIMG_FFTW3_CFLAGS)" \
"CONF_LDFLAGS = \
$(CIMG_X11_LDFLAGS) \
$(CIMG_TIFF_LDFLAGS) \
$(CIMG_MINC2_LDFLAGS) \
$(CIMG_EXR_LDFLAGS) \
$(CIMG_PNG_LDFLAGS) \
$(CIMG_JPEG_LDFLAGS) \
$(CIMG_ZLIB_LDFLAGS) \
$(CIMG_OPENCV_LDFLAGS) \
$(CIMG_MAGICK_LDFLAGS) \
$(CIMG_FFTW3_LDFLAGS)" \
all $(CIMG_EXTRA_FILES)

# Windows targets, with GDI32 display.
windows:
	@$(MAKE) \
"CONF_CFLAGS = " \
"CONF_LDFLAGS = \
$(CIMG_GDI32_LDFLAGS)" \
all

dwindows:
	@$(MAKE) \
"CONF_CFLAGS = \
$(CIMG_DEBUG_CFLAGS)" \
"CONF_LDFLAGS = \
$(CIMG_GDI32_LDFLAGS)" \
all

owindows:
	@$(MAKE) \
"CONF_CFLAGS = \
$(CIMG_OPT_CFLAGS)" \
"CONF_LDFLAGS = \
$(CIMG_GDI32_LDFLAGS)" \
"STRIP_EXE=true" \
all

mwindows:
	@$(MAKE) \
"CONF_CFLAGS = \
$(CIMG_NODISPLAY_CFLAGS) \
$(CIMG_OPT_CFLAGS)" \
"STRIP_EXE=true" \
all

Mwindows:
	@$(MAKE) \
"CONF_CFLAGS = \
$(CIMG_OPT_CFLAGS) \
$(CIMG_TIFF_CFLAGS) \
$(CIMG_PNG_CFLAGS) \
$(CIMG_JPEG_CFLAGS) \
$(CIMG_ZLIB_CFLAGS) \
$(CIMG_OPENCV_CFLAGS) \
$(CIMG_FFTW3_CFLAGS)" \
"CONF_LDFLAGS = \
$(CIMG_GDI32_LDFLAGS) \
$(CIMG_TIFF_LDFLAGS) \
$(CIMG_PNG_LDFLAGS) \
$(CIMG_JPEG_LDFLAGS) \
$(CIMG_ZLIB_LDFLAGS) \
$(CIMG_OPENCV_LDFLAGS) \
$(CIMG_FFTW3_LDFLAGS)" \
"STRIP_EXE=true" \
all $(CIMG_EXTRA_FILES)

#-----------------
# End of makefile
#-----------------
