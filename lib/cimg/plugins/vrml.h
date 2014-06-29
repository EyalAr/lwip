/*
 #
 #  File        : vrml.h
 #                ( C++ header file - CImg plug-in )
 #
 #  Description : CImg plugin that provide functions to load/save VRML files.
 #                This file is a part of the CImg Library project.
 #                ( http://cimg.sourceforge.net )
 #
 #  Copyright   : Greg Rami
 #                ( greg.rami36 (at) gmail.com )
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

 #include <vector>
 #include <string>
 #include <sstream>
 #include <algorithm>

------------------------------------------------------------------------------------*/

#ifndef cimg_plugin_vrml
#define cimg_plugin_vrml

//! Load a 3d object from a .VRML file.
template<typename tf, typename tc>
CImg<T>& load_vrml(const char *const filename, CImgList<tf>& primitives, CImgList<tc>& colors) {
  return _load_vrml(0,filename,primitives,colors);
}

//! Load a 3d object from a .VRML file.
template<typename tf, typename tc>
static CImg<T> get_load_vrml(const char *const filename, CImgList<tf>& primitives, CImgList<tc>& colors) {
  return CImg<T>().load_vrml(filename,primitives,colors);
}

//! Load a 3d object from a .VRML file.
template<typename tf, typename tc>
CImg<T>& load_vrml(std::FILE *const file, CImgList<tf>& primitives, CImgList<tc>& colors) {
  return _load_vrml(file,0,primitives,colors);
}

//! Load a 3d object from a .VRML file.
template<typename tf, typename tc>
static CImg<T> get_load_vrml(std::FILE *const file, CImgList<tf>& primitives, CImgList<tc>& colors) {
  return CImg<T>().load_vrml(file,primitives,colors);
}

//! Load a 3d object from a .VRML file (internal).
template<typename tf, typename tc>
CImg<T>& _load_vrml(std::FILE *const file, const char *const filename,CImgList<tf>& primitives, CImgList<tc>& colors) {
  if (!file && !filename)
    throw CImgArgumentException(_cimg_instance
                                "load_vrml() : Specified filename is (null).",
                                cimg_instance);
  std::FILE *const nfile = file?file:cimg::fopen(filename,"r");

  char line[1024] = { 0 };
  int err;

  // Skip comments, and read the first node.
  do { err = std::fscanf(nfile,"%65535[^\n] ",line); } while (!err || (err==1 && *line=='#'));

  // Check for a first valid vrml valid node.
  if (cimg::strncasecmp(line,"Shape",5) &&
      cimg::strncasecmp(line,"Transform",9) &&
      cimg::strncasecmp(line,"NavigationInfo",14) &&
      cimg::strncasecmp(line,"Billboard",9)) {
    if (!file) cimg::fclose(nfile);
    throw CImgIOException(_cimg_instance
                          "load_vrml() : VRML nodes not found in file '%s'.",
                          cimg_instance,filename?filename:"(FILE*)");
  }

  // Look for the Shape node (as we do not manage the treatment for the other nodes yet).
  if (cimg::strncasecmp(line,"Shape",5)) {
    while (cimg::strncasecmp(line,"Shape",5) && !std::feof(nfile)) err = std::fscanf(nfile,"%1023[^\n] ",line);
    if (std::feof(nfile)) {
      if (!file) cimg::fclose(nfile);
      throw CImgIOException(_cimg_instance
                            "load_vrml() : Shape node not found in file '%s'.",
                            cimg_instance,filename?filename:"(FILE*)");
    }
  }

  // Look for either geometry or appearance node.
  while (cimg::strncasecmp(line,"geometry",8) && cimg::strncasecmp(line,"appearance",10) && !std::feof(nfile)) err = std::fscanf(nfile,"%1023[^\n] ",line);
  if (std::feof(nfile)) { // If none of these nodes are defined.
    if (!file) cimg::fclose(nfile);
    throw CImgIOException(_cimg_instance
                          "load_vrml() : Geometry and appearance nodes not found in file '%s'.",
                          cimg_instance,filename?filename:"(FILE*)");
  }

  std::vector<T> listePoints; // Intermediate list containing the points of the whole object.
  primitives.assign();
  colors.assign();
  int nbPointsTotal = 0, nbPrimitives = 0; // Count the number of points of the whole object and the number of primitives.
  float r = 0.78f, g = 0.78f, b = 0.78f; // RGB level of the object, the object is gray by default.
  bool colorDefined = true, multipleColors = false, textureTest = false; // Boolean used to know if a color is defined for an object, if this object has multiple colors or if the object has a texture
  char textureFile[1024] = { 0 }; // Variable containing the name of the image used as a texture

  while (!std::feof(nfile)) {
    char type[1024] = { 0 }, textureFileTemp[1024] = { 0 };
    colorDefined = true;
    if (!cimg::strncasecmp(line,"geometry",8)) {       // We are at the geometry node
      std::sscanf(line,"geometry %s",type);            // We are looking for the type of geometry to draw
      const CImg<float> coords = CImg<float>::empty(); // CImg used for the texturization of an object
      CImgList<tc> colorsTextured;                     // CImgList used for the texturization of the color of an object
      CImgList<tf> primitivesTemp;                     // Intermediate CImgList used to update the primitives of the whole object

      if (!cimg::strncasecmp(type,"Box",3)) { // If the object to draw is a box
        while (cimg::strncasecmp(line,"size",4) && !std::feof(nfile)) // We are looking for the size of the box
          err = std::fscanf(nfile,"%1023[^\n] ",line);

        if (std::feof(nfile)) { // If no size is specified
          if (!file) cimg::fclose(nfile);
          throw CImgIOException(_cimg_instance
                                "load_vrml() : size of box not defined in file '%s'.",
                                cimg_instance, filename?filename:"(FILE*)");
        }

        float X = 0, Y = 0, Z = 0; // The width, height and depth of the box
        if ((err = std::sscanf(line,"size %f %f %f[^\n] ",&X,&Y,&Z))!=3 && (err = std::sscanf(line,"size %f,%f,%f[^\n] ",&X,&Y,&Z))!=3) {
          if (!file) cimg::fclose(nfile);
          throw CImgIOException(_cimg_instance
                                "load_vrml() : Failed to read box size in file '%s'.",
                                cimg_instance,filename?filename:"(FILE*)");
        }
        const CImg<T> pointsTemp = CImg<T>::box3d(primitivesTemp,(T)X,(T)Y,(T)Z); // We generate the primitives and the points of the box

        nbPrimitives = primitivesTemp.size(); // We save the number of primitives of the object

        if (textureTest) { // If the object has a texture
          const CImg<float> texture(textureFile);                                      // We put the image used as a texture into a CImg object
          colorsTextured.insert(primitivesTemp.size(),CImg<unsigned char>::vector(0,50,250));   // We initialize the colorsTextured list
          pointsTemp.texturize_object3d(primitivesTemp,colorsTextured,texture,coords); // We texturize the object
          nbPrimitives = 0;
        }

        if(nbPointsTotal) { // If there are already some objects in the scene
          for (int j=0;j<(int)primitivesTemp.size();j++) {
            for(int i=0;i<4;i++)
              primitivesTemp(j).at(i) += (tf)nbPointsTotal; // We shift the indices in the primitives to designate the right points
          }
        }
        primitives.push_back(primitivesTemp); // We add the primitives of the box to the general primitives variable
        for(int i=0;i<(int)pointsTemp.size()/3;++i) { // We add the points into the temporary list in the right order
          listePoints.push_back((T)pointsTemp.at(i));
          listePoints.push_back((T)pointsTemp.at(i+8));
          listePoints.push_back((T)pointsTemp.at(i+16));
        }
        nbPointsTotal += pointsTemp.size()/3; // We increase the number of points of the whole object
      }
      else if(!cimg::strncasecmp(type,"Sphere",6)) { // If the object to draw is a sphere
        while(cimg::strncasecmp(line,"radius",6) && !std::feof(nfile))
          err = std::fscanf(nfile,"%1023[^\n] ",line);

        if(std::feof(nfile)) {
          if (!file)
            cimg::fclose(nfile);
          throw CImgIOException(_cimg_instance "load_vrml() : radius of sphere not defined in file '%s'.",cimg_instance, filename?filename:"(FILE*)");
        }

        float R = 0;
        if ((err = std::sscanf(line,"radius %f[^\n] ",&R))!=1) { // We get the radius of the sphere
          if (!file)
            cimg::fclose(nfile);
          throw CImgIOException(_cimg_instance "load_vrml() : Failed to read sphere radius in file '%s'.",cimg_instance,filename?filename:"(FILE*)");
        }
        const CImg<T> pointsTemp = CImg<T>::sphere3d(primitivesTemp,(T)R); // Compute the necessary points and primitives for a sphere of radius R

        nbPrimitives = primitivesTemp.size(); // We get the number of primitives to used on the attribution of a color, in case no specific color is defined

        if(textureTest) { // If the object has a texture
          const CImg<float> texture(textureFile);                                      // We put the image used as a texture into a CImg object
          colorsTextured.insert(primitivesTemp.size(),CImg<unsigned char>::vector(0,50,250));   // We initialize the colorsTextured list
          pointsTemp.texturize_object3d(primitivesTemp,colorsTextured,texture,coords); // We texturize the object
          nbPrimitives = 0; // We set to 0 because there is no color to use
        }

        if(nbPointsTotal) { // If there are already some objects in the scene
          for (int j=0;j<(int)primitivesTemp.size();j++) {
            for(int i=0;i<3;i++)
              primitivesTemp(j).at(i) += (tf)nbPointsTotal;
          }
        }

        primitives.push_back(primitivesTemp);
        for(int i=0;i<(int)pointsTemp.size()/3;++i) {
          listePoints.push_back((T)pointsTemp.at(i));
          listePoints.push_back((T)pointsTemp.at(i+pointsTemp.size()/3));
          listePoints.push_back((T)pointsTemp.at(i+2*pointsTemp.size()/3));
        }
        nbPointsTotal += pointsTemp.size()/3;
      }
      else if(!cimg::strncasecmp(type,"Cone",4)) { // If the object to draw is a cone
        while(cimg::strncasecmp(line,"bottomRadius",12) && !std::feof(nfile) && cimg::strncasecmp(line,"height",6))
          err = std::fscanf(nfile,"%1023[^\n] ",line);
        float R = 0, H = 0;
        if(std::feof(nfile)) {
          if (!file)
            cimg::fclose(nfile);
          throw CImgIOException(_cimg_instance "load_vrml() : bottom radius and height of cone not defined in file '%s'.",cimg_instance, filename?filename:"(FILE*)");
        }
        else if(!cimg::strncasecmp(line,"bottomRadius",12)) { // We find the bottom radius of the cone first
          if ((err = std::sscanf(line,"bottomRadius %f[^\n] ",&R))!=1) { // We get the radius into the variable R
            if (!file)
              cimg::fclose(nfile);
            throw CImgIOException(_cimg_instance "load_vrml() : Failed to read cone bottomRadius in file '%s'.",cimg_instance,filename?filename:"(FILE*)");
          }
          while(!std::feof(nfile) && cimg::strncasecmp(line,"height",6)) // We look for the height of the cone
            err = std::fscanf(nfile,"%1023[^\n] ",line);

          if(std::feof(nfile)) {
            if (!file)
              cimg::fclose(nfile);
            throw CImgIOException(_cimg_instance "load_vrml() : height of cone not defined in file '%s'.",cimg_instance, filename?filename:"(FILE*)");
          }

          if ((err = std::sscanf(line,"height %f[^\n] ",&H))!=1) { // We get the height into the variable H
            if (!file)
              cimg::fclose(nfile);
            throw CImgIOException(_cimg_instance "load_vrml() : Failed to read cone height in file '%s'.",cimg_instance,filename?filename:"(FILE*)");
          }
        }
        else { // We find the height of the cone first
          if ((err = std::sscanf(line,"height %f[^\n] ",&H))!=1) {
            if (!file)
              cimg::fclose(nfile);
            throw CImgIOException(_cimg_instance "load_vrml() : Failed to read cone height in file '%s'.",cimg_instance,filename?filename:"(FILE*)");
          }
          while(!std::feof(nfile) && cimg::strncasecmp(line,"bottomRadius",12))
            err = std::fscanf(nfile,"%1023[^\n] ",line);

          if(std::feof(nfile)) {
            if (!file)
              cimg::fclose(nfile);
            throw CImgIOException(_cimg_instance "load_vrml() : bottom radius of cone not defined in file '%s'.",cimg_instance, filename?filename:"(FILE*)");
          }

          if ((err = std::sscanf(line,"bottomRadius %f[^\n] ",&R))!=1) {
            if (!file)
              cimg::fclose(nfile);
            throw CImgIOException(_cimg_instance "load_vrml() : Failed to read cone bottom radius in file '%s'.",cimg_instance,filename?filename:"(FILE*)");
          }
        }
        const CImg<T> pointsTemp = CImg<T>::cone3d(primitivesTemp,(T)R,(T)H); // Compute the necessary points and primitives for a cone of radius R and height H

        nbPrimitives = primitivesTemp.size();

        if(textureTest) { // If the object has a texture
          const CImg<float> texture(textureFile);                                      // We put the image used as a texture into a CImg object
          colorsTextured.insert(primitivesTemp.size(),CImg<unsigned char>::vector(0,50,250));   // We initialize the colorsTextured list
          pointsTemp.texturize_object3d(primitivesTemp,colorsTextured,texture,coords); // We texturize the object
          nbPrimitives = 0;
        }

        if(nbPointsTotal) {
          for (int j=0;j<(int)primitivesTemp.size();j++) {
            for(int i=0;i<3;i++)
              primitivesTemp(j).at(i) += (tf)nbPointsTotal;
          }
        }

        primitives.push_back(primitivesTemp);
        for(int i=0;i<(int)pointsTemp.size()/3;++i) {
          listePoints.push_back((T)pointsTemp.at(i));
          listePoints.push_back((T)pointsTemp.at(i+pointsTemp.size()/3));
          listePoints.push_back((T)pointsTemp.at(i+2*pointsTemp.size()/3));
        }
        nbPointsTotal += pointsTemp.size()/3;
      }
      else if(!cimg::strncasecmp(type,"Cylinder",8)) { // If the object to draw is a cylinder
        while(cimg::strncasecmp(line,"radius",6) && !std::feof(nfile) && cimg::strncasecmp(line,"height",6))
          err = std::fscanf(nfile,"%1023[^\n] ",line);
        float R = 0, H = 0;
        if(std::feof(nfile)) {
          if (!file)
            cimg::fclose(nfile);
          throw CImgIOException(_cimg_instance "load_vrml() : radius or height of cylinder not defined in file '%s'.",cimg_instance, filename?filename:"(FILE*)");
        }
        else if(!cimg::strncasecmp(line,"radius",6)) { // If we find the radius first
          if ((err = std::sscanf(line,"radius %f[^\n] ",&R))!=1) {
            if (!file)
              cimg::fclose(nfile);
            throw CImgIOException(_cimg_instance "load_vrml() : Failed to read cylinder radius in file '%s'.",cimg_instance,filename?filename:"(FILE*)");
          }
          while(!std::feof(nfile) && cimg::strncasecmp(line,"height",6)) // We now look for the height of the cylinder
            err = std::fscanf(nfile,"%1023[^\n] ",line);

          if(std::feof(nfile)) {
            if (!file)
              cimg::fclose(nfile);
            throw CImgIOException(_cimg_instance "load_vrml() : height of cylinder not defined in file '%s'.",cimg_instance, filename?filename:"(FILE*)");
          }

          if ((err = std::sscanf(line,"height %f[^\n] ",&H))!=1) {
            if (!file)
              cimg::fclose(nfile);
            throw CImgIOException(_cimg_instance "load_vrml() : Failed to read cylinder height in file '%s'.",cimg_instance,filename?filename:"(FILE*)");
          }
        }
        else { // If we find the height first
          if ((err = std::sscanf(line,"height %f[^\n] ",&H))!=1) {
            if (!file)
              cimg::fclose(nfile);
            throw CImgIOException(_cimg_instance "load_vrml() : Failed to read cylinder height in file '%s'.",cimg_instance,filename?filename:"(FILE*)");
          }
          while(!std::feof(nfile) && cimg::strncasecmp(line,"radius",6))// We now look for the radius of the cylinder
            err = std::fscanf(nfile,"%1023[^\n] ",line);

          if(std::feof(nfile)) {
            if (!file)
              cimg::fclose(nfile);
            throw CImgIOException(_cimg_instance "load_vrml() : radius of cylinder not defined in file '%s'.",cimg_instance, filename?filename:"(FILE*)");
          }

          if ((err = std::sscanf(line,"radius %f[^\n] ",&R))!=1) {
            if (!file)
              cimg::fclose(nfile);
            throw CImgIOException(_cimg_instance "load_vrml() : Failed to read cylinder radius in file '%s'.",cimg_instance,filename?filename:"(FILE*)");
          }
        }
        const CImg<T> pointsTemp = CImg<T>::cylinder3d(primitivesTemp,(T)R,(T)H); // Compute the necessary points and primitives for a cylinder of radius R and height H

        if(textureTest) { // If the object has a texture
          const CImg<float> texture(textureFile);                                      // We put the image used as a texture into a CImg object
          colorsTextured.insert(primitivesTemp.size(),CImg<unsigned char>::vector(0,50,250));   // We initialize the colorsTextured list
          pointsTemp.texturize_object3d(primitivesTemp,colorsTextured,texture,coords); // We texturize the object
          nbPrimitives = 0;
        }

        nbPrimitives = primitivesTemp.size();

        if(nbPointsTotal) {
          for (int j=0;j<(int)primitivesTemp.size();j++) {
            for(int i=0;i<3;i++)
              primitivesTemp(j).at(i) += (tf)nbPointsTotal;
          }
        }

        primitives.push_back(primitivesTemp);
        for(int i=0;i<(int)pointsTemp.size()/3;++i) {
          listePoints.push_back((T)pointsTemp.at(i));
          listePoints.push_back((T)pointsTemp.at(i+pointsTemp.size()/3));
          listePoints.push_back((T)pointsTemp.at(i+2*pointsTemp.size()/3));
        }
        nbPointsTotal += pointsTemp.size()/3;
      }
      else if(!cimg::strncasecmp(type,"PointSet",8)) { // If the object to draw is a set of points
        while(cimg::strncasecmp(line,"point [",7) && !std::feof(nfile))
          err = std::fscanf(nfile,"%1023[^\n] ",line);

        if(std::feof(nfile)) {
          if (!file)
            cimg::fclose(nfile);
          throw CImgIOException(_cimg_instance "load_vrml() : points of pointSet node not defined in file '%s'.",cimg_instance, filename?filename:"(FILE*)");
        }

        err = std::fscanf(nfile,"%1023[^\n] ",line);
        int nbPoints = 0;

        while(cimg::strncasecmp(line,"]",1) && !std::feof(nfile)) { // while we did not get all the points and while we are not at the end of the file
          float X=0,Y=0,Z=0;
          if ((err = std::sscanf(line,"%f %f %f,[^\n] ",&X,&Y,&Z))==3 || (err = std::sscanf(line,"%f,%f,%f,[^\n] ",&X,&Y,&Z))==3) {
            // We get the coordinates of all the points and store them into a list of points
            listePoints.push_back((T)X);
            listePoints.push_back((T)Y);
            listePoints.push_back((T)Z);
            ++nbPoints;
          }
          err = std::fscanf(nfile,"%1023[^\n] ",line);
        }
        if(std::feof(nfile)) {
          if (!file)
            cimg::fclose(nfile);
          throw CImgIOException(_cimg_instance "load_vrml() : bad structure of pointSet node in file '%s'.",cimg_instance, filename?filename:"(FILE*)");
        }
        primitivesTemp.assign();

        for(int i=0;i<nbPoints;++i) { // The primitive is only composed of the indice of the point itself
          CImg<tf> temp(1,1,1,1,(tf)i);
          primitivesTemp.push_back(temp);
        }

        if(nbPointsTotal) {
          for (int j=0;j<(int)primitivesTemp.size();j++) {
            for(int i=0;i<(int)primitivesTemp(j).size();i++)
              primitivesTemp(j).at(i) += (tf)nbPointsTotal;
          }
        }
        nbPrimitives = primitivesTemp.size();

        primitives.push_back(primitivesTemp);

        nbPointsTotal += nbPoints;
      }
      else if(!cimg::strncasecmp(type,"IndexedLineSet",14) || !cimg::strncasecmp(type,"IndexedFaceSet",14)) { // If the object to draw is a set of lines or a set of faces
        while(cimg::strncasecmp(line,"point [",7) && !std::feof(nfile))
          err = std::fscanf(nfile,"%1023[^\n] ",line);
        if(std::feof(nfile)) {
          if (!file)
            cimg::fclose(nfile);
          throw CImgIOException(_cimg_instance "load_vrml() : points of IndexedSet node not defined in file '%s'.",cimg_instance, filename?filename:"(FILE*)");
        }

        err = std::fscanf(nfile,"%1023[^\n] ",line);
        int nbPoints = 0;
        while(cimg::strncasecmp(line,"]",1) && !std::feof(nfile)) { // As long as there are points defined we add them to the list
          float X=0,Y=0,Z=0;
          if ((err = std::sscanf(line,"%f %f %f,[^\n] ",&X,&Y,&Z))==3 || (err = std::sscanf(line,"%f,%f,%f,[^\n] ",&X,&Y,&Z))==3) {
            // We get the coordinates of the points into a list of points
            listePoints.push_back((T)X);
            listePoints.push_back((T)Y);
            listePoints.push_back((T)Z);
            ++nbPoints;
          }
          err = std::fscanf(nfile,"%1023[^\n] ",line);
        }
        if(std::feof(nfile)) {
          if (!file)
            cimg::fclose(nfile);
          throw CImgIOException(_cimg_instance "load_vrml() : bad structure of point vector node in file '%s'.",cimg_instance, filename?filename:"(FILE*)");
        }

        primitivesTemp.assign();

        while(cimg::strncasecmp(line,"coordIndex [",12) && !std::feof(nfile)) // We are looking for the index of the points
          err = std::fscanf(nfile,"%1023[^\n] ",line);
        if(std::feof(nfile)) {
          if (!file)
            cimg::fclose(nfile);
          throw CImgIOException(_cimg_instance "load_vrml() : coordIndex not furnished for IndexedSet node in file '%s'.",cimg_instance, filename?filename:"(FILE*)");
        }

        err = std::fscanf(nfile,"%1023[^\n] ",line);

        while(cimg::strncasecmp(line,"]",1) && !std::feof(nfile)) { // As long as there are indices
          if(*line!='#') {
            std::vector<tf> primitiveComponents;
            char * pch;
            pch = std::strtok (line,",");

            while (pch != NULL && std::atof(pch)!=-1) { // We extract the list of indices and store them into a vector
              if(!(int)count(primitiveComponents.begin(),primitiveComponents.end(),(tf)std::atof(pch)))
                primitiveComponents.push_back((tf)std::atof(pch));
              pch = std::strtok (NULL, ",");
            }
            CImg<tf> temp(1,primitiveComponents.size(),1,1);

            for(int i=0;i<(int)primitiveComponents.size();++i)
              temp(0,i) = primitiveComponents.at(i);
            primitivesTemp.push_back(temp);
          }
          err = std::fscanf(nfile,"%1023[^\n] ",line);
        }

        if(std::feof(nfile)) {
          if (!file)
            cimg::fclose(nfile);
          throw CImgIOException(_cimg_instance "load_vrml() : bad structure of coordIndex in file '%s'.",cimg_instance, filename?filename:"(FILE*)");
        }

        if(nbPointsTotal) {
          for (int j=0;j<(int)primitivesTemp.size();j++) {
            for(int i=0;i<(int)primitivesTemp(j).size();i++)
              primitivesTemp(j).at(i) += (tf)nbPointsTotal;
          }
        }

        nbPrimitives = primitivesTemp.size();
        primitives.push_back(primitivesTemp);
        nbPointsTotal += nbPoints;

        while(cimg::strncasecmp(line,"color [",7) && cimg::strncasecmp(line,"}",1) && !std::feof(nfile))
          err = std::fscanf(nfile,"%1023[^\n] ",line);
        if(std::feof(nfile)) {
          if (!file)
            cimg::fclose(nfile);
          throw CImgIOException(_cimg_instance "load_vrml() : bad structure of coordIndex in file '%s'.",cimg_instance, filename?filename:"(FILE*)");
        }
        else if(!cimg::strncasecmp(line,"color [",7)) { // If there are different colors defined for each faces
          multipleColors = true;
          std::vector<CImg<tc> > listColors;
          err = std::fscanf(nfile,"%1023[^\n] ",line);
          while(cimg::strncasecmp(line,"]",1) && !std::feof(nfile)) { // We add the list of all colors defined into the vector listColors
            if(*line!='#') {
              if ((err = std::sscanf(line,"%f %f %f[^\n] ",&r,&g,&b))!=3) {
                if (!file)
                  cimg::fclose(nfile);
                throw CImgIOException(_cimg_instance "load_vrml() : wrong number of color furnished in file '%s'.",cimg_instance,filename?filename:"(FILE*)");
              }
              CImg<tc> img(3,1,1,1,(tc)(r*255),(tc)(g*255),(tc)(b*255));
              listColors.push_back(img);
            }
            err = std::fscanf(nfile,"%1023[^\n] ",line);
          }
          if(std::feof(nfile)) {
            if (!file)
              cimg::fclose(nfile);
            throw CImgIOException(_cimg_instance "load_vrml() : bad structure of color in file '%s'.",cimg_instance, filename?filename:"(FILE*)");
          }
          else {
            while(cimg::strncasecmp(line,"colorIndex [",12) && !std::feof(nfile))
              err = std::fscanf(nfile,"%1023[^\n] ",line);
            if(std::feof(nfile)) {
              if (!file)
                cimg::fclose(nfile);
              throw CImgIOException(_cimg_instance "load_vrml() : colorIndex not furnished for Color node in file '%s'.",cimg_instance, filename?filename:"(FILE*)");
            }
            err = std::fscanf(nfile,"%1023[^\n] ",line);
            while(cimg::strncasecmp(line,"]",1) && !std::feof(nfile)) { // We add the colors at the right index into the vector colors
              if(*line!='#') {
                char * pch;
                pch = std::strtok (line," ");
                while (pch != NULL) {
                  int indice = std::atoi(pch);
                  colors.insert(CImg<tc>::vector((tc)(listColors[indice])[0],(tc)(listColors[indice])[1],(tc)(listColors[indice])[2]));
                  pch = std::strtok (NULL, " ");
                }
              }
              err = std::fscanf(nfile,"%1023[^\n] ",line);
            }
          }
        }
      }
      else // If none of the known type of shape is defined
        cimg::warn(_cimg_instance "load_vrml() : Failed to read type of geometry to draw from file '%s'.", cimg_instance,filename?filename:"(FILE*)");

      if(textureTest) { // If the object considered is texturized
        colors.push_back(colorsTextured);
        *textureFile = 0;
      }
      while(cimg::strncasecmp(line,"appearance",10) && cimg::strncasecmp(line,"Shape",5) && !std::feof(nfile)) // We look for the node appearance or for another shape
        err = std::fscanf(nfile,"%1023[^\n] ",line);
    }
    if(!cimg::strncasecmp(line,"appearance",10)) { // We are at the appearance node
      while(cimg::strncasecmp(line,"texture ImageTexture",20) && cimg::strncasecmp(line,"diffuseColor",12) && !std::feof(nfile)) // We are looking for a valid appearance node
        err = std::fscanf(nfile,"%1023[^\n] ",line);
      if(!cimg::strncasecmp(line,"diffuseColor",12)) { // If the object as a unique diffuse color
        if ((err = std::sscanf(line,"diffuseColor %f,%f,%f[^\n] ",&r,&g,&b))!=3 && (err = std::sscanf(line,"diffuseColor %f %f %f[^\n] ",&r,&g,&b))!=3) {
          if (!file)
            cimg::fclose(nfile);
          throw CImgIOException(_cimg_instance "load_vrml() : wrong number of color furnished in file '%s'.",cimg_instance,filename?filename:"(FILE*)");
        }
      }
      else if(!cimg::strncasecmp(line,"texture ImageTexture",20)) { // If there is a texture defined in the VRML file
        textureTest = true;
        colorDefined = false;
        while(cimg::strncasecmp(line,"url",3) && !std::feof(nfile))
          err = std::fscanf(nfile,"%1023[^\n] ",line);
        if(std::feof(nfile)) {
          if (!file)
            cimg::fclose(nfile);
          throw CImgIOException(_cimg_instance "load_vrml() : texture not defined in file '%s'.",cimg_instance, filename?filename:"(FILE*)");
        }
        std::sscanf(line,"url [%s][^\n] ",textureFileTemp); // We temporary put the name of the texture image into textureFileTemp

        char * pch;
        pch = std::strtok (textureFileTemp,"\"");
        strcpy(textureFile,pch); // We put the url of the texture image into textureFile
      }
    }
    else if(!cimg::strncasecmp(line,"Shape",5)) // We have another shape node
      textureTest = false; // We reinitialize the texture boolean

    if(nbPrimitives && colorDefined && !multipleColors && !textureTest) { // If there is only one color defined we add it to the colors CImgList or if no color is defined for an object, we add the default color
      CImgList<tc> colorsTemp;
      colorsTemp.insert(nbPrimitives,CImg<tc>::vector((tc)(r*255),(tc)(g*255),(tc)(b*255)));
      colors.push_back(colorsTemp);
      nbPrimitives = 0;
      r = 0.7f;
      g = 0.7f;
      b = 0.7f;
    }
    err = std::fscanf(nfile,"%1023[^\n] ",line);
  }

  assign(listePoints.size()/3,3);
  cimg_forX(*this,l) { // We add the points coordinates to the calling object
    (*this)(l,0) = (T)(listePoints.at(l*3));
    (*this)(l,1) = (T)(listePoints.at(l*3+1));
    (*this)(l,2) = (T)(listePoints.at(l*3+2));
  }

  if (!file)
    cimg::fclose(nfile);

  return *this;
}

//! Save VRML files.
template<typename tf, typename tc>
const CImg<T>& save_vrml(const char *const filename,const CImgList<tf>& primitives, const CImgList<tc>& colors, const char *const texturefile = 0) const {
  return _save_vrml(0,filename,primitives,colors,texturefile);
}

//! Save VRML files.
template<typename tf, typename tc>
const CImg<T>& save_vrml(std::FILE *const file,const CImgList<tf>& primitives, const CImgList<tc>& colors, const char *const texturefile = 0) const {
  return _save_vrml(file,0,primitives,colors,texturefile);
}

// Save VRML files (internal).
template<typename tf, typename tc>
const CImg<T>& _save_vrml(std::FILE *const file, const char *const filename,const CImgList<tf>& primitives, const CImgList<tc>& colors, const char *const texturefile) const {

  // Check that the user furnished a file to save the object and that the object is not empty.
  if (!file && !filename)
    throw CImgArgumentException(_cimg_instance
                                "save_vrml() : Specified filename is (null).",
                                cimg_instance);
  if (is_empty())
    throw CImgInstanceException(_cimg_instance
                                "save_vrml() : Empty instance, for file '%s'.",
                                cimg_instance,filename?filename:"(FILE*)");

  // Check that the object we want to save is a 3D object.
  CImgList<T> opacities;
  char error_message[1024] = {0};
  if (!is_object3d(primitives,colors,opacities,true,error_message))
    throw CImgInstanceException(_cimg_instance "save_vrml() : Invalid specified 3d object, for file '%s' (%s).",cimg_instance,filename?filename:"(FILE*)",error_message);
  const CImg<tc> default_color(1,3,1,1,200);

  // We open the file in which we will save the 3D object.
  std::FILE * nfile;
  if(file) nfile = file;
  else nfile = cimg::fopen(filename,"w");

  // We use the version 2.0 of VRML to represent the object in UTF8
  std::fprintf(nfile,"#VRML V2.0 utf8\n");

  // We copy the coordinates of all the points
  std::fprintf(nfile,"Shape {\n\tgeometry IndexedFaceSet {\n\t\tcoord Coordinate {\n\t\t\tpoint [\n");
  cimg_forX(*this,i)
    std::fprintf(nfile,"\t\t\t\t%f %f %f,\n",(float)((*this)(i,0)),(float)((*this)(i,1)),(float)((*this)(i,2)));
  std::fprintf(nfile,"\t\t\t]\n\t\t}\n\t\tcoordIndex [\n");
  bool sameColor = true;

  float r = colors[0][0]/255.0f;
  float g = colors[0][1]/255.0f;
  float b = colors[0][2]/255.0f;

  std::vector<std::string> listColor;
  std::string listColorPerFace("");
  for(int i=0;i<(int)colors.size();++i) {// Test if the object is composed of only one color
    float valR = (colors[i][0])/255.0f;
    float valG = (colors[i][1])/255.0f;
    float valB = (colors[i][2])/255.0f;

    if (r!=valR || g!=valG || b!=valB) { // If the object has different colors
      sameColor = false;
      i = colors.size();
    }
  }

  cimglist_for(primitives,l) { // For each primitive
    const CImg<tc>& color = l<colors.width()?colors[l]:default_color;
    const unsigned int psiz = primitives[l].size(), csiz = color.size();
    float r = color[0]/255.0f;
    float g, b;
    if (csiz > 1) g = color[1]/255.0f;
    else g = r/255.0f;
    if (csiz > 2) b = color[2]/255.0f;
    else b = g/255.0f;

    switch (psiz) {
    case 1 :
      std::fprintf(nfile,"\t\t\t%u,-1\n",(unsigned int)primitives(l,0));
      break;
    case 2 :
      std::fprintf(nfile,"\t\t\t%u,%u,-1\n",(unsigned int)primitives(l,0),(unsigned int)primitives(l,1));
      break;
    case 3 :
      std::fprintf(nfile,"\t\t\t%u,%u,%u,-1\n",(unsigned int)primitives(l,0),(unsigned int)primitives(l,2),(unsigned int)primitives(l,1));
      break;
    case 4 :
      std::fprintf(nfile,"\t\t\t%u,%u,%u,%u,-1\n",(unsigned int)primitives(l,0),(unsigned int)primitives(l,3),(unsigned int)primitives(l,2),(unsigned int)primitives(l,1));
      break;
    case 6 : {
      const unsigned int xt = (unsigned int)primitives(l,2), yt = (unsigned int)primitives(l,3);
      r = color.atXY(xt,yt,0)/255.0f;
      g = (csiz>1?color.atXY(xt,yt,1):r)/255.0f;
      b = (csiz>2?color.atXY(xt,yt,2):g)/255.0f;
      std::fprintf(nfile,"\t\t\t%u,%u,-1\n",(unsigned int)primitives(l,0),(unsigned int)primitives(l,1));
    } break;
    case 9 : {
      const unsigned int xt = (unsigned int)primitives(l,3), yt = (unsigned int)primitives(l,4);
      r = color.atXY(xt,yt,0)/255.0f;
      g = (csiz>1?color.atXY(xt,yt,1):r)/255.0f;
      b = (csiz>2?color.atXY(xt,yt,2):g)/255.0f;
      std::fprintf(nfile,"\t\t\t%u,%u,%u,-1\n",(unsigned int)primitives(l,0),(unsigned int)primitives(l,2),(unsigned int)primitives(l,1));
    } break;
    case 12 : {
      const unsigned int xt = (unsigned int)primitives(l,4), yt = (unsigned int)primitives(l,5);
      r = color.atXY(xt,yt,0)/255.0f;
      g = (csiz>1?color.atXY(xt,yt,1):r)/255.0f;
      b = (csiz>2?color.atXY(xt,yt,2):g)/255.0f;
      std::fprintf(nfile,"\t\t\t%u,%u,%u,%u,-1\n",(unsigned int)primitives(l,0),(unsigned int)primitives(l,3),(unsigned int)primitives(l,2),(unsigned int)primitives(l,1));
    } break;
    }
    if (!sameColor) { // If there are different colors we store on every loop the RGB values into the vector listColor
      std::ostringstream oss;
      oss << r << " " << g << " " << b << "\n";
      if (listColor.size() == 0) {
        listColor.push_back(oss.str());
        listColorPerFace += "0"; // We store the indice of the color
      }
      else {
        std::vector<std::string>::iterator it;
        it = find (listColor.begin(), listColor.end(), oss.str());
        std::ostringstream oss2;
        if(it==listColor.end()) {
          oss2 << " " << listColor.size();
          listColorPerFace += oss2.str();
          listColor.push_back(oss.str());
        }
        else {
          int n = 0;
          for (std::vector<std::string>::iterator iter = listColor.begin(); iter != it; iter++) ++n;
          oss2 << " " << n;
          listColorPerFace += oss2.str();
        }
      }
    }
  }
  std::fprintf(nfile,"\t\t]\n");

  if (texturefile) // If we have a texture instead of a color
    std::fprintf(nfile,"\n\t}\n\tappearance DEF theTexture Appearance {\n\t\ttexture ImageTexture {\n\t\t\turl [\"%s\"]\n\t\t}\n\t}\n}",
                 texturefile);
  else {
    if(!sameColor) { // If there are different colors we add all of them
      std::fprintf(nfile,"\tcolorPerVertex FALSE\n\tcolor Color {\n\t\tcolor [\n");
      while(!listColor.empty()) {
        std::fprintf(nfile,"\t\t\t%s",(listColor.back()).c_str());
        listColor.pop_back();
      }
      std::fprintf(nfile,"\t\t]\n\t}\n\tcolorIndex [\n\t\t");
      std::fprintf(nfile,"%s",listColorPerFace.c_str());
      std::fprintf(nfile,"\n\t]\n\t}\n}");
    }
    else { // If there is only one color we add it with the Material node
      std::fprintf(nfile,"\t}\n\tappearance Appearance {\n\t\tmaterial Material {\n\t\t\tdiffuseColor %f,%f,%f\n\t\t}\n\t}\n}",
                   colors[0][0]/255.0f,colors[0][1]/255.0f,colors[0][2]/255.0f);
    }
  }
  if (!file) cimg::fclose(nfile);
  return *this;
}

#endif
