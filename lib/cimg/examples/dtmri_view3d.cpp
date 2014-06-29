/*
 #
 #  File        : dtmri_view3d.cpp
 #                ( C++ source file )
 #
 #  Description : A viewer of Diffusion-Tensor MRI volumes (medical imaging).
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

#include "CImg.h"
using namespace cimg_library;
#undef min
#undef max

// Compute fractional anisotropy (FA) of a tensor
//-------------------------------------------
template<typename T> float get_FA(const T& val1, const T& val2, const T& val3) {
  const float
    l1 = val1>0?val1:0, l2 = val2>0?val2:0, l3 = val3>0?val3:0,
    lm = (l1+l2+l3)/3,
    tr2 = 2*(l1*l1 + l2*l2 + l3*l3),
    ll1 = l1-lm,
    ll2 = l2-lm,
    ll3 = l3-lm;
  if (tr2>0) return (float)std::sqrt(3*(ll1*ll1 + ll2*ll2 + ll3*ll3)/tr2);
  return 0;
}

// Insert an ellipsoid in a CImg 3D scene
//----------------------------------------
template<typename t, typename tp, typename tf, typename tc>
void insert_ellipsoid(const CImg<t>& tensor, const float X, const float Y, const float Z, const float tfact,
                      const float vx, const float vy, const float vz,
                      CImgList<tp>& points, CImgList<tf>& faces, CImgList<tc>& colors,
                      const unsigned int res1=20, const unsigned int res2=20) {

  // Compute eigen elements
  float l1 = tensor[0], l2 = tensor[1], l3 = tensor[2], fa = get_FA(l1,l2,l3);
  CImg<> vec = CImg<>::matrix(tensor[3],tensor[6],tensor[9],
                              tensor[4],tensor[7],tensor[10],
                              tensor[5],tensor[8],tensor[11]);
  const int
    r = (int)cimg::min(30 + 1.5f*cimg::abs(255*fa*tensor[3]),255.0f),
    g = (int)cimg::min(30 + 1.5f*cimg::abs(255*fa*tensor[4]),255.0f),
    b = (int)cimg::min(30 + 1.5f*cimg::abs(255*fa*tensor[5]),255.0f);

  // Define mesh points
  const unsigned int N0 = points.size();
  for (unsigned int v = 1; v<res2; v++)
    for (unsigned int u = 0; u<res1; u++) {
      const float
        alpha = (float)(u*2*cimg::PI/res1),
        beta = (float)(-cimg::PI/2 + v*cimg::PI/res2),
        x = (float)(tfact*l1*std::cos(beta)*std::cos(alpha)),
        y = (float)(tfact*l2*std::cos(beta)*std::sin(alpha)),
        z = (float)(tfact*l3*std::sin(beta));
      points.insert((CImg<tp>::vector(X,Y,Z) + vec*CImg<tp>::vector(x,y,z)).mul(CImg<tp>::vector(vx,vy,vz)));
    }
  const unsigned int N1 = points.size();
  points.insert((CImg<tp>::vector(X,Y,Z) - vec*CImg<tp>::vector(0,0,l3*tfact)));
  points.insert((CImg<tp>::vector(X,Y,Z) + vec*CImg<tp>::vector(0,0,l3*tfact)));
  points[points.size()-2](0)*=vx; points[points.size()-2](1)*=vy; points[points.size()-2](2)*=vz;
  points[points.size()-1](0)*=vx; points[points.size()-1](1)*=vy; points[points.size()-1](2)*=vz;

  // Define mesh triangles
  for (unsigned int vv = 0; vv<res2-2; ++vv)
    for (unsigned int uu = 0; uu<res1; ++uu) {
      const int nv = (vv+1)%(res2-1), nu = (uu+1)%res1;
      faces.insert(CImg<tf>::vector(N0+res1*vv+nu,N0+res1*nv+uu,N0+res1*vv+uu));
      faces.insert(CImg<tf>::vector(N0+res1*vv+nu,N0+res1*nv+nu,N0+res1*nv+uu));
      colors.insert(CImg<tc>::vector(r,g,b));
      colors.insert(CImg<tc>::vector(r,g,b));
    }
  for (unsigned int uu = 0; uu<res1; ++uu) {
    const int nu = (uu+1)%res1;
    faces.insert(CImg<tf>::vector(N0+nu,N0+uu,N1));
    faces.insert(CImg<tf>::vector(N0+res1*(res2-2)+nu, N1+1,N0+res1*(res2-2)+uu));
    colors.insert(CImg<tc>::vector(r,g,b));
    colors.insert(CImg<tc>::vector(r,g,b));
  }
}

// Insert a fiber in a CImg 3D scene
//-----------------------------------
template<typename T,typename te,typename tp, typename tf, typename tc>
void insert_fiber(const CImg<T>& fiber, const CImg<te>& eigen, const CImg<tc>& palette,
                  const int xm, const int ym, const int zm,
                  const float vx, const float vy, const float vz,
                  CImgList<tp>& points, CImgList<tf>& primitives, CImgList<tc>& colors) {
  const int N0 = points.size();
  float x0 = fiber(0,0), y0 = fiber(0,1), z0 = fiber(0,2), fa0 = eigen.linear_atXYZ(x0,y0,z0,12);
  points.insert(CImg<>::vector(vx*(x0-xm),vy*(y0-ym),vz*(z0-zm)));
  for (int l = 1; l<fiber.width(); ++l) {
    float x1 = fiber(l,0), y1 = fiber(l,1), z1 = fiber(l,2), fa1 = eigen.linear_atXYZ(x1,y1,z1,12);
    points.insert(CImg<tp>::vector(vx*(x1-xm),vy*(y1-ym),vz*(z1-zm)));
    primitives.insert(CImg<tf>::vector(N0+l-1,N0+l));
    const unsigned char
      icol = (unsigned char)(fa0*255),
      r = palette(icol,0),
      g = palette(icol,1),
      b = palette(icol,2);
    colors.insert(CImg<unsigned char>::vector(r,g,b));
    x0 = x1; y0 = y1; z0 = z1; fa0 = fa1;
  }
}

// Compute fiber tracking using 4th-order Runge Kutta integration
//-----------------------------------------------------------------
template<typename T>
CImg<> get_fibertrack(CImg<T>& eigen,
                      const int X0, const int Y0, const int Z0, const float lmax=100,
                      const float dl=0.1f, const float FAmin=0.7f, const float cmin=0.5f) {
#define align_eigen(i,j,k) \
  { T &u = eigen(i,j,k,3), &v = eigen(i,j,k,4), &w = eigen(i,j,k,5); \
    if (u*cu+v*cv+w*cw<0) { u=-u; v=-v; w=-w; }}

  CImgList<> resf;

  // Forward tracking
  float normU = 0, normpU = 0, l = 0, X = (float)X0, Y = (float)Y0, Z = (float)Z0;
  T
    pu = eigen(X0,Y0,Z0,3),
    pv = eigen(X0,Y0,Z0,4),
    pw = eigen(X0,Y0,Z0,5);
  normpU = (float)std::sqrt(pu*pu + pv*pv + pw*pw);
  bool stopflag = false;

  while (!stopflag) {
    if (X<0 || X>eigen.width()-1 || Y<0 || Y>eigen.height()-1 || Z<0 || Z>eigen.depth()-1 ||
        eigen((int)X,(int)Y,(int)Z,12)<FAmin || l>lmax) stopflag = true;
    else {
      resf.insert(CImg<>::vector(X,Y,Z));

      const int
        cx = (int)X, px = (cx-1<0)?0:cx-1, nx = (cx+1>=eigen.width())?eigen.width()-1:cx+1,
        cy = (int)Y, py = (cy-1<0)?0:cy-1, ny = (cy+1>=eigen.height())?eigen.height()-1:cy+1,
        cz = (int)Z, pz = (cz-1<0)?0:cz-1, nz = (cz+1>=eigen.depth())?eigen.depth()-1:cz+1;
      const T cu = eigen(cx,cy,cz,3), cv = eigen(cx,cy,cz,4), cw = eigen(cx,cy,cz,5);

      align_eigen(px,py,pz); align_eigen(cx,py,pz); align_eigen(nx,py,pz);
      align_eigen(px,cy,pz); align_eigen(cx,cy,pz); align_eigen(nx,cy,pz);
      align_eigen(px,ny,pz); align_eigen(cx,ny,pz); align_eigen(nx,ny,pz);
      align_eigen(px,py,cz); align_eigen(cx,py,cz); align_eigen(nx,py,cz);
      align_eigen(px,cy,cz);                        align_eigen(nx,cy,cz);
      align_eigen(px,ny,cz); align_eigen(cx,ny,cz); align_eigen(nx,ny,cz);
      align_eigen(px,py,nz); align_eigen(cx,py,nz); align_eigen(nx,py,nz);
      align_eigen(px,cy,nz); align_eigen(cx,cy,nz); align_eigen(nx,cy,nz);
      align_eigen(px,ny,nz); align_eigen(cx,ny,nz); align_eigen(nx,ny,nz);

      const T
        u0 = 0.5f*dl*eigen.linear_atXYZ(X,Y,Z,3),
        v0 = 0.5f*dl*eigen.linear_atXYZ(X,Y,Z,4),
        w0 = 0.5f*dl*eigen.linear_atXYZ(X,Y,Z,5),
        u1 = 0.5f*dl*eigen.linear_atXYZ(X+u0,Y+v0,Z+w0,3),
        v1 = 0.5f*dl*eigen.linear_atXYZ(X+u0,Y+v0,Z+w0,4),
        w1 = 0.5f*dl*eigen.linear_atXYZ(X+u0,Y+v0,Z+w0,5),
        u2 = 0.5f*dl*eigen.linear_atXYZ(X+u1,Y+v1,Z+w1,3),
        v2 = 0.5f*dl*eigen.linear_atXYZ(X+u1,Y+v1,Z+w1,4),
        w2 = 0.5f*dl*eigen.linear_atXYZ(X+u1,Y+v1,Z+w1,5),
        u3 = 0.5f*dl*eigen.linear_atXYZ(X+u2,Y+v2,Z+w2,3),
        v3 = 0.5f*dl*eigen.linear_atXYZ(X+u2,Y+v2,Z+w2,4),
        w3 = 0.5f*dl*eigen.linear_atXYZ(X+u2,Y+v2,Z+w2,5);
      T
        u = u0/3 + 2*u1/3 + 2*u2/3 + u3/3,
        v = v0/3 + 2*v1/3 + 2*v2/3 + v3/3,
        w = w0/3 + 2*w1/3 + 2*w2/3 + w3/3;
      if (u*pu + v*pv + w*pw<0) { u = -u; v = -v; w = -w; }
      normU = (float)std::sqrt(u*u + v*v + w*w);
      const float scal = (u*pu + v*pv + w*pw)/(normU*normpU);
      if (scal<cmin) stopflag=true;

      X+=(pu=u); Y+=(pv=v); Z+=(pw=w);
      normpU = normU;
      l+=dl;
    }
  }

  // Backward tracking
  l = dl; X = (float)X0; Y = (float)Y0; Z = (float)Z0;
  pu = eigen(X0,Y0,Z0,3);
  pv = eigen(X0,Y0,Z0,4);
  pw = eigen(X0,Y0,Z0,5);
  normpU = (float)std::sqrt(pu*pu + pv*pv + pw*pw);
  stopflag = false;

  while (!stopflag) {
    if (X<0 || X>eigen.width()-1 || Y<0 || Y>eigen.height()-1 || Z<0 || Z>eigen.depth()-1 ||
        eigen((int)X,(int)Y,(int)Z,12)<FAmin || l>lmax) stopflag = true;
    else {

      const int
        cx = (int)X, px = (cx-1<0)?0:cx-1, nx = (cx+1>=eigen.width())?eigen.width()-1:cx+1,
        cy = (int)Y, py = (cy-1<0)?0:cy-1, ny = (cy+1>=eigen.height())?eigen.height()-1:cy+1,
        cz = (int)Z, pz = (cz-1<0)?0:cz-1, nz = (cz+1>=eigen.depth())?eigen.depth()-1:cz+1;
      const T cu = eigen(cx,cy,cz,3), cv = eigen(cx,cy,cz,4), cw = eigen(cx,cy,cz,5);

      align_eigen(px,py,pz); align_eigen(cx,py,pz); align_eigen(nx,py,pz);
      align_eigen(px,cy,pz); align_eigen(cx,cy,pz); align_eigen(nx,cy,pz);
      align_eigen(px,ny,pz); align_eigen(cx,ny,pz); align_eigen(nx,ny,pz);
      align_eigen(px,py,cz); align_eigen(cx,py,cz); align_eigen(nx,py,cz);
      align_eigen(px,cy,cz);                        align_eigen(nx,cy,cz);
      align_eigen(px,ny,cz); align_eigen(cx,ny,cz); align_eigen(nx,ny,cz);
      align_eigen(px,py,nz); align_eigen(cx,py,nz); align_eigen(nx,py,nz);
      align_eigen(px,cy,nz); align_eigen(cx,cy,nz); align_eigen(nx,cy,nz);
      align_eigen(px,ny,nz); align_eigen(cx,ny,nz); align_eigen(nx,ny,nz);

      const T
        u0 = 0.5f*dl*eigen.linear_atXYZ(X,Y,Z,3),
        v0 = 0.5f*dl*eigen.linear_atXYZ(X,Y,Z,4),
        w0 = 0.5f*dl*eigen.linear_atXYZ(X,Y,Z,5),
        u1 = 0.5f*dl*eigen.linear_atXYZ(X+u0,Y+v0,Z+w0,3),
        v1 = 0.5f*dl*eigen.linear_atXYZ(X+u0,Y+v0,Z+w0,4),
        w1 = 0.5f*dl*eigen.linear_atXYZ(X+u0,Y+v0,Z+w0,5),
        u2 = 0.5f*dl*eigen.linear_atXYZ(X+u1,Y+v1,Z+w1,3),
        v2 = 0.5f*dl*eigen.linear_atXYZ(X+u1,Y+v1,Z+w1,4),
        w2 = 0.5f*dl*eigen.linear_atXYZ(X+u1,Y+v1,Z+w1,5),
        u3 = 0.5f*dl*eigen.linear_atXYZ(X+u2,Y+v2,Z+w2,3),
        v3 = 0.5f*dl*eigen.linear_atXYZ(X+u2,Y+v2,Z+w2,4),
        w3 = 0.5f*dl*eigen.linear_atXYZ(X+u2,Y+v2,Z+w2,5);
      T
        u = u0/3 + 2*u1/3 + 2*u2/3 + u3/3,
        v = v0/3 + 2*v1/3 + 2*v2/3 + v3/3,
        w = w0/3 + 2*w1/3 + 2*w2/3 + w3/3;
      if (u*pu + v*pv + w*pw<0) { u = -u; v = -v; w = -w; }
      normU = (float)std::sqrt(u*u + v*v + w*w);
      const float scal = (u*pu + v*pv + w*pw)/(normU*normpU);
      if (scal<cmin) stopflag=true;

      X-=(pu=u); Y-=(pv=v); Z-=(pw=w);
      normpU=normU;
      l+=dl;

      resf.insert(CImg<>::vector(X,Y,Z),0);
    }
  }

  return resf>'x';
}

// Main procedure
//----------------
int main(int argc,char **argv) {

  // Read and init data
  //--------------------
  cimg_usage("A viewer of Diffusion-Tensor MRI volumes.");
  const char *file_i   = cimg_option("-i",(char*)0,"Input : Filename of tensor field (volume wxhxdx6)");
  const char* vsize    = cimg_option("-vsize","1x1x1","Input : Voxel aspect");
  const bool normalize = cimg_option("-normalize",true,"Input : Enable tensor normalization");
  const char *file_f   = cimg_option("-f",(char*)0,"Input : Input fibers\n");
  const float dl       = cimg_option("-dl",0.5f,"Fiber computation : Integration step");
  const float famin    = cimg_option("-famin",0.3f,"Fiber computation : Fractional Anisotropy threshold");
  const float cmin     = cimg_option("-cmin",0.2f,"Fiber computation : Curvature threshold");
  const float lmin     = cimg_option("-lmin",10.0f,"Fiber computation : Minimum length\n");
  const float lmax     = cimg_option("-lmax",1000.0f,"Fiber computation : Maximum length\n");
  const float tfact    = cimg_option("-tfact",1.2f,"Display : Tensor size factor");
  const char *bgcolor  = cimg_option("-bg","0,0,0","Display : Background color");
  unsigned int bgr = 0, bgg = 0, bgb = 0;
  std::sscanf(bgcolor,"%u%*c%u%*c%u",&bgr,&bgg,&bgb);

  CImg<> tensors;
  if (file_i) {
    std::fprintf(stderr,"\n- Loading tensors '%s'",cimg::basename(file_i));
    tensors.load(file_i);
  } else {
    // Create a synthetic tensor field here
    std::fprintf(stderr,"\n- No input files : Creating a synthetic tensor field");
    tensors.assign(32,32,32,6);
    cimg_forXYZ(tensors,x,y,z) {
      const float
        u = x - tensors.width()/2.0f,
        v = y - tensors.height()/2.0f,
        w = z - tensors.depth()/2.0f,
        norm = (float)std::sqrt(1e-5f + u*u + v*v + w*w),
        nu = u/norm, nv = v/norm, nw = w/norm;
      const CImg<>
        dir1 = CImg<>::vector(nu,nv,nw),
        dir2 = CImg<>::vector(-nv,nu,nw),
        dir3 = CImg<>::vector(nw*(nv-nu),-nw*(nu+nv),nu*nu+nv*nv);
      tensors.set_tensor_at(2.0*dir1*dir1.get_transpose() +
                            1.0*dir2*dir2.get_transpose() +
                            0.7*dir3*dir3.get_transpose(),
                            x,y,z);
    }
  }
  float voxw = 1, voxh = 1, voxd = 1;
  std::sscanf(vsize,"%f%*c%f%*c%f",&voxw,&voxh,&voxd);

  std::fprintf(stderr," : %ux%ux%u image, voxsize=%gx%gx%g.",
               tensors.width(),tensors.height(),tensors.depth(),
               voxw,voxh,voxd);

  CImgList<> fibers;
  if (file_f) {
    std::fprintf(stderr,"\n- Loading fibers '%s'.",cimg::basename(file_f));
    fibers.load(file_f);
  }

  const CImg<unsigned char> fiber_palette =
    CImg<>(2,1,1,3).fill(200,255,0,255,0,200).RGBtoHSV().resize(256,1,1,3,3).HSVtoRGB();

  // Compute eigen elements
  //------------------------
  std::fprintf(stderr,"\n- Compute eigen elements.");
  CImg<unsigned char> coloredFA(tensors.width(),tensors.height(),tensors.depth(),3);
  CImg<> eigen(tensors.width(),tensors.height(),tensors.depth(),13);
  CImg<> val,vec;
  float eigmax = 0;
  cimg_forXYZ(tensors,x,y,z) {
    tensors.get_tensor_at(x,y,z).symmetric_eigen(val,vec);
    eigen(x,y,z,0) = val[0]; eigen(x,y,z,1) = val[1]; eigen(x,y,z,2) = val[2];
    if (val[0]<0) val[0] = 0;
    if (val[1]<0) val[1] = 0;
    if (val[2]<0) val[2] = 0;
    if (val[0]>eigmax) eigmax = val[0];
    eigen(x,y,z,3) = vec(0,0); eigen(x,y,z,4)  = vec(0,1); eigen(x,y,z,5)  = vec(0,2);
    eigen(x,y,z,6) = vec(1,0); eigen(x,y,z,7)  = vec(1,1); eigen(x,y,z,8)  = vec(1,2);
    eigen(x,y,z,9) = vec(2,0); eigen(x,y,z,10) = vec(2,1); eigen(x,y,z,11) = vec(2,2);
    const float fa = get_FA(val[0],val[1],val[2]);
    eigen(x,y,z,12) = fa;
    const int
      r = (int)cimg::min(255.0f,1.5f*cimg::abs(255*fa*vec(0,0))),
      g = (int)cimg::min(255.0f,1.5f*cimg::abs(255*fa*vec(0,1))),
      b = (int)cimg::min(255.0f,1.5f*cimg::abs(255*fa*vec(0,2)));
    coloredFA(x,y,z,0) = (unsigned char)r;
    coloredFA(x,y,z,1) = (unsigned char)g;
    coloredFA(x,y,z,2) = (unsigned char)b;
  }
  tensors.assign();
  std::fprintf(stderr,"\n- Maximum diffusivity = %g, Maximum FA = %g",eigmax,eigen.get_shared_channel(12).max());
  if (normalize) {
    std::fprintf(stderr,"\n- Normalize tensors.");
    eigen.get_shared_channels(0,2)/=eigmax;
  }

  // Init display and begin user interaction
  //-----------------------------------------
  std::fprintf(stderr,"\n- Open user window.");
  CImgDisplay disp(256,256,"DTMRI Viewer",0);
  CImgDisplay disp3d(800,600,"3D Local View",0,false,true);
  unsigned int XYZ[3];
  XYZ[0] = eigen.width()/2; XYZ[1] = eigen.height()/2; XYZ[2] = eigen.depth()/2;

  while (!disp.is_closed() && !disp.is_keyQ() && !disp.is_keyESC()) {
    const CImg<int> s = coloredFA.get_select(disp,2,XYZ);
    if (!disp.is_closed()) switch (disp.key()) {

      // Open 3D visualization window
      //-----------------------------
    case cimg::keyA :
    case 0 : {
      const unsigned char white[] = { 255 };
      disp3d.display(CImg<unsigned char>(disp3d.width(),disp3d.height(),1,1,0).draw_text(10,10,"Please wait...",white)).show();
      int xm,ym,zm,xM,yM,zM;
      if (!disp.key()) { xm = s[0]; ym = s[1]; zm = s[2]; xM = s[3]; yM = s[4]; zM = s[5]; }
      else { xm = ym = zm = 0; xM = eigen.width() - 1; yM = eigen.height() - 1; zM = eigen.height() - 1; }
      const CImg<> img = eigen.get_crop(xm,ym,zm,xM,yM,zM);
      CImgList<> points;
      CImgList<unsigned int> primitives;
      CImgList<unsigned char> colors;

      // Add ellipsoids to the 3D scene
      int X = img.width()/2, Y = img.height()/2, Z = img.depth()/2;
      cimg_forXY(img,x,y) insert_ellipsoid(img.get_vector_at(x,y,Z),(float)x,(float)y,(float)Z,tfact,voxw,voxh,voxd,points,primitives,colors,10,6);
      cimg_forXZ(img,x,z) insert_ellipsoid(img.get_vector_at(x,Y,z),(float)x,(float)Y,(float)z,tfact,voxw,voxh,voxd,points,primitives,colors,10,6);
      cimg_forYZ(img,y,z) insert_ellipsoid(img.get_vector_at(X,y,z),(float)X,(float)y,(float)z,tfact,voxw,voxh,voxd,points,primitives,colors,10,6);

      // Add computed fibers to the 3D scene
      const CImg<> veigen = eigen.get_crop(xm,ym,zm,xM,yM,zM);
      cimglist_for(fibers,l) {
        const CImg<>& fiber = fibers[l];
        if (fiber.width()) insert_fiber(fiber,eigen,fiber_palette,
                                       xm,ym,zm,voxw,voxh,voxd,
                                       points,primitives,colors);
      }

      // Display 3D object
      CImg<unsigned char> visu = CImg<unsigned char>(3,disp3d.width(),disp3d.height(),1,0).fill(bgr,bgg,bgb).permute_axes("yzcx");
      bool stopflag = false;
      while (!disp3d.is_closed() && !stopflag) {
        const CImg<> pts = points>'x';
        visu.display_object3d(disp3d,pts,primitives,colors,true,4,-1,false,800,0.05f,1.0f);
        disp3d.close();
        switch (disp3d.key()) {
        case cimg::keyM : { // Create movie
          std::fprintf(stderr,"\n- Movie mode.\n");
          const unsigned int N = 256;
          CImg<> cpts(pts);
          const CImg<> x = pts.get_shared_row(0), y = pts.get_shared_row(1), z = pts.get_shared_row(2);
          float
            xm, xM = x.max_min(xm),
            ym, yM = y.max_min(ym),
            zm, zM = z.max_min(zm),
            ratio = 2.0f*cimg::min(visu.width(),visu.height())/(3.0f*cimg::max(xM-xm,yM-ym,zM-zm)),
            dx = 0.5f*(xM+xm), dy = 0.5f*(yM+ym), dz = 0.5f*(zM+zm);
          cimg_forX(pts,l) {
            cpts(l,0) = (pts(l,0)-dx)*ratio;
            cpts(l,1) = (pts(l,1)-dy)*ratio;
            cpts(l,2) = (pts(l,2)-dz)*ratio;
          }

          for (unsigned int i=0; i<N; i++) {
            std::fprintf(stderr,"\r- Frame %u/%u.",i,N);
            const float alpha = (float)(i*2*cimg::PI/N);
            const CImg<> rpts = CImg<>::rotation_matrix(0,1,0,alpha)*CImg<>::rotation_matrix(1,0,0,1.30f)*cpts;
            visu.fill(0).draw_object3d(visu.width()/2.0f,visu.height()/2.0f,-500.0f,rpts,primitives,colors,
                                       4,false,800.0f,visu.width()/2.0f,visu.height()/2.0f,-800.0f,0.05f,1.0f).display(disp3d);
            visu.save("frame.png",i);
          }
          visu.fill(0);
        } break;
        default: stopflag = true;
        }
      }
      if (disp3d.is_fullscreen()) disp3d.toggle_fullscreen().resize(800,600).close();
    } break;

    // Compute region statistics
    //---------------------------
    case cimg::keyR : {
      std::fprintf(stderr,"\n- Statistics computation. Select region."); std::fflush(stderr);
      const CImg<int> s = coloredFA.get_select(disp,2,XYZ);
      int xm, ym, zm, xM, yM, zM;
      if (!disp.key()) { xm = s[0]; ym = s[1]; zm = s[2]; xM = s[3]; yM = s[4]; zM = s[5]; }
      else { xm = ym = zm = 0; xM = eigen.width()-1; yM = eigen.height()-1; zM = eigen.height()-1; }
      const CImg<> img = eigen.get_crop(xm,ym,zm,xM,yM,zM);
      std::fprintf(stderr,"\n- Mean diffusivity = %g, Mean FA = %g\n",
                   eigen.get_shared_channel(0).mean(),
                   eigen.get_shared_channel(12).mean());
    } break;

    // Track fiber bundle (single region)
    //----------------------------------
    case cimg::keyF : {
      std::fprintf(stderr,"\n- Tracking mode (single region). Select starting region.\n"); std::fflush(stderr);
      const CImg<int> s = coloredFA.get_select(disp,2,XYZ);
      const unsigned int N = fibers.size();
      for (int z=s[2]; z<=s[5]; z++)
        for (int y=s[1]; y<=s[4]; y++)
          for (int x=s[0]; x<=s[3]; x++) {
            const CImg<> fiber = get_fibertrack(eigen,x,y,z,lmax,dl,famin,cmin);
            if (fiber.width()>lmin) {
              std::fprintf(stderr,"\rFiber %u : Starting from (%d,%d,%d)\t\t",fibers.size(),x,y,z);
              fibers.insert(fiber);
            }
          }
      std::fprintf(stderr,"\n- %u fiber(s) added (total %u).",fibers.size()-N,fibers.size());
    } break;

    // Track fiber bundle (double regions)
    //------------------------------------
    case cimg::keyG : {
      std::fprintf(stderr,"\n- Tracking mode (double region). Select starting region."); std::fflush(stderr);
      const CImg<int> s = coloredFA.get_select(disp,2,XYZ);
      std::fprintf(stderr," Select ending region."); std::fflush(stderr);
      const CImg<int> ns = coloredFA.get_select(disp,2,XYZ);
      const unsigned int N = fibers.size();

      // Track from start to end
      for (int z = s[2]; z<=s[5]; ++z)
        for (int y = s[1]; y<=s[4]; ++y)
          for (int x = s[0]; x<=s[3]; ++x) {
            const CImg<> fiber = get_fibertrack(eigen,x,y,z,lmax,dl,famin,cmin);
            if (fiber.width()>lmin) {
              bool valid_fiber = false;
              cimg_forX(fiber,k) {
                const int fx = (int)fiber(k,0), fy = (int)fiber(k,1), fz = (int)fiber(k,2);
                if (fx>=ns[0] && fx<=ns[3] &&
                    fy>=ns[1] && fy<=ns[4] &&
                    fz>=ns[2] && fz<=ns[5]) valid_fiber = true;
              }
              if (valid_fiber) fibers.insert(fiber);
            }
          }

      // Track from end to start
      for (int z = ns[2]; z<=ns[5]; ++z)
        for (int y = ns[1]; y<=ns[4]; ++y)
          for (int x = ns[0]; x<=ns[3]; ++x) {
            const CImg<> fiber = get_fibertrack(eigen,x,y,z,lmax,dl,famin,cmin);
            if (fiber.width()>lmin) {
              bool valid_fiber = false;
              cimg_forX(fiber,k) {
                const int fx = (int)fiber(k,0), fy = (int)fiber(k,1), fz = (int)fiber(k,2);
                if (fx>=s[0] && fx<=s[3] &&
                    fy>=s[1] && fy<=s[4] &&
                    fz>=s[2] && fz<=s[5]) valid_fiber = true;
              }
              if (valid_fiber) {
                std::fprintf(stderr,"\rFiber %u : Starting from (%d,%d,%d)\t\t",fibers.size(),x,y,z);
                fibers.insert(fiber);
              }
            }
          }

      std::fprintf(stderr," %u fiber(s) added (total %u).",fibers.size()-N,fibers.size());
    } break;

    // Clear fiber bundle
    //-------------------
    case cimg::keyC : {
      std::fprintf(stderr,"\n- Fibers removed.");
      fibers.assign();
    } break;

    // Save fibers
    //-------------
    case cimg::keyS : {
      fibers.save("fibers.cimg");
      std::fprintf(stderr,"\n- Fibers saved.");
    } break;

    }
  }

  std::fprintf(stderr,"\n- Exit.\n\n\n");
  return 0;
}
