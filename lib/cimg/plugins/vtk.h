/*
 #
 #  File        : vtk.h
 #                ( C++ header file - CImg plug-in )
 #
 #  Description : CImg plugin that implements a way to save 3d scene as TK legacy file format.
 #                This file is a part of the CImg Library project.
 #                ( http://cimg.sourceforge.net )
 #
 #  Copyright   : Haz-Edine Assemlal
 #                ( http://www.cim.mcgill.ca/~assemlal/ )
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

#ifndef cimg_plugin_vtk
#define cimg_plugin_vtk

// Save 3D scene in legacy VTK format
/* *this: CImgList of points
 * faces: CImgList of faces
 * colors: CImgList of colors,
 * opacities: CImgList of opacities
 */
template<typename tf, typename tc, typename to>
  CImgList<T>& save_vtk(const char* const filename,
                        const CImgList<tf>& faces,
                        const CImgList<tc>& colors,
                        const CImgList<to<& opacities) {
  // Open file
  std::FILE *const nfile = cimg::fopen(filename,"w");

  // Header
  std::fprintf(nfile,"# vtk DataFile Version 3.0\n");
  std::fprintf(nfile,"%s\n",filename);
  std::fprintf(nfile,"ASCII\n");
  std::fprintf(nfile,"DATASET UNSTRUCTURED_GRID\n");

  // Points
  std::fprintf(nfile,"POINTS %u float\n",points.size());
  cimglist_for(points,p)
    std::fprintf(nfile,"%f %f %f\n",points[p](0),points[p](1),points[p](2));
  std::fprintf(nfile,"\n");

  // Faces (valid only for triangles - type 5)
  if (faces) {
    std::fprintf(nfile,"CELLS %u %u\n",faces.size(),faces.size()*4);
    cimglist_for(faces,f)
      std::fprintf(nfile,"%d %u %u %u\n",3,faces[f](0),faces[f](1),faces[f](2));
    std::fprintf(nfile,"CELL_TYPES %u\n",faces.size());
    cimglist_for(faces,f)
      std::fprintf(nfile,"%d\n",5);
    std::fprintf(nfile,"\n");
  }

  // Colors and Opacities
  std::fprintf(nfile,"CELL_DATA %d\n",colors.size());
  std::fprintf(nfile,"COLOR_SCALARS colors 4\n");

  const tc tcmax = cimg::type<tc>::max();
  cimglist_for(colors,t)
    std::fprintf(nfile,"%f %f %f %f\n",
                 (float)colors[t](0)/tcmax,
                 (float)colors[t](1)/tcmax,
                 (float)colors[t](2)/tcmax,
                 opacities[t](0));
  std::fprintf(nfile,"\n");

  // Close file
  cimg::fclose(nfile);

  return *this;
}

#endif
