/*------------------------------------------------------------------------
#
#  File        : CImg_documentation.h
#
#  Description : Extra documentation file for the CImg Library.
#                Used by doxygen to generate the reference documentation.
#                ( http://cimg.sourceforge.net )
#
#  Copyright   : David Tschumperle
#                ( http://tschumperle.users.greyc.fr/ )
#
#
-------------------------------------------------------------------------*/

/*-----------------------------------

  Main reference documentation page

  -------------------------------------*/

/**
   \mainpage

   This is the reference documentation of <a href="http://cimg.sourceforge.net">the CImg Library</a>,
   the C++ template image processing library.
   This documentation have been generated using the tool <a href="http://www.doxygen.org">doxygen</a>.
   It contains a detailed description of all classes and functions of the %CImg Library.
   If you have downloaded the CImg package, you actually have a local copy of these pages in the
   \c CImg/html/reference/ directory.

   Use the menu above to navigate through the documentation pages.
   As a first step, you may look at the list of <a href="modules.html">available modules</a>.

   You may be interested also in the
   <a href="../CImg_slides.pdf">presentation slides</a> presenting an overview
   of the %CImg Library capabilities.

**/

/*-----------------------------------

  CImg Library overview

  -------------------------------------*/

/** \addtogroup cimg_overview CImg Library Overview */
/*@{*/
/**
  \page foo2

  The <b>CImg Library</b> is an image processing library, designed for C++ programmers.
  It provides useful classes and functions to load/save, display and process various types of images.

  \section s1 Library structure

  The %CImg Library consists in a single header file <tt>CImg.h</tt> providing a set of C++ template classes that
  can be used in your own sources, to load/save, process and display images or list of images.
  Very portable (Unix/X11,Windows, MacOS X, FreeBSD,..), efficient, simple to use, it's a pleasant toolkit
  for coding image processing stuffs in C++.

  The header file <tt>CImg.h</tt> contains all the classes and functions that compose the library itself.
  This is one originality of the %CImg Library. This particularly means that :
  - No pre-compilation of the library is needed, since the compilation of the CImg functions is done at the same time as
  the compilation of your own C++ code.
  - No complex dependencies have to be handled : Just include the <tt>CImg.h</tt> file, and you get a working C++ image processing toolkit.
  - The compilation is done on the fly : only CImg functionalities really used by your program are compiled and appear in the
  compiled executable program. This leads to very compact code, without any unused stuffs.
  - Class members and functions are inlined, leading to better performance during the program execution.

  The %CImg Library is structured as follows :

  - All library classes and functions are defined in the namespace \ref cimg_library. This namespace
  encapsulates the library functionalities and avoid any class name collision that could happen with
  other includes. Generally, one uses this namespace as a default namespace :
  \code
  #include "CImg.h"
  using namespace cimg_library;
  ...
  \endcode

  - The namespace \ref cimg_library::cimg defines a set of \e low-level functions and variables used by the library.
  Documented functions in this namespace can be safely used in your own program. But, \b never use the
  \ref cimg_library::cimg namespace as a default namespace, since it contains functions whose names are already
  defined in the standard C/C++ library.

  - The class \ref cimg_library::CImg represents images up to 4-dimensions wide, containing pixels of type \c T
  (template parameter). This is actually the main class of the library.

  - The class \ref cimg_library::CImgList represents lists of cimg_library::CImg<T> images. It can be used for instance
  to store different frames of an image sequence.

  - The class \ref cimg_library::CImgDisplay is able to display images or image lists into graphical display windows.
  As you may guess, the code of this class is highly system-dependent but this is transparent for the programmer,
  as environment variables are automatically set by the CImg library (see also \ref cimg_environment).

  - The class \ref cimg_library::CImgException (and its subclasses) are used by the library to throw exceptions
  when errors occur. Those exceptions can be catched with a bloc <tt>try { ..} catch (CImgException) { .. }</tt>.
  Subclasses define precisely the type of encountered errors.

  Knowing these four classes is \b enough to get benefit of the %CImg Library functionalities.


  \section s2 CImg version of "Hello world".

  Below is a very simple code that creates a "Hello World" image. This shows you basically how a CImg program looks like.

  \code
  #include "CImg.h"
  using namespace cimg_library;

  int main() {
    CImg<unsigned char> img(640,400,1,3);        // Define a 640x400 color image with 8 bits per color component.
    img.fill(0);                                 // Set pixel values to 0 (color : black)
    unsigned char purple[] = { 255,0,255 };      // Define a purple color
    img.draw_text(100,100,"Hello World",purple); // Draw a purple "Hello world" at coordinates (100,100).
    img.display("My first CImg code");           // Display the image in a display window.
    return 0;
  }
  \endcode

  Which can be also written in a more compact way as :

  \code
  #include "CImg.h"
  using namespace cimg_library;

  int main() {
    const unsigned char purple[] = { 255,0,255 };
    CImg<unsigned char>(640,400,1,3,0).draw_text(100,100,"Hello World",purple).display("My first CImg code");
    return 0;
  }
  \endcode

  Generally, you can write very small code that performs complex image processing tasks. The %CImg Library is very simple
  to use and provide a lot of interesting algorithms for image manipulation.

  \section s3 How to compile ?

  The CImg library is a very light and user-friendly library : only standard system libraries are used.
  It avoid to handle complex dependancies and problems with library compatibility.
  The only thing you need is a (quite modern) C++ compiler :

  - <b>Microsoft Visual C++ 6.0, Visual Studio.NET and Visual Express Edition</b> : Use project files and solution files provided in the
  %CImg Library package (directory 'compilation/') to see how it works.
  - <b>Intel ICL compiler</b> : Use the following command to compile a CImg-based program with ICL :
  \code
  icl /Ox hello_world.cpp user32.lib gdi32.lib
  \endcode
  - <b>g++ (MingW windows version)</b> : Use the following command to compile a CImg-based program with g++, on Windows :
  \code
  g++ -o hello_word.exe hello_word.cpp -O2 -lgdi32
  \endcode
  - <b>g++ (Linux version)</b> : Use the following command to compile a CImg-based program with g++, on Linux :
  \code
  g++ -o hello_word.exe hello_world.cpp -O2 -L/usr/X11R6/lib -lm -lpthread -lX11
  \endcode
  - <b>g++ (Solaris version)</b> : Use the following command to compile a CImg-based program with g++, on Solaris :
  \code
  g++ -o hello_word.exe hello_world.cpp -O2 -lm -lpthread -R/usr/X11R6/lib -lrt -lnsl -lsocket
  \endcode
  - <b>g++ (Mac OS X version)</b> : Use the following command to compile a CImg-based program with g++, on Mac OS X :
  \code
  g++ -o hello_word.exe hello_world.cpp -O2 -lm -lpthread -I/usr/X11R6/include -L/usr/X11R6/lib -lm -lpthread -lX11
  \endcode
  - <b>Dev-Cpp</b> : Use the project file provided in the CImg library package to see how it works.

  If you are using another compilers and encounter problems, please
  <a href="http://www.greyc.ensicaen.fr/~dtschump">write me</a> since maintaining compatibility is one
  of the priority of the %CImg Library. Nevertheless, old compilers that does not respect the C++ norm will not
  support the %CImg Library.

  \section s4 What's next ?

  If you are ready to get more, and to start writing more serious programs
  with CImg, you are invited to go to the \ref cimg_tutorial section.

**/
/*@}*/

/*-----------------------------------

   FAQ : Frequently Asked Questions

  -------------------------------------*/

/** \addtogroup cimg_faq FAQ : Frequently Asked Questions. */
/*@{*/
/**
  \page foofaq

  \section ssf0 FAQ Summary

  - <a href="#sf1">General information and availability</a>
    - <a href="#ssf11">What is the CImg Library ?</a>
    - <a href="#ssf12">What platforms are supported ?</a>
    - <a href="#ssf13">How is CImg distributed ?</a>
    - <a href="#ssf14">What kind of people are concerned by CImg ?</a>
    - <a href="#ssf15">What are the specificities of the CeCILL license ?</a>
    - <a href="#ssf16">Who is behind CImg ?</a>

  - <a href="#sf2">C++ related questions</a>
    - <a href="#ssf21">What is the level of C++ knowledge needed to use CImg ?</a>
    - <a href="#ssf22">How to use CImg in my own C++ program ?</a>
    - <a href="#ssf23">Why is CImg entirely contained in a single header file ?</a>

  - <a href="#sf3">Other resources</a>
    - <a href="#ssf31">Translations</a>

  \section sf1 1. General information and availability

  \subsection ssf11 1.1. What is the CImg Library ?

  The CImg Library is an <i>open-source C++ toolkit for image processing</i>.\n

  It mainly consists in a (big) single header file
  <a href="http://cimg.cvs.sourceforge.net/cimg/CImg/CImg.h?view=markup">CImg.h</a>
  providing a set of C++ classes and functions that can be used in your own sources,
  to load/save, manage/process and display generic images.
  It's actually a very simple and pleasant toolkit for coding image processing stuffs in C++ :
  Just include the header file <tt>CImg.h</tt>, and you are ready to handle images in your C++ programs.

  \subsection ssf12 1.2. What platforms are supported ?

  CImg has been designed with <i>portability</i> in mind.
  It is regularly tested on different architectures and compilers,
  and should also work on any decent OS having a decent C++ compiler.
  Before each release, the CImg Library is compiled under these different configurations :
  \li PC Linux 32 bits, with g++.
  \li PC Windows 32 bits, with Visual C++ 6.0.
  \li PC Windows 32 bits, with Visual C++ Express Edition.
  \li Sun SPARC Solaris 32 bits, with g++.
  \li Mac PPC with OS X and g++.

  CImg has a minimal number of dependencies. In its minimal version, it can be compiled only with standard C++ headers.
  Anyway, it has interesting extension capabilities and can use external libraries to perform specific tasks more
  efficiently (Fourier Transform computation using FFTW for instance).

  \subsection ssf13 1.3. How is CImg distributed ?

  The CImg Library is freely distributed as a complete .zip compressed package, hosted at the
  <a href="http://sourceforge.net/project/showfiles.php?group_id=96492">Sourceforge servers</a>.\n
  The package is distributed under the <a href="http://www.cecill.info">CeCILL license</a>.

  This package contains :
  - The main library file <a href="http://cimg.cvs.sourceforge.net/cimg/CImg/CImg.h?view=markup">CImg.h</a> (C++ header file).
  - Several C++ source code showing <a href="http://cimg.cvs.sourceforge.net/cimg/CImg/examples/">examples of using CImg</a>.
  - A complete library documentation, in <a href="index.html">HTML</a> and <a href="../CImg_reference.pdf">PDF</a> formats.
  - Additional <a href="http://cimg.cvs.sourceforge.net/cimg/CImg/plugins/">library plug-ins</a> that can be used to extend
  library capabilities for specific uses.

  The CImg Library is a quite lightweight library which is easy to maintain (due to its particular structure), and thus
  has a fast rythm of release. A new version of the CImg package is released approximately every three months.

  \subsection ssf14 1.4. What kind of people are concerned by CImg ?

  The CImg library is an <i>image processing</i> library, primarily intended for computer scientists or students working in the fields
  of image processing or computer vision, and knowing bases of C++.
  As the library is handy and really easy to use, it can be also used by any programmer
  needing occasional tools for dealing with images in C++, since there are no standard library yet
  for this purpose.

  \subsection ssf15 1.5. What are the specificities of the CeCILL license ?

  The <a href="http://www.cecill.info">CeCILL license</a> governs the use of the CImg Library.
  This is an <i>open-source</i> license which gives you rights to access, use, modify and redistribute the source code,
  under certains conditions.
  There are two different variants of the CeCILL license used in CImg
  (namely
  <a href="http://www.cecill.info/licences/Licence_CeCILL_V2-en.html">CeCILL</a> and
  <a href="http://www.cecill.info/licences/Licence_CeCILL-C_V1-en.html">CeCILL-C</a>, all open-source),
  corresponding to different constraints on the source files :
  - The <a href="http://www.cecill.info/licences/Licence_CeCILL-C_V1-en.html">CeCILL-C</a> license is the most permissive one, close to
  the <i>GNU LGPL license</i>, and <i>applies <b>only</b> on the main library file
  <a href="http://cimg.cvs.sourceforge.net/cimg/CImg/CImg.h?view=markup">CImg.h</a></i>.
  Basically, this license allows to use <a href="http://cimg.cvs.sourceforge.net/cimg/CImg/CImg.h?view=markup">CImg.h</a>
  in a closed-source product without forcing you to redistribute the entire software source code. Anyway,
  if one modifies the <a href="http://cimg.cvs.sourceforge.net/cimg/CImg/CImg.h?view=markup">CImg.h</a> source file, one has to redistribute
  the modified version of the file that must be governed by the same <a href="http://www.cecill.info/licences/Licence_CeCILL-C_V1-en.html">CeCILL-C</a> license.

  - The <a href="http://www.cecill.info/licences/Licence_CeCILL_V2-en.html">CeCILL</a> license applies to all other files
  (source examples, plug-ins and documentation) of the CImg Library package, and is close (even <i>compatible</i>)
  with the <i>GNU GPL license</i>. It <i>does not allow</i> the use of these files in closed-source products.

  You are invited to read the complete descriptions of the
  the <a href="http://www.cecill.info/licences/Licence_CeCILL-C_V1-en.html">CeCILL-C</a>
  and <a href="http://www.cecill.info/licences/Licence_CeCILL_V2-en.html">CeCILL</a> licenses before releasing a
  software based on the CImg Library.

  \subsection ssf16 1.6. Who is behind CImg ?

  CImg has been started by
  <a href="http://tschumperle.users.greyc.fr/">David Tschumperle</a> at the beginning of his PhD thesis, in October 1999.
  He is still the main coordinator of the project.
  Since the first release at Sourceforge, a growing number of contributors has appeared.
  Due to the very simple and compact form of the library, submitting a contribution is quite easy and can be
  fastly integrated into the supported releases.
  List of contributors can be found on the front page.

  \section sf2 2. C++ related questions

  \subsection ssf21 2.1 What is the level of C++ knowledge needed to use CImg ?

  The CImg Library has been designed using C++ templates and object-oriented programming techniques,
  but in a very accessible level.
  There are only public classes without any derivation (just like C structures) and
  there is at most one template parameter for each CImg class (defining the pixel type of the images).
  The design is simple but clean, making the library accessible even for non professional C++ programmers, while proposing
  strong extension capabilities for C++ experts.

  \subsection ssf22 2.2 How to use CImg in my own C++ program ?

  Basically, you need to add these two lines in your C++ source code, in order
  to be able to work with CImg images :
  \code
  #include "CImg.h"
  using namespace cimg_library;
  \endcode

  \subsection ssf23 2.3 Why is CImg entirely contained in a single header file ?

  People are often surprised to see that the complete code of the library is contained in a single (big) C++ header file
  <a href="http://cimg.cvs.sourceforge.net/cimg/CImg/CImg.h?view=markup">CImg.h</a>.
  There are good practical and technical reasons to do that. Some arguments are listed below to justify this approach,
  so (I hope) you won't think this is a awkwardly C++ design of the CImg library :\n

  - First, the library is based on <i>template datatypes</i> (images with generic pixel type),
  meaning that the programmer is free to decide what type of image he instanciates in his code.
  Even if there are roughly a limited number of fully supported types (basically, the "atomic" types of C++ : <i>unsigned char, int, float, ...</i>),
  this is <i>not imaginable</i> to pre-compile the library classes and functions for <i>all possible atomic datatypes</i>,
  since many functions and methods can have two or three arguments having different template parameters.
  This really means <i>a huge number</i> of possible combinations. The size of the object binary file generated to cover all possible cases
  would be just <i>colossal</i>. Is the STL library a pre-compiled one ? No, CImg neither.
  CImg is not using a classical <i>.cpp</i> and <i>.h</i> mechanism, just like the STL.
  Architectures of C++ <i>template-based</i> libraries are somewhat special in this sense. This is a proven technical fact.

  - Second, why CImg does not have several header files, just like the STL does (one for each class for instance) ?
  This would be possible of course.
  There are only 4 classes in CImg, the two most important being <i>CImg<T></i> and <i>CImgList<T></i> representing respectively
  an image and a collection of images.
  But contrary to the STL library, these two CImg classes are strongly <i>inter-dependent</i>. All CImg algorithms
  are actually not defined as separate functions acting on containers (as the STL does with his header \<algorithm\>),
  but are directly methods of the image and image collection classes. This inter-dependence practically means that you
  will undoubtly need these two main classes at the same time if you are using CImg.
  If they were defined in separate header files, you would be forced to include both of them. What is the gain then ? No gain.\n
  Concerning the two other classes : You can disable the third most important class <i>CImgDisplay</i> of the CImg library, by setting the compilation
  macro <i>cimg_display</i> to 0, avoiding thus to compile this class if you don't use display capabilities of CImg in your code.
  But to be honest, this is a quite small class and doing this doesn't save much compilation time.
  The last and fourth class is <i>CImgException</i>, which is only few lines long and is obviously required in almost all methods of CImg.
  Including this one is <i>mandatory</i>.\n
  As a consequence, having a single header file instead of several ones is just a way for you to avoid including all of them,
  without any consequences on compilation time. This is both good technical and practical reasons to do like this.

  - Third, having a single header file has plenty of advantages : Simplicity for the user, and for the developers (maintenance is in fact easier).
  Look at the <tt>CImg.h</tt> file, it looks like a mess at a first glance, but it is in fact very well organized and structured.
  Finding pieces of code in CImg functions or methods is particularly easy and fast.
  Also, how about the fact that library installation problems just disappear ?
  Just bring <tt>CImg.h</tt> with you, put it in your source directory, and the library is ready to go !

  I admit the compilation time of CImg-based programs can be sometime long, but don't think that it is due to the fact that you are
  using a single header file. Using several header files wouldn't arrange anything since you would need all of them.
  Having a pre-compiled library object would be the only solution to speed up compilation time, but it is not possible at all,
  due to the too much generic nature of the library.

  \section sf3 3. Other resources
  \subsection ssf31 3.1 Translations

  This FAQ has been translated to <a href="http://science.webhostinggeeks.com/cimg-biblioteka">Serbo-Croatian</a> language by <a href="http://webhostinggeeks.com/"> Web Geeks </a>.

**/
/*@}*/

/*-----------------------------------

   Setting Environment Variables

  -------------------------------------*/

/** \addtogroup cimg_environment Setting Environment Variables */
/*@{*/
/**
  \page foo1

  The CImg library is a multiplatform library, working on a wide variety of systems.
  This implies the existence of some \e environment \e variables that must be correctly defined
  depending on your current system.
  Most of the time, the %CImg Library defines these variables automatically
  (for popular systems). Anyway, if your system is not recognized, you will have to set the environment
  variables by hand. Here is a quick explanations of environment variables.\n

  Setting the environment variables is done with the <tt>#define</tt> keyword.
  This setting must be done <i>before including the file <tt>CImg.h</tt></i> in your source code.
  For instance,
  defining the environment variable \c cimg_display would be done like this :
  \code
  #define cimg_display 0
  #include "CImg.h"
  ...
  \endcode

  Here are the different environment variables used by the %CImg Library :

  - \b \c cimg_OS : This variable defines the type of your Operating System. It can be set to \b 1 (\e Unix),
  \b 2 (\e Windows), or \b 0 (\e Other \e configuration).
  It should be actually auto-detected by the CImg library. If this is not the case (<tt>cimg_OS=0</tt>), you
  will probably have to tune the environment variables described below.

  - \b \c cimg_display : This variable defines the type of graphical library used to
  display images in windows. It can be set to 0 (no display library available), \b 1 (X11-based display) or
  \b 2 (Windows-GDI display).
  If you are running on a system without X11 or Windows-GDI ability, please set this variable to \c 0.
  This will disable the display support, since the %CImg Library doesn't contain the necessary code to display
  images on systems other than X11 or Windows GDI.

  - \b \c cimg_use_vt100 : This variable tells the library if the system terminal has VT100 color capabilities.
  It can be \e defined or \e not \e defined. Define this variable to get colored output on your terminal,
  when using the %CImg Library.

  - \b \c cimg_verbosity : This variable defines the level of run-time debug messages that will be displayed by
  the %CImg Library. It can be set to 0 (no debug messages), 1 (normal debug messages displayed on
  standard error), 2 (normal debug messages displayed in modal windows, which is
  the default value), or 3 (high debug messages). Note that setting this value to 3 may slow down your
  program since more debug tests are made by the library (particularly to check if pixel access is made outside
  image boundaries). See also CImgException to better understand how debug messages are working.

  - \b \c cimg_plugin : This variable tells the library to use a plugin file to add features to the CImg<T> class.
  Define it with the path of your plugin file, if you want to add member functions to the CImg<T> class,
  without having to modify directly the \c "<tt>CImg.h</tt>" file. An include of the plugin file is performed in the CImg<T>
  class. If \c cimg_plugin if not specified (default), no include is done.

  - \b \c cimglist_plugin : Same as \c cimg_plugin, but to add features to the CImgList<T> class.

  - \b \c cimgdisplay_plugin : Same as \c cimg_plugin, but to add features to the CImgDisplay<T> class.

  All these compilation variables can be checked, using the function cimg_library::cimg::info(), which
  displays a list of the different configuration variables and their values on the standard error output.
**/
/*@}*/


/** \addtogroup cimg_visual2005 How to use CImg library with Visual C++ 2005 Express Edition ?. */
/*@{*/
/**
  \page foo89198

  \section s13968 How to use CImg library with Visual C++ 2005 Express Edition ?

  This section has been written by Vincent Garcia and Alexandre Fournier from I3S/Sophia_Antipolis.

   - Download CImg library
   - Download and install Visual C++ 2005 Express Edition
   - Download and install Microsoft Windows SDK
   - Configure Visual C++ to take into account Microsoft SDK
         - 1. Go to menu "Tools -> options"
         - 2. Select option "Projects and Solutions -> VC++ Directories"
         - 3. In the select liste "Show directories for", choose "include files", and add C:\\Program Files\\Microsoft Platform SDK\\Include (adapt if needed)
         - 4. In the select liste "Show directories for", choose "library files", and add C:\\Program Files\\Microsoft Platform SDK\\Lib
            (adapt if needed) Edit file C:\\Program Files\\Microsoft Visual Studio 8\\VC\\VCProjectDefaults\\corewin_express.vsprops (adapt if needed)
         - 6. 7. Remplace the line AdditionalDependencies="kernel32.lib" /> by AdditionalDependencies="kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib" />
   - Restart Visual C++
   - Import CImg library in your main file

**/
/*@}*/


/*-----------------------------------

   Tutorial : Getting started

  -------------------------------------*/

/** \addtogroup cimg_tutorial Tutorial : Getting Started. */
/*@{*/
/**
  \page foo3

  Let's start to write our first program to get the idea. This will demonstrate how to load and create images, as well as handle image
  display and mouse events.
  Assume we want to load a color image <tt>lena.jpg</tt>, smooth it, display it in a windows, and enter an event loop so that clicking a
  point in the image will draw the (R,G,B) intensity profiles of the corresponding image line (in another window).
  Yes, that sounds quite complex for a first code, but don't worry, it will be very simple using the CImg library ! Well, just look
  at the code below, it does the task :

  \code
  #include "CImg.h"
  using namespace cimg_library;

  int main() {
    CImg<unsigned char> image("lena.jpg"), visu(500,400,1,3,0);
    const unsigned char red[] = { 255,0,0 }, green[] = { 0,255,0 }, blue[] = { 0,0,255 };
    image.blur(2.5);
    CImgDisplay main_disp(image,"Click a point"), draw_disp(visu,"Intensity profile");
    while (!main_disp.is_closed() && !draw_disp.is_closed()) {
      main_disp.wait();
      if (main_disp.button() && main_disp.mouse_y()>=0) {
        const int y = main_disp.mouse_y();
        visu.fill(0).draw_graph(image.get_crop(0,y,0,0,image.width()-1,y,0,0),red,1,1,0,255,0);
        visu.draw_graph(image.get_crop(0,y,0,1,image.width()-1,y,0,1),green,1,1,0,255,0);
        visu.draw_graph(image.get_crop(0,y,0,2,image.width()-1,y,0,2),blue,1,1,0,255,0).display(draw_disp);
        }
      }
    return 0;
  }
  \endcode

  Here is a screenshot of the resulting program :

  <img SRC="../img/tutorial.jpg">

  And here is the detailled explanation of the source, line by line :

  \code #include "CImg.h" \endcode
  Include the main and only header file of the CImg library.
  \code using namespace cimg_library; \endcode
  Use the library namespace to ease the declarations afterward.
  \code int main() { \endcode
  Definition of the main function.
  \code CImg<unsigned char> image("lena.jpg"), visu(500,400,1,3,0); \endcode
  Creation of two instances of images of \c unsigned \c char pixels.
  The first image \c image is initialized by reading an image file from the disk.
  Here, <tt>lena.jpg</tt> must be in the same directory than the current program.
  Note that you must also have installed the \e ImageMagick package in order to be able to read JPG images.
  The second image \c visu is initialized as a black color image with dimension <tt>dx=500</tt>, <tt>dy=400</tt>,
  <tt>dz=1</tt> (here, it is a 2D image, not a 3D one), and <tt>dv=3</tt> (each pixel has 3 'vector' channels R,G,B).
  The last argument in the constructor defines the default value of the pixel values
  (here \c 0, which means that \c visu will be initially black).
  \code const unsigned char red[] = { 255,0,0 }, green[] = { 0,255,0 }, blue[] = { 0,0,255 }; \endcode
  Definition of three different colors as array of unsigned char. This will be used to draw plots with different colors.
  \code image.blur(2.5); \endcode
  Blur the image, with a gaussian blur and a standard variation of 2.5. Note that most of the CImg functions have two versions :
  one that acts in-place (which is the case of blur), and one that returns the result as a new image (the name of the function
  begins then with <tt>get_</tt>&nbsp;). In this case, one could have also written <tt>image = image.get_blur(2.5);</tt>
  (more expensive, since it needs an additional copy operation).
  \code CImgDisplay main_disp(image,"Click a point"), draw_disp(visu,"Intensity profile"); \endcode
  Creation of two display windows, one for the input image image, and one for the image visu which will be display intensity profiles.
  By default, CImg displays handles events (mouse,keyboard,..). On Windows, there is a way to create fullscreen displays.
  \code while (!main_disp.is_closed() && !draw_disp.is_closed()) { \endcode
  Enter the event loop, the code will exit when one of the two display windows is closed.
  \code main_disp.wait(); \endcode
  Wait for an event (mouse, keyboard,..) in the display window \c main_disp.
  \code if (main_disp.button() && main_disp.mouse_y()>=0) { \endcode
  Test if the mouse button has been clicked on the image area.
  One may distinguish between the 3 different mouse buttons,
  but in this case it is not necessary
  \code const int y = main_disp.mouse_y(); \endcode
  Get the image line y-coordinate that has been clicked.
  \code visu.fill(0).draw_graph(image.get_crop(0,y,0,0,image.width()-1,y,0,0),red,1,0,256,0); \endcode
  This line illustrates the pipeline property of most of the CImg class functions. The first function <tt>fill(0)</tt> simply sets
  all pixel values with 0 (i.e. clear the image \c visu). The interesting thing is that it returns a reference to
  \c visu and then, can be pipelined with the function \c draw_graph() which draws a plot in the image \c visu.
  The plot data are given by another image (the first argument of \c draw_graph()). In this case, the given image is
  the red-component of the line y of the original image, retrieved by the function \c get_crop() which returns a
  sub-image of the image \c image. Remember that images coordinates are 4D (x,y,z,v) and for color images,
  the R,G,B channels are respectively given by <tt>v=0, v=1</tt> and <tt>v=2</tt>.
  \code visu.draw_graph(image.get_crop(0,y,0,1,image.width()-1,y,0,1),green,1,0,256,0); \endcode
  Plot the intensity profile for the green channel of the clicked line.
  \code visu.draw_graph(image.get_crop(0,y,0,2,image.width()-1,y,0,2),blue,1,0,256,0).display(draw_disp); \endcode
  Same thing for the blue channel. Note how the function (which return a reference to \c visu) is pipelined with the function
  \c display() that just paints the image visu in the corresponding display window.
  \code ...till the end \endcode
  I don't think you need more explanations !

  As you have noticed, the CImg library allows to write very small and intuitive code. Note also that this source will perfectly
  work on Unix and Windows systems. Take also a look to the examples provided in the CImg package (
  directory \c examples/ ). It will show you how CImg-based code can be surprisingly small.
  Moreover, there is surely one example close to what you want to do.
  A good start will be to look at the file <tt>CImg_demo.cpp</tt> which contains small and various examples of what you can do
  with the %CImg Library. All CImg classes are used in this source, and the code can be easily modified to see what happens.

**/
/*@}*/

/*-----------------------------------

   Using drawing functions

  -------------------------------------*/

/** \addtogroup cimg_drawing Using Drawing Functions. */
/*@{*/
/**
  \page foo5

  \section s5 Using Drawing Functions.

  This section tells more about drawing features in CImg images.
  Drawing functions list can be found in <a href="structCImg.html">the CImg functions list</a>
  (section \b Drawing Functions),
  and are all defined on a common basis. Here are the important points to understand before using
  drawing functions :

  - Drawing is performed on the instance image. Drawing functions parameters
  are defined as \e const variables and return a reference to the current instance <tt>(*this)</tt>,
  so that drawing functions can be pipelined (see examples below).
  Drawing is usually done in 2D color images but can be performed in 3D images with any vector-valued dimension,
  and with any possible pixel type.

  - A color parameter is always needed to draw features in an image. The color must be defined as a C-style array
  whose dimension is at least

**/
/*@}*/

/*-----------------------------------

   Using image loops

  -------------------------------------*/

/** \addtogroup cimg_loops Using Image Loops. */
/*@{*/
/**
  \page foo_lo
  The %CImg Library provides different macros that define useful iterative loops over an image.
  Basically, it can be used to replace one or several <tt>for(..)</tt> instructions, but it also proposes
  interesting extensions to classical loops.
  Below is a list of all existing loop macros, classified in four different categories :
  - \ref lo1
  - \ref lo4
  - \ref lo5
  - \ref lo6

  \section lo1 Loops over the pixel buffer

  Loops over the pixel buffer are really basic loops that iterate a pointer on the pixel data buffer
  of a \c cimg_library::CImg image. Two macros are defined for this purpose :

  - \b cimg_for(img,ptr,T) :
  This macro loops over the pixel data buffer of the image \c img, using a pointer <tt>T* ptr</tt>,
  starting from the beginning of the buffer (first pixel) till the end of the buffer (first pixel).
      - \c img must be a (non empty) \c cimg_library::CImg image of pixels \c T.
      - \c ptr is a pointer of type \c T*.
  This kind of loop should not appear a lot in your own source code, since this is a low-level loop
  and many functions of the CImg class may be used instead. Here is an example of use :
  \code
  CImg<float> img(320,200);
  cimg_for(img,ptr,float) { *ptr=0; }      // Equivalent to 'img.fill(0);'
  \endcode

  - \b cimg_rof(img,ptr,T) :
  This macro does the same as \c cimg_for() but from the end to the beginning of the pixel buffer.

  - \b cimg_foroff(img,off) :
  This macro loops over the pixel data buffer of the image \c img, using an offset \c ,
  starting from the beginning of the buffer (first pixel, \c off=0)
  till the end of the buffer (last pixel value, <tt>off = img.size()-1</tt>).
      - \c img must be a (non empty) cimg_library::CImg<T> image of pixels \c T.
      - \c off is an inner-loop variable, only defined inside the scope of the loop.

  Here is an example of use :
  \code
  CImg<float> img(320,200);
  cimg_foroff(img,off) { img[off]=0; }  // Equivalent to 'img.fill(0);'
  \endcode

  \section lo4 Loops over image dimensions

  The following loops are probably the most used loops in image processing programs.
  They allow to loop over the image along one or several dimensions, along a raster scan course.
  Here is the list of such loop macros for a single dimension :
  - \b cimg_forX(img,x) : equivalent to : <tt>for (int x = 0; x<img.width(); x++)</tt>.
  - \b cimg_forY(img,y) : equivalent to : <tt>for (int y = 0; y<img.height(); y++)</tt>.
  - \b cimg_forZ(img,z) : equivalent to : <tt>for (int z = 0; z<img.depth(); z++)</tt>.
  - \b cimg_forC(img,v) : equivalent to : <tt>for (int v = 0; v<img.spectrum(); v++)</tt>.

  Combinations of these macros are also defined as other loop macros, allowing to loop directly over 2D, 3D or 4D images :
  - \b cimg_forXY(img,x,y) : equivalent to : \c cimg_forY(img,y) \c cimg_forX(img,x).
  - \b cimg_forXZ(img,x,z) : equivalent to : \c cimg_forZ(img,z) \c cimg_forX(img,x).
  - \b cimg_forYZ(img,y,z) : equivalent to : \c cimg_forZ(img,z) \c cimg_forY(img,y).
  - \b cimg_forXC(img,x,v) : equivalent to : \c cimg_forC(img,v) \c cimg_forX(img,x).
  - \b cimg_forYC(img,y,v) : equivalent to : \c cimg_forC(img,v) \c cimg_forY(img,y).
  - \b cimg_forZC(img,z,v) : equivalent to : \c cimg_forC(img,v) \c cimg_forZ(img,z).
  - \b cimg_forXYZ(img,x,y,z) : equivalent to : \c cimg_forZ(img,z) \c cimg_forXY(img,x,y).
  - \b cimg_forXYC(img,x,y,v) : equivalent to : \c cimg_forC(img,v) \c cimg_forXY(img,x,y).
  - \b cimg_forXZC(img,x,z,v) : equivalent to : \c cimg_forC(img,v) \c cimg_forXZ(img,x,z).
  - \b cimg_forYZC(img,y,z,v) : equivalent to : \c cimg_forC(img,v) \c cimg_forYZ(img,y,z).
  - \b cimg_forXYZC(img,x,y,z,v) : equivalent to : \c cimg_forC(img,v) \c cimg_forXYZ(img,x,y,z).

  - For all these loops, \c x,\c y,\c z and \c v are inner-defined variables only visible inside the scope of the loop.
  They don't have to be defined before the call of the macro.
  - \c img must be a (non empty) cimg_library::CImg image.

  Here is an example of use that creates an image with a smooth color gradient :
  \code
  CImg<unsigned char> img(256,256,1,3);       // Define a 256x256 color image
  cimg_forXYC(img,x,y,v) { img(x,y,v) = (x+y)*(v+1)/6; }
  img.display("Color gradient");
  \endcode

  \section lo5 Loops over interior regions and borders.

  Similar macros are also defined to loop only on the border of an image, or inside the image (excluding the border).
  The border may be several pixel wide :

  - \b cimg_for_insideX(img,x,n) : Loop along the x-axis, except for pixels inside a border of \p n pixels wide.
  - \b cimg_for_insideY(img,y,n) : Loop along the y-axis, except for pixels inside a border of \p n pixels wide.
  - \b cimg_for_insideZ(img,z,n) : Loop along the z-axis, except for pixels inside a border of \p n pixels wide.
  - \b cimg_for_insideC(img,v,n) : Loop along the v-axis, except for pixels inside a border of \p n pixels wide.
  - \b cimg_for_insideXY(img,x,y,n) : Loop along the (x,y)-axes, excepted for pixels inside a border of \p n pixels wide.
  - \b cimg_for_insideXYZ(img,x,y,z,n) : Loop along the (x,y,z)-axes, excepted for pixels inside a border of \p n pixels wide.

  And also :

  - \b cimg_for_borderX(img,x,n) : Loop along the x-axis, only for pixels inside a border of \p n pixels wide.
  - \b cimg_for_borderY(img,y,n) : Loop along the y-axis, only for pixels inside a border of \p n pixels wide.
  - \b cimg_for_borderZ(img,z,n) : Loop along the z-axis, only for pixels inside a border of \p n pixels wide.
  - \b cimg_for_borderC(img,v,n) : Loop along the z-axis, only for pixels inside a border of \p n pixels wide.
  - \b cimg_for_borderXY(img,x,y,n) : Loop along the (x,y)-axes, only for pixels inside a border of \p n pixels wide.
  - \b cimg_for_borderXYZ(img,x,y,z,n) : Loop along the (x,y,z)-axes, only for pixels inside a border of \p n pixels wide.

  - For all these loops, \c x,\c y,\c z and \c v are inner-defined variables only visible inside the scope of the loop.
  They don't have to be defined before the call of the macro.
  - \c img must be a (non empty) cimg_library::CImg image.
  - The constant \c n stands for the size of the border.

  Here is an example of use, to create a 2d grayscale image with two different intensity gradients :
  \code
  CImg<> img(256,256);
  cimg_for_insideXY(img,x,y,50) img(x,y) = x+y;
  cimg_for_borderXY(img,x,y,50) img(x,y) = x-y;
  img.display();
  \endcode

  \section lo6 Loops using neighborhoods.

  Inside an image loop, it is often useful to get values of neighborhood pixels of the
  current pixel at the loop location.
  The %CImg Library provides a very smart and fast mechanism for this purpose, with the definition
  of several loop macros that remember the neighborhood values of the pixels.
  The use of these macros can highly optimize your code, and also simplify your program.

  \subsection lo7 Neighborhood-based loops for 2D images

  For 2D images, the neighborhood-based loop macros are :

  - \b cimg_for2x2(img,x,y,z,v,I,T) : Loop along the (x,y)-axes using a centered 2x2 neighborhood.
  - \b cimg_for3x3(img,x,y,z,v,I,T) : Loop along the (x,y)-axes using a centered 3x3 neighborhood.
  - \b cimg_for4x4(img,x,y,z,v,I,T) : Loop along the (x,y)-axes using a centered 4x4 neighborhood.
  - \b cimg_for5x5(img,x,y,z,v,I,T) : Loop along the (x,y)-axes using a centered 5x5 neighborhood.

  For all these loops, \c x and \c y are inner-defined variables only visible inside the scope of the loop.
  They don't have to be defined before the call of the macro.
  \c img is a non empty CImg<T> image. \c z and \c v are constants that define on which image slice and
  vector channel the loop must apply (usually both 0 for grayscale 2D images).
  Finally, \c I is the 2x2, 3x3, 4x4 or 5x5 neighborhood of type \c T that will be updated with the correct pixel values
  during the loop (see \ref lo9).

  \subsection lo8 Neighborhood-based loops for 3D images

  For 3D images, the neighborhood-based loop macros are :

  - \b cimg_for2x2x2(img,x,y,z,v,I,T) : Loop along the (x,y,z)-axes using a centered 2x2x2 neighborhood.
  - \b cimg_for3x3x3(img,x,y,z,v,I,T) : Loop along the (x,y,z)-axes using a centered 3x3x3 neighborhood.

  For all these loops, \c x, \c y and \c z are inner-defined variables only visible inside the scope of the loop.
  They don't have to be defined before the call of the macro.
  \c img is a non empty CImg<T> image. \c v is a constant that defines on which image channel
  the loop must apply (usually 0 for grayscale 3D images).
  Finally, \c I is the 2x2x2 or 3x3x3 neighborhood of type \c T that will be updated with the correct pixel values
  during the loop (see \ref lo9).

  \subsection lo9 Defining neighborhoods

  A neighborhood is defined as an instance of a class having operator[] defined.
  This particularly includes classical C-array, as well as CImg<T> objects.

  For instance, a 3x3 neighborhood can be defined either as a 'float[9]' or a
  'CImg<float>(3,3)' variable.

  \subsection lo10 Using alternate variable names

  There are also some useful macros that can be used to define variables that
  reference the neighborhood elements. There are :

  - \b CImg_2x2(I,type) : Define a 2x2 neighborhood named \c I, of type \c type.
  - \b CImg_3x3(I,type) : Define a 3x3 neighborhood named \c I, of type \c type.
  - \b CImg_4x4(I,type) : Define a 4x4 neighborhood named \c I, of type \c type.
  - \b CImg_5x5(I,type) : Define a 5x5 neighborhood named \c I, of type \c type.
  - \b CImg_2x2x2(I,type) : Define a 2x2x2 neighborhood named \c I, of type \c type.
  - \b CImg_3x3x3(I,type) : Define a 3x3x3 neighborhood named \c I, of type \c type.

  Actually, \c I is a \e generic \e name for the neighborhood. In fact, these macros declare
  a \e set of new variables.
  For instance, defining a 3x3 neighborhood \c CImg_3x3(I,float) declares 9 different float variables
  \c Ipp,\c Icp,\c Inp,\c Ipc,\c Icc,\c Inc,\c Ipn,\c Icn,\c Inn which correspond to each pixel value of
  a 3x3 neighborhood.
  Variable indices are \c p,\c c or \c n, and stand respectively for \e 'previous', \e 'current' and \e 'next'.
  First indice denotes the \c x-axis, second indice denotes the \c y-axis.
  Then, the names of the variables are directly related to the position of the corresponding pixels
  in the neighborhood. For 3D neighborhoods, a third indice denotes the \c z-axis.
  Then, inside a neighborhood loop, you will have the following equivalence :
  - <tt>Ipp = img(x-1,y-1)</tt>
  - <tt>Icn = img(x,y+1)</tt>
  - <tt>Inp = img(x+1,y-1)</tt>
  - <tt>Inpc = img(x+1,y-1,z)</tt>
  - <tt>Ippn = img(x-1,y-1,z+1)</tt>
  - and so on...

  For bigger neighborhoods, such as 4x4 or 5x5 neighborhoods, two additionnal indices are introduced :
  \c a (stands for \e 'after') and \c b (stands for \e 'before'), so that :
  - <tt>Ibb = img(x-2,y-2)</tt>
  - <tt>Ina = img(x+1,y+2)</tt>
  - and so on...

  The value of a neighborhood pixel outside the image range (image border problem) is automatically set to the same
  values than the nearest valid pixel in the image (this is also called the \e Neumann \e border \e condition).

  \subsection lo11 Example codes
  More than a long discussion, the above example will demonstrate how to compute the gradient norm of a 3D volume
  using the \c cimg_for3x3x3() loop macro :

  \code
  CImg<float> volume("IRM.hdr");          // Load an IRM volume from an Analyze7.5 file
  CImg_3x3x3(I,float);                    // Define a 3x3x3 neighborhood
  CImg<float> gradnorm(volume);           // Create an image with same size as 'volume'
  cimg_for3x3x3(volume,x,y,z,0,I,float) { // Loop over the volume, using the neighborhood I
    const float ix = 0.5f*(Incc-Ipcc);    // Compute the derivative along the x-axis.
    const float iy = 0.5f*(Icnc-Icpc);    // Compute the derivative along the y-axis.
    const float iz = 0.5f*(Iccn-Iccp);    // Compute the derivative along the z-axis.
    gradnorm(x,y,z) = std::sqrt(ix*ix+iy*iy+iz*iz);  // Set the gradient norm in the destination image
  }
  gradnorm.display("Gradient norm");
  \endcode

  And the following example shows how to deal with neighborhood references to blur a color image by averaging
  pixel values on a 5x5 neighborhood.

  \code
  CImg<unsigned char> src("image_color.jpg"), dest(src,false), neighbor(5,5);  // Image definitions.
  typedef unsigned char uchar;             // Avoid space in the second parameter of the macro CImg_5x5x1 below.
  CImg<> N(5,5);                           // Define a 5x5 neighborhood as a 5x5 image.
  cimg_forC(src,k)                         // Standard loop on color channels
     cimg_for5x5(src,x,y,0,k,N,float)      // 5x5 neighborhood loop.
       dest(x,y,k) = N.sum()/(5*5);        // Averaging pixels to filter the color image.
  CImgList<unsigned char> visu(src,dest);
  visu.display("Original + Filtered");     // Display both original and filtered image.
  \endcode

  As you can see, explaining the use of the CImg neighborhood macros is actually more difficult than using them !

**/
/*@}*/

/*-----------------------------------

   Using display windows

  -------------------------------------*/

/** \addtogroup cimg_displays Using Display Windows. */
/*@{*/
/**
  \page foo_di

  When opening a display window, you can choose the way the pixel values will be normalized
  before being displayed on the screen. Screen displays only support color values between [0,255],
  and some

  When displaying an image into the display window using CImgDisplay::display(), values of
  the image pixels can be eventually linearly normalized between [0,255] for visualization purposes.
  This may be useful for instance when displaying \p CImg<double> images with pixel values
  between [0,1].
  The normalization behavior depends on the value of \p normalize which can be either \p 0,\p 1 or \p 2 :
  - \p 0 : No pixel normalization is performed when displaying an image. This is the fastest
  process, but you must be sure your displayed image have pixel values inside the range [0,255].
  - \p 1 : Pixel value normalization is done for each new image display. Image pixels are
  not modified themselves, only displayed pixels are normalized.
  - \p 2 : Pixel value normalization is done for the first image display, then the
  normalization parameters are kept and used for all the next image displays.

**/
/*@}*/

/*-----------------------------------

  How pixel data are stored

  -------------------------------------*/

/** \addtogroup cimg_storage How pixel data are stored with CImg. */
/*@{*/
/**
  \page foo_store

  First, CImg<T> are *very* basic structures, which means that there are no memory tricks, weird memory alignments or
  disk caches used to store pixel data of images. When an image is instanced, all its pixel values are stored in memory at
  the same time (yes, you should avoid working with huge images when dealing with CImg, if you have only 64kb of RAM).

  A CImg<T> is basically a 4th-dimensional array (width,height,depth,dim), and its pixel data are stored linearly in a single
  memory buffer of general size (width*height*depth*dim). Nothing more, nothing less. The address of this memory buffer can be
  retrieved by the function CImg<T>::data().
  As each image value is stored as a type T (T being known by the programmer of course), this pointer is a 'T*', or a 'const T*' if your image is 'const'.
  so, 'T *ptr = img.data()' gives you the pointer to the first value of the image 'img'. The overall size of the used memory for one
  instance image (in bytes) is then 'width*height*depth*dim*sizeof(T)'.

  Now, the ordering of the pixel values in this buffer follows these rules :
  The values are *not* interleaved, and are ordered first along the X,Y,Z and V axis respectively (corresponding to the width,height,depth,dim dimensions),
  starting from the upper-left pixel to the bottom-right pixel of the instane image, with a classical scanline run.

  So, a color image with dim=3 and depth=1, will be stored in memory as :

  R1R2R3R4R5R6......G1G2G3G4G5G6.......B1B2B3B4B5B6.... (i.e following a 'planar' structure)

  and *not* as R1G1B1R2G2B2R3G3B3... (interleaved channels),
  where R1 = img(0,0,0,0) is the first upper-left pixel of the red component of the image,
  R2 is img(1,0,0,0), G1 = img(0,0,0,1), G2 = img(1,0,0,1), B1 = img(0,0,0,2), and so on...

  Another example, a (1x5x1x1) CImg<T> (column vector A) will be stored as : A1A2A3A4A5
  where A1 = img(0,0), A2 = img(0,1), ... , A5 = img(0,4).

  As you see, it is *very* simple and intuitive : no interleaving, no padding, just simple.
  This is cool not only because it is simple, but this has in fact a number of interesting properties. For instance, a 2D color image
  is stored in memory exactly as a 3D scalar image having a depth=3, meaning that when you are dealing with 2D color images, you can write 'img(x,y,k)'
  instead of 'img(x,y,0,k)' to access the kth channel of the (x,y) pixel. More generally, if you have one dimension that is 1 in
  your image, you can just skip it in the call to the operator(). Similarly, values of a column vector stored as an image with
  width=depth=spectrum=1 can be accessed by 'img(y)' instead of 'img(0,y)'. This is very convenient.

  Another cool thing is that it allows you to work easily with 'shared' images. A shared image is a CImg<T> instance that shares
  its memory with another one (the 'base' image). Destroying a shared image does nothing in fact. Shared images is a convenient
  way of modifying only *portions* (consecutive in memory) of an image. For instance, if 'img' is a 2D color image, you can write :

  img.get_shared_channel(0).blur(2);
  img.get_shared_channels(1,2).mirror('x');

  which just blur the red channel of the image, and mirror the two others along the X-axis.
  This is possible since channels of an image are not interleaved but are stored as different consecutive planes in memory, so you see that constructing a shared image is possible (and trivial).

**/
/*@}*/

/*-----------------------------------

  Files IO

  -------------------------------------*/

/** \addtogroup cimg_files_io Files IO in CImg. */
/*@{*/
/**
  \page foo_fi

  The %CImg Library can NATIVELY handle the following file formats :
  - RAW : consists in a very simple header (in ascii), then the image data.
  - ASC (Ascii)
  - HDR (Analyze 7.5)
  - INR (Inrimage)
  - PPM/PGM (Portable Pixmap)
  - BMP (uncompressed)
  - PAN (Pandore-5)
  - DLM (Matlab ASCII)

  If ImageMagick is installed, The %CImg Library can save image in formats handled by ImageMagick : JPG, GIF, PNG, TIF,...

**/
/*@}*/

/*-----------------------------------

   Retrieving command line arguments

  -------------------------------------*/

/** \addtogroup cimg_options Retrieving Command Line Arguments. */
/*@{*/
/**
  \page foo_so

   The CImg library offers facilities to retrieve command line arguments in a console-based
   program, as it is a commonly needed operation.
   Three macros \c cimg_usage(), \c cimg_help() and \c cimg_option() are defined for this purpose.
   Using these macros allows to easily retrieve options values from the command line.
   Invoking the compiled executable with the option \c -h or \c --help will
   automatically display the program usage, followed by the list of requested options.

   \section so1 The cimg_usage() macro

   The macro \c cimg_usage(usage) may be used to describe the program goal and usage.
   It is generally inserted one time after the <tt>int main(int argc,char **argv)</tt> definition.

   \param usage : A string describing the program goal and usage.
   \pre The function where \c cimg_usage() is used must have correctly defined \c argc and \c argv variables.

   \section so1_5 The cimg_help() macro

   The macro \c cimg_help(str) will display the string \c str only if the \c -help or \c --help option
   are invoked when running the programm.

   \section so2 The cimg_option() macro

   The macro \c cimg_option(name,default,usage) may be used to retrieve an option value from the command line.

   \param name    : The name of the option to be retrieved from the command line.
   \param default : The default value returned by the macro if no options \p name has been specified when running the program.
   \param usage   : A brief explanation of the option. If \c usage==0, the option won't appear on the option list
                    when invoking the executable with options \c -h or \c --help (hidden option).

   \return \c cimg_option() returns an object that has the \e same \e type than the default value \c default.
   The return value is equal to the one specified on the command line. If no such option have been specified,
   the return value is equal to the default value \c default.
   Warning, this can be confusing in some situations (look at the end of the next section).
   \pre The function where \c cimg_option() is used must have correctly defined \c argc and \c argv variables.

   \section so3 Example of use

   The code below uses the macros \c cimg_usage() and \c cimg_option().
   It loads an image, smoothes it an quantifies it with a specified number of values.
   \code
   #include "CImg.h"
   using namespace cimg_library;
   int main(int argc,char **argv) {
     cimg_usage("Retrieve command line arguments");
     const char* filename = cimg_option("-i","image.gif","Input image file");
     const char* output   = cimg_option("-o",(char*)0,"Output image file");
     const double sigma   = cimg_option("-s",1.0,"Standard variation of the gaussian smoothing");
     const  int nblevels  = cimg_option("-n",16,"Number of quantification levels");
     const bool hidden    = cimg_option("-hidden",false,0);      // This is a hidden option

     CImg<unsigned char> img(filename);
     img.blur(sigma).quantize(nblevels);
     if (output) img.save(output); else img.display("Output image");
     if (hidden) std::fprintf(stderr,"You found me !\n");
     return 0;
   }
   \endcode

   Invoking the corresponding executable with <tt>test -h -hidden -n 20 -i foo.jpg</tt> will display :
   \verbatim
   ./test -h -hidden -n 20 -i foo.jpg

 test : Retrieve command line arguments (Oct 16 2004, 12:34:26)

    -i       = foo.jpg      : Input image file
    -o       = 0            : Output image file
    -s       = 1            : Standard variation of the gaussian smoothing
    -n       = 20           : Number of quantification levels

   You found me !
\endverbatim

   \warning As the type of object returned by the macro \c cimg_option(option,default,usage)
   is defined by the type of \c default, undesired casts may appear when writting code such as :
   \code
   const double sigma = cimg_option("-val",0,"A floating point value");
   \endcode
   In this case, \c sigma will always be equal to an integer (since the default value \c 0 is an integer).
   When passing a float value on the command line, a \e float \e to \e integer cast is then done,
   truncating the given parameter to an integer value (this is surely not a desired behavior).
   You must specify <tt>0.0</tt> as the default value in this case.

   \section so4 How to learn more about command line options ?
   You should take a look at the examples <tt>examples/gmic.cpp</tt> provided in the %CImg Library package.
   This is a command line based image converter which intensively uses the \c cimg_option() and \c cimg_usage()
   macros to retrieve command line parameters.
**/
/*@}*/

