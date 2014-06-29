/*
 #
 #  File        : generate_loop_macros.cpp
 #                ( C++ source file )
 #
 #  Description : Generate C++ macros to deal with MxN[xP] neighborhood
 #                loops within the CImg Library.
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

// Generate macro(s) 'cimg_forN(i,bound)'
//----------------------------------------
void generate_forN(const unsigned int N) {
  if (N>=2) {
    const unsigned int Nn = N/2, Np = Nn-((N+1)%2);
    std::printf("#define cimg_for%u(bound,i) for (int i = 0, \\\n",N);
    for (unsigned int k = 0; k<Np; ++k) std::printf(" _p%u##i = 0,",Np - k);
    std::printf(" \\\n");
    for (unsigned int k = 1; k<=Nn; ++k) std::printf(" _n%u##i = %u>=(int)(bound)?(int)(bound)-1:%u%c \\\n",k,k,k,k==Nn?';':',');
    std::printf(" _n%u##i<(int)(bound) || ",Nn);
    for (unsigned int k = Nn-1; k>=1; --k) std::printf("_n%u##i==--_n%u##i || ",k,k+1);
    std::printf("\\\n i==(");
    for (unsigned int k = Nn; k>=2; --k) std::printf("_n%u##i = ",k);
    std::printf("--_n1##i); \\\n ");
    for (unsigned int k = Np; k>=2; --k) std::printf("_p%u##i = _p%u##i, ",k,k-1);
    if (Np) std::printf("_p1##i = i++, \\\n ");
    else std::printf(" ++i, ");
    for (unsigned int k = 1; k<Nn; ++k) std::printf("++_n%u##i, ",k);
    std::printf("++_n%u##i)\n\n",Nn);

    std::printf("#define cimg_for%uX(img,x) cimg_for%u((img)._width,x)\n",N,N);
    std::printf("#define cimg_for%uY(img,y) cimg_for%u((img)._height,y)\n",N,N);
    std::printf("#define cimg_for%uZ(img,z) cimg_for%u((img)._depth,z)\n",N,N);
    std::printf("#define cimg_for%uC(img,c) cimg_for%u((img)._spectrum,c)\n",N,N);
    std::printf("#define cimg_for%uXY(img,x,y) cimg_for%uY(img,y) cimg_for%uX(img,x)\n",N,N,N);
    std::printf("#define cimg_for%uXZ(img,x,z) cimg_for%uZ(img,z) cimg_for%uX(img,x)\n",N,N,N);
    std::printf("#define cimg_for%uXC(img,x,c) cimg_for%uC(img,c) cimg_for%uX(img,x)\n",N,N,N);
    std::printf("#define cimg_for%uYZ(img,y,z) cimg_for%uZ(img,z) cimg_for%uY(img,y)\n",N,N,N);
    std::printf("#define cimg_for%uYC(img,y,c) cimg_for%uC(img,c) cimg_for%uY(img,y)\n",N,N,N);
    std::printf("#define cimg_for%uZC(img,z,c) cimg_for%uC(img,c) cimg_for%uZ(img,z)\n",N,N,N);
    std::printf("#define cimg_for%uXYZ(img,x,y,z) cimg_for%uZ(img,z) cimg_for%uXY(img,x,y)\n",N,N,N);
    std::printf("#define cimg_for%uXZC(img,x,z,c) cimg_for%uC(img,c) cimg_for%uXZ(img,x,z)\n",N,N,N);
    std::printf("#define cimg_for%uYZC(img,y,z,c) cimg_for%uC(img,c) cimg_for%uYZ(img,y,z)\n",N,N,N);
    std::printf("#define cimg_for%uXYZC(img,x,y,z,c) cimg_for%uC(img,c) cimg_for%uXYZ(img,x,y,z)\n\n",N,N,N);
  }
}

// Generate macro(s) 'cimg_for_inN(i,bound)'
//------------------------------------------
void generate_for_inN(const unsigned int N) {
  if (N>=2) {
    const unsigned int Nn = N/2, Np = Nn - ((N+1)%2);
    std::printf("#define cimg_for_in%u(bound,i0,i1,i) for (int i = (int)(i0)<0?0:(int)(i0), \\\n",N);
    for (unsigned int k = 0; k<Np; ++k) std::printf(" _p%u##i = i-%u<0?0:i-%u, \\\n",Np-k,Np-k,Np-k);
    for (unsigned int k = 1; k<=Nn; ++k) std::printf(" _n%u##i = i+%u>=(int)(bound)?(int)(bound)-1:i+%u%c \\\n",k,k,k,k==Nn?';':',');
    std::printf(" i<=(int)(i1) && (_n%u##i<(int)(bound) || ",Nn);
    for (unsigned int k = Nn-1; k>=1; --k) std::printf("_n%u##i==--_n%u##i || ",k,k+1);
    std::printf("\\\n i==(");
    for (unsigned int k = Nn; k>=2; --k) std::printf("_n%u##i = ",k);
    std::printf("--_n1##i)); \\\n ");
    for (unsigned int k = Np; k>=2; --k) std::printf("_p%u##i = _p%u##i, ",k,k-1);
    if (Np) std::printf("_p1##i = i++, \\\n ");
    else std::printf(" ++i, ");
    for (unsigned int k = 1; k<Nn; ++k) std::printf("++_n%u##i, ",k);
    std::printf("++_n%u##i)\n\n",Nn);
  }

  std::printf("#define cimg_for_in%uX(img,x0,x1,x) cimg_for_in%u((img)._width,x0,x1,x)\n",N,N);
  std::printf("#define cimg_for_in%uY(img,y0,y1,y) cimg_for_in%u((img)._height,y0,y1,y)\n",N,N);
  std::printf("#define cimg_for_in%uZ(img,z0,z1,z) cimg_for_in%u((img)._depth,z0,z1,z)\n",N,N);
  std::printf("#define cimg_for_in%uC(img,c0,c1,c) cimg_for_in%u((img)._spectrum,c0,c1,c)\n",N,N);
  std::printf("#define cimg_for_in%uXY(img,x0,y0,x1,y1,x,y) cimg_for_in%uY(img,y0,y1,y) cimg_for_in%uX(img,x0,x1,x)\n",N,N,N);
  std::printf("#define cimg_for_in%uXZ(img,x0,z0,x1,z1,x,z) cimg_for_in%uZ(img,z0,z1,z) cimg_for_in%uX(img,x0,x1,x)\n",N,N,N);
  std::printf("#define cimg_for_in%uXC(img,x0,c0,x1,c1,x,c) cimg_for_in%uC(img,c0,c1,c) cimg_for_in%uX(img,x0,x1,x)\n",N,N,N);
  std::printf("#define cimg_for_in%uYZ(img,y0,z0,y1,z1,y,z) cimg_for_in%uZ(img,z0,z1,z) cimg_for_in%uY(img,y0,y1,y)\n",N,N,N);
  std::printf("#define cimg_for_in%uYC(img,y0,c0,y1,c1,y,c) cimg_for_in%uC(img,c0,c1,c) cimg_for_in%uY(img,y0,y1,y)\n",N,N,N);
  std::printf("#define cimg_for_in%uZC(img,z0,c0,z1,c1,z,c) cimg_for_in%uC(img,c0,c1,c) cimg_for_in%uZ(img,z0,z1,z)\n",N,N,N);
  std::printf("#define cimg_for_in%uXYZ(img,x0,y0,z0,x1,y1,z1,x,y,z) cimg_for_in%uZ(img,z0,z1,z) cimg_for_in%uXY(img,x0,y0,x1,y1,x,y)\n",N,N,N);
  std::printf("#define cimg_for_in%uXZC(img,x0,z0,c0,x1,y1,c1,x,z,c) cimg_for_in%uC(img,c0,c1,c) cimg_for_in%uXZ(img,x0,y0,x1,y1,x,z)\n",N,N,N);
  std::printf("#define cimg_for_in%uYZC(img,y0,z0,c0,y1,z1,c1,y,z,c) cimg_for_in%uC(img,c0,c1,c) cimg_for_in%uYZ(img,y0,z0,y1,z1,y,z)\n",N,N,N);
  std::printf("#define cimg_for_in%uXYZC(img,x0,y0,z0,c0,x1,y1,z1,c1,x,y,z,c) cimg_for_in%uC(img,c0,c1,c) cimg_for_in%uXYZ(img,x0,y0,z0,x1,y1,z1,x,y,z)\n\n",N,N,N);

}

// Generate macro 'cimg_forMxN[xP](img,x,y,z,c,I,T)'
//--------------------------------------------------
void generate_forMxNxP(const unsigned int M, const unsigned int N, const unsigned int P) {
  char indx[16], indy[16], indz[16];
  const int
    Mn = (int)(M/2), Mp = (int)(Mn - ((M+1)%2)),
    Nn = (int)(N/2), Np = (int)(Nn - ((N+1)%2)),
    Pn = (int)(P/2), Pp = (int)(Pn - ((P+1)%2)),
    last = (int)(M*N*P);

  if (P>1) std::printf("#define cimg_for%ux%ux%u(img,x,y,z,c,I,T) \\\n cimg_for%u((img)._depth,z)",M,N,P,P);
  else std::printf("#define cimg_for%ux%u(img,x,y,z,c,I,T) \\\n",M,N);
  if (N>1) std::printf(" cimg_for%u((img)._height,y) ",N);
  else std::printf(" cimg_forY(img,y) ");

  std::printf("for (int x = 0%c \\\n",M>1?',':';');
  for (int k = Mp; k>=1; --k) std::printf(" _p%u##x = 0%s",k,k==1?", \\\n":",");
  for (int k = 1; k<Mn; ++k) std::printf(" _n%u##x = %u>=((img)._width)?(img).width()-1:%u, \\\n",k,k,k);

  if (M>1) {
    std::printf(" _n%u##x = (int)( \\\n ",Mn);
    for (int k = 0, z = -Pp; z<=Pn; ++z)
      for (int y = -Np; y<=Nn; ++y) {
        for (int x = -Mp; x<=0; ++x) { std::printf("%sI[%d] =",k && x==-Mp?" (":(x==-Mp?"(":" "),k); ++k; }
        k+=Mn;
        if (y<0) std::sprintf(indy,"_p%d##",-y); else if (y>0) std::sprintf(indy,"_n%d##",y); else indy[0]='\0';
        if (z<0) std::sprintf(indz,"_p%d##",-z); else if (z>0) std::sprintf(indz,"_n%d##",z); else indz[0]='\0';
        std::printf(" (T)(img)(0,%sy,%sz,c))%s",indy,indz,k>=last?",":", \\\n");
      }

    std::printf(" \\\n");
    for (int x = 1; x<Mn; ++x)
      for (int z = -Pp; z<=Pn; ++z)
        for (int y = -Np; y<=Nn; ++y) {
          if (y<0) std::sprintf(indy,"_p%d##",-y); else if (y>0) std::sprintf(indy,"_n%d##",y); else indy[0]='\0';
          if (z<0) std::sprintf(indz,"_p%d##",-z); else if (z>0) std::sprintf(indz,"_n%d##",z); else indz[0]='\0';
          std::printf(" (I[%d] = (T)(img)(_n%d##x,%sy,%sz,c)), \\\n",(Mp+x)+(y+Np)*M+(z+Pp)*M*N,x,indy,indz);
        }
    std::printf(" %u>=((img)._width)?(img).width()-1:%u); \\\n",Mn,Mn);
  }

  if (M>1) std::printf(" (_n%u##x",Mn); else std::printf(" (x");
  std::printf("<(img).width() && ( \\\n");

  for (int z = -Pp; z<=Pn; ++z)
    for (int y = -Np; y<=Nn; ++y) {
      if (M>1) std::sprintf(indx,"_n%d##",Mn); else indx[0]='\0';
      if (y<0) std::sprintf(indy,"_p%d##",-y); else if (y>0) std::sprintf(indy,"_n%d##",y); else indy[0]='\0';
      if (z<0) std::sprintf(indz,"_p%d##",-z); else if (z>0) std::sprintf(indz,"_n%d##",z); else indz[0]='\0';
      std::printf(" (I[%d] = (T)(img)(%sx,%sy,%sz,c))%s",M-1+(y+Np)*M+(z+Pp)*M*N,indx,indy,indz,
                  z==Pn && y==Nn?",1))":", \\\n");
    }

  if (M>1) {
    std::printf(" || \\\n ");
    for (int k = Mn-1; k>=1; --k) std::printf("_n%d##x==--_n%u##x || ",k,k+1);
    std::printf("x==(");
    for (int k = Mn; k>=2; --k) std::printf("_n%d##x = ",k);
    std::printf("--_n1##x); \\\n");
  } else std::printf("; \\\n");

  if (M>1) {
    for (unsigned int k = 0, z = 0; z<P; ++z)
      for (unsigned int y = 0; y<N; ++y) {
        for (unsigned int x = 0; x<M-1; ++x) {
          std::printf(" I[%d] = I[%d],",k,k+1);
          ++k;
        }
        std::printf(" \\\n");
        ++k;
      }
    std::printf(" ");
    for (int k = Mp; k>=2; --k) std::printf("_p%d##x = _p%d##x, ",k,k-1);
    if (M>2) std::printf("_p1##x = x++, "); else std::printf("++x, ");
    for (int k = 1; k<=Mn-1; ++k) std::printf("++_n%d##x, ",k);
    std::printf("++_n%d##x)\n\n",Mn);
  } else std::printf(" ++x)\n\n");
}

// Generate macro 'cimg_for_inMxN[xP](img,x,y,z,c,I,T)'
//-----------------------------------------------------
void generate_for_inMxNxP(const unsigned int M, const unsigned int N, const unsigned int P) {
  char indx[16], indy[16], indz[16];
  const int
    Mn = (int)(M/2), Mp = (int)(Mn - ((M+1)%2)),
    Nn = (int)(N/2), Np = (int)(Nn - ((N+1)%2)),
    Pn = (int)(P/2), Pp = (int)(Pn - ((P+1)%2));

  if (P>1) std::printf("#define cimg_for_in%ux%ux%u(img,x0,y0,z0,x1,y1,z1,x,y,z,c,I,T) \\\n cimg_for_in%u((img)._depth,z0,z1,z)",M,N,P,P);
  else std::printf("#define cimg_for_in%ux%u(img,x0,y0,x1,y1,x,y,z,c,I,T) \\\n",M,N);
  if (N>1) std::printf(" cimg_for_in%u((img)._height,y0,y1,y) ",N);
  else std::printf(" cimg_for_inY(img,y0,y1,y) ");

  std::printf("for (int x = (int)(x0)<0?0:(int)(x0)%c \\\n",M>1?',':';');
  for (int k = Mp; k>=1; --k) std::printf(" _p%u##x = x-%u<0?0:x-%u, \\\n",k,k,k);
  for (int k = 1; k<Mn; ++k) std::printf(" _n%u##x = x+%u>=(img).width()?(img).width()-1:x+%u, \\\n",k,k,k);

  if (M>1) {
    std::printf(" _n%u##x = (int)( \\\n",Mn);
    for (int x = -Mp; x<Mn; ++x)
      for (int z = -Pp; z<=Pn; ++z)
        for (int y = -Np; y<=Nn; ++y) {
          if (x<0) std::sprintf(indx,"_p%d##",-x); else if (x>0) std::sprintf(indx,"_n%d##",x); else indx[0]='\0';
          if (y<0) std::sprintf(indy,"_p%d##",-y); else if (y>0) std::sprintf(indy,"_n%d##",y); else indy[0]='\0';
          if (z<0) std::sprintf(indz,"_p%d##",-z); else if (z>0) std::sprintf(indz,"_n%d##",z); else indz[0]='\0';
          std::printf(" (I[%d] = (T)(img)(%sx,%sy,%sz,c)), \\\n",(Mp+x)+(y+Np)*M+(z+Pp)*M*N,indx,indy,indz);
        }
    std::printf(" x+%u>=(img).width()?(img).width()-1:x+%u); \\\n",Mn,Mn);
  }
  std::printf(" x<=(int)(x1) && (");
  if (M>1) std::printf("(_n%u##x",Mn); else std::printf("(x");
  std::printf("<(img).width() && ( \\\n");

  for (int z = -Pp; z<=Pn; ++z)
    for (int y = -Np; y<=Nn; ++y) {
      if (M>1) std::sprintf(indx,"_n%d##",Mn); else indx[0]='\0';
      if (y<0) std::sprintf(indy,"_p%d##",-y); else if (y>0) std::sprintf(indy,"_n%d##",y); else indy[0]='\0';
      if (z<0) std::sprintf(indz,"_p%d##",-z); else if (z>0) std::sprintf(indz,"_n%d##",z); else indz[0]='\0';
      std::printf(" (I[%d] = (T)(img)(%sx,%sy,%sz,c))%s",M-1+(y+Np)*M+(z+Pp)*M*N,indx,indy,indz,
                  z==Pn && y==Nn?",1))":", \\\n");
    }

  if (M>1) {
    std::printf(" || \\\n ");
    for (int k = Mn-1; k>=1; --k) std::printf("_n%d##x==--_n%u##x || ",k,k+1);
    std::printf("x==(");
    for (int k = Mn; k>=2; --k) std::printf("_n%d##x = ",k);
    std::printf("--_n1##x)); \\\n");
  } else std::printf("); \\\n");

  if (M>1) {
    for (unsigned int k = 0, z = 0; z<P; ++z)
      for (unsigned int y = 0; y<N; ++y) {
        for (unsigned int x = 0; x<M-1; ++x) {
          std::printf(" I[%d] = I[%d],",k,k+1);
          ++k;
        }
        std::printf(" \\\n");
        ++k;
      }
    std::printf(" ");
    for (int k = Mp; k>=2; --k) std::printf("_p%d##x = _p%d##x, ",k,k-1);
    if (M>2) std::printf("_p1##x = x++, "); else std::printf("++x, ");
    for (int k = 1; k<=Mn-1; ++k) std::printf("++_n%d##x, ",k);
    std::printf("++_n%d##x)\n\n",Mn);
  } else std::printf(" ++x)\n\n");
}

// Generate macro 'cimg_getMxN[xP](img,x,y,z,c,I,T)'
//--------------------------------------------------
void generate_getMxNxP(const unsigned int M, const unsigned int N, const unsigned int P) {
  const int
    Mn = (int)(M/2), Mp = (int)(Mn - ((M+1)%2)),
    Nn = (int)(N/2), Np = (int)(Nn - ((N+1)%2)),
    Pn = (int)(P/2), Pp = (int)(Pn - ((P+1)%2)),
    last = M*N*P - 1;
  if (P>1) std::printf("#define cimg_get%ux%ux%u(img,x,y,z,c,I,T) \\\n",M,N,P);
  else std::printf("#define cimg_get%ux%u(img,x,y,z,c,I,T) \\\n",M,N);
  char indx[16], indy[16], indz[16];
  for (int k = 0, z = -Pp; z<=Pn; ++z)
    for (int y = -Np; y<=Nn; ++y)
      for (int x = -Mp; x<=Mn; ++x) {
        if (x<0) std::sprintf(indx,"_p%d##",-x); else if (x>0) std::sprintf(indx,"_n%d##",x); else indx[0]='\0';
        if (y<0) std::sprintf(indy,"_p%d##",-y); else if (y>0) std::sprintf(indy,"_n%d##",y); else indy[0]='\0';
        if (z<0) std::sprintf(indz,"_p%d##",-z); else if (z>0) std::sprintf(indz,"_n%d##",z); else indz[0]='\0';
        std::printf(" I[%u] = (T)(img)(%sx,%sy,%sz,c)%s",k,indx,indy,indz,
                    k==last?";\n\n":(x==Mn?", \\\n":","));
        ++k;
      }
}

//-----------------
// Main Procedure
//-----------------
int main(int argc, char **argv) {

  cimg_usage("Generate C++ macros to deal with MxN[xP] neighborhood loops within the CImg Library");

  // Read command line arguments
  //----------------------------
  const char *const size = cimg_option("-s","5x4","Size of the neighborhood");
  const bool do_forN     = cimg_option("-forN",true,"Generate 'cimg_forN()'");
  const bool do_for_inN  = cimg_option("-for_inN",true,"Generate 'cimg_for_inN()'");
  const bool do_for      = cimg_option("-for",true,"Generate 'cimg_forMxNxP()'");
  const bool do_for_in   = cimg_option("-for_in",true,"Generate 'cimg_for_inMxNxP()'");
  const bool do_get      = cimg_option("-get",true,"Generate 'cimg_getMxNxP()'");
  if (cimg_option("-h",false,0)) std::exit(0);

  unsigned int M = 1, N = 1 , P = 1;
  std::sscanf(size,"%u%*c%u%*c%u",&M,&N,&P);
  if (!M || !N || !P || (M==1 && N==1 && P==1)) {
    std::fprintf(stderr,"\n%s : Error, bad neighborhood size '%s'\n",argv[0],size);
    std::exit(0);
  }
  if (!do_forN && !do_get && !do_for) return 0;

  if (P>1)
    std::printf("// Define %ux%ux%u loop macros\n"
                "//----------------------------\n",M,N,P);
  else
    std::printf("// Define %ux%u loop macros\n"
                "//-------------------------\n",M,N);

  if (do_forN) {
    if (N>1) generate_forN(N);
    if (P>1 && P!=N) generate_forN(P);
  }
  if (do_for_inN) {
    if (N>1) generate_for_inN(N);
    if (P>1 && P!=N) generate_for_inN(P);
  }
  if (do_for) generate_forMxNxP(M,N,P);
  if (do_for_in) generate_for_inMxNxP(M,N,P);
  if (do_get) generate_getMxNxP(M,N,P);

  return 0;
}
