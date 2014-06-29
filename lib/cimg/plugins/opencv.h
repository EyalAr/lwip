/*
 #
 #  File        : opencv.h
 #                ( C++ header file - CImg plug-in )
 #
 #  Description : CImg plugin that provides OpenCV-based function to perform various stuffs.
 #
 #  IMPORTANT WARNING : You must include STL's <map> and <string> before plugin inclusion to make it working !
 #
 #  Copyright   : Antonio Albiol
 #                ( http://personales.upv.es/~aalbiol/ )
 #
 #  This software is governed by the CeCILL license under French law and
 #  abiding by the rules of distribution of free software. You can use,
 #  modify and/or redistribute the software under the terms of the CeCILL
 #  license as circulated by CEA, CNRS and INRIA at the following URL
 #  "http://www.cecill.info".
 #
 #  As a counterpart to the access to the source code and rights to copy,
 #  modify and redistribute granted by the license, users are provided only
 #  with a limited warranty and the software's author, the holder of the
 #  economic rights, and the successive licensors have only limited
 #  liability.
 #
 #  In this respect, the user's attention is drawn to the risks associated
 #  with loading, using, modifying and/or developing or reproducing the
 #  software by the user in light of its specific status of free software,
 #  that may mean that it is complicated to manipulate, and that also
 #  therefore means that it is reserved for developers and experienced
 #  professionals having in-depth computer knowledge. Users are therefore
 #  encouraged to load and test the software's suitability as regards their
 #  requirements in conditions enabling the security of their systems and/or
 #  data to be ensured and, more generally, to use and operate it in the
 #  same conditions as regards security.
 #
 #  The fact that you are presently reading this means that you have had
 #  knowledge of the CeCILL license and that you accept its terms.
 #
*/
#ifndef cimg_plugin_opencv
#define cimg_plugin_opencv

//! Load image from a video file, using OpenCV.
/**
   \param filename
   \param skip_frames Number of frames to skip before the capture.
   \param release_video Request to close the video.
   \return an empty image if error or the end of file is reached
**/
CImg<T>& load_video(const std::string& filename,  const unsigned int skip_frames=0, const bool release_video=false) {
#ifdef cimg_use_opencv

  static std::map<std::string , cv::VideoCapture * > namesTable;

  cv::VideoCapture * capture;
  int c = namesTable.count( filename );
  if (! c ) { //File not open. Create Capture object to start using it
    capture = new cv::VideoCapture (filename);
    if (capture->isOpened())
      namesTable.insert( std::pair< std::string , cv::VideoCapture * > (filename, capture));
    else {//Invalid video filename
      throw CImgIOException(_cimg_instance
                            "load_video(): Failed to initialize video %s.",
                            cimg_instance,
                            filename.c_str() );
      return *this;
    }
  }
  else
    capture = namesTable.find(filename)->second;

  //From here, capture points to a valid capture

  if (release_video) {
    if (capture) delete capture;
    namesTable.erase(filename);
    return *this;
  }

  cv::Mat img;
  for (unsigned int i = 0; i<skip_frames; ++i) (*capture) >> img;
  (*capture) >> img;
  if (!img.empty() ) {
    const int step = (int)(img.step[0] - 3 * img.cols);
    assign(img.cols,img.rows,1,3);
    const unsigned char * ptrs = img.data;
    T *ptr_r = data(0,0,0,0), *ptr_g = data(0,0,0,1), *ptr_b = data(0,0,0,2);
    if (step>0) cimg_forY(*this,y) {
        cimg_forX(*this,x) { *(ptr_b++) = (T)*(ptrs++); *(ptr_g++) = (T)*(ptrs++); *(ptr_r++) = (T)*(ptrs++); }
        ptrs+=step;
      } else for (unsigned long siz = (unsigned long)img.cols*img.rows; siz; --siz) {
        *(ptr_b++) = (T)*(ptrs++); *(ptr_g++) = (T)*(ptrs++); *(ptr_r++) = (T)*(ptrs++);
      }
  }
  else {
    assign(); //Return an empty image to indicate EOF
  }
#else
  throw CImgIOException(_cimg_instance
                        "load_video(): This function requires the OpenCV library to run "
                        "(macro 'cimg_use_opencv' must be defined).",
                        cimg_instance);
#endif
  return *this;
}

//! Load image from a camera stream, using OpenCV \newinstance.
static CImg<T> get_load_video(const std::string& filename,  const unsigned int skip_frames=0, const bool release_video=false) {
  return CImg<T>().load_video(filename,skip_frames,release_video);
}

#endif
