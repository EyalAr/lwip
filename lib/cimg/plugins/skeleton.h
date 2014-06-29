/*
 #
 #  File        : skeleton.h
 #                ( C++ header file - CImg plug-in )
 #
 #  Description : CImg plugin that implements the computation of the Hamilton-Jacobi skeletons
 #                using Siddiqi algorithm with the correction proposed by Torsello,
 #                as described in :
 #
 #  [SBTZ02] K. Siddiqi, S. Bouix, A. Tannenbaum and S.W. Zucker. Hamilton-Jacobi Skeletons
 #           International Journal of Computer Vision, 48(3):215-231, 2002
 #
 #  [TH03]   A. Torsello and E. R. Hancock. Curvature Correction of the Hamilton-Jacobi Skeleton
 #           IEEE Computer Vision and Pattern Recognition, 2003
 #
 #  [BST05] S. Bouix, K. Siddiqi and A. Tannenbaum. Flux driven automatic centerline
 #          extraction. Medical Image Analysis, 9:209-221, 2005
 #
 #  IMPORTANT WARNING : You must include STL's <queue> before plugin inclusion to make it working !
 #
 #  Copyright   : Francois-Xavier Dupe
 #                ( http://www.greyc.ensicaen.fr/~fdupe/ )
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
#ifndef cimg_plugin_skeleton
#define cimg_plugin_skeleton

/**
 * Compute the flux of the gradient
 * @param grad   the gradient of the distance function
 * @param sY     the sampling size in Y
 * @param sZ     the sampling size in Z
 * @return the flux
 */
CImg<floatT> get_flux(const CImgList<floatT> & grad,
                      const float sY=1.0f, const float sZ=1.0f) const {

  int stop = 0;  // Stop flag
  float f = 0;   // The current flux
  int count = 0; // Counter
  CImg<floatT> flux(width(),height(),depth(),1,0);

  cimg_forXYZ((*this),x,y,z) {
    if (!(*this)(x,y,z)) continue; // If the point is the background

    // Look at the neigthboorhound and compute the flux
    stop = 0;
    f = 0;
    count = 0;

    for (int k = -1; k<=1; ++k)
      for (int l = -1; l<= 1; ++l)
        for (int m = -1; m<= 1; ++m) {
          if (stop==1) continue;

          // Protection
          if ((x+k<0) || (x+k>=width()) || (y+l<0) || (y+l>=height()) ||
              (z+m<0) || (z+m>=depth()) || (k==0 && l==0 && m==0)) continue;
          ++count;

          // Test if the point is in the interior
          if ((*this)(x+k,y+l,z+m)==0) { stop = 1; continue; }

          // Compute the flux
          f+=(grad(0,x+k,y+l,z+m)*k + grad(1,x+k,y+l,z+m)*l/sY + grad(2,x+k,y+l,z+m)*m/sZ)/std::sqrt((float)(k*k+l*l+m*m));
        }

    // Update
    if (stop==1 || count==0) flux(x,y,z) = 0;
    else flux(x,y,z) = f/count;
  }

  return flux;
}

/**
 * Definition of a point with his flux value
 */
struct _PointFlux {
  int pos [3];
  float flux;
  float dist;
};

/**
 * Class for the priority queue
 */
class _compare_point {
  /**
   * Create medial curves
   */
  bool curve;

 public:
  _compare_point(const bool curve=false) { this->curve = curve; }

  bool operator()(const _PointFlux & p1, const _PointFlux & p2) const {
    if (curve) {
      if (p1.dist>p2.dist) return true;
      else if (p1.dist==p2.dist && p1.flux<p2.flux) return true;
    } else {
      if (p1.flux<p2.flux) return true;
      else if (p1.flux==p2.flux && p1.dist>p2.dist) return true;
    }
    return false;
  }
};

/**
 * Compute the log-density using the algorithm from Torsello
 * @param dist  the distance map
 * @param grad  the gradient of the distance map, e.g. the flux
 * @param flux  the divergence map
 * @param delta the threshold for the division
 * @return the logdensity \rho
 */
CImg<floatT> get_logdensity(const CImg<floatT> & dist,
                            const CImgList<floatT> & grad,
                            const CImg<floatT> & flux, float delta = 0.1) const {
  std::priority_queue< _PointFlux, std::vector<_PointFlux>, _compare_point > pqueue(true);
  CImg<floatT> logdensity(width(),height(),depth(),1,0);

  // 1 - Put all the pixel inside the priority queue
  cimg_forXYZ(dist,x,y,z) if (dist(x,y,z)!=0) {
    _PointFlux p;
    p.pos[0] = x;
    p.pos[1] = y;
    p.pos[2] = z;
    p.flux = 0;
    p.dist = dist(x,y,z);
    pqueue.push(p);
  }

  // 2 - Compute the logdensity
  while (!pqueue.empty()) {
    _PointFlux p = pqueue.top();
    pqueue.pop();

    const float
      Fx = grad(0,p.pos[0],p.pos[1],p.pos[2]),
      Fy = grad(1,p.pos[0],p.pos[1],p.pos[2]),
      Fz = grad(2,p.pos[0],p.pos[1],p.pos[2]);

    logdensity(p.pos[0],p.pos[1],p.pos[2]) = logdensity.linear_atXYZ(p.pos[0]-Fx,p.pos[1]-Fy,p.pos[2]-Fz)
      - 0.5f * (flux(p.pos[0],p.pos[1],p.pos[2])+flux.linear_atXYZ(p.pos[0]-Fx,p.pos[1]-Fy,p.pos[2]-Fz));

    const float tmp = 1.0f - (1.0f-fabs(Fx)) * (1.0f-fabs(Fy)) * (1.0f-fabs(Fz));
    if (tmp>delta) logdensity(p.pos[0],p.pos[1],p.pos[2])/=tmp;
    else if (delta<1) logdensity(p.pos[0],p.pos[1],p.pos[2]) = 0;
  }

  return logdensity;
}

/**
 * Computed the corrected divergence map using Torsello formula and idea
 * @param logdensity the log density map
 * @param grad       the gradient of the distance map
 * @param flux       the flux using siddiqi formula
 * @param delta      the discrete step
 * @return the corrected divergence map
 */
CImg<floatT> get_corrected_flux(const CImg<floatT> & logdensity,
                                const CImgList<floatT> & grad,
                                const CImg<floatT> & flux,
                                float delta = 1.0) const {

  CImg<floatT> corr_map(width(),height(),depth(),1,0);
  cimg_forXYZ(corr_map,x,y,z) {
    const float
      Fx = grad(0,x,y,z),
      Fy = grad(1,x,y,z),
      Fz = grad(2,x,y,z);
    corr_map(x,y,z) = (logdensity(x,y,z) - logdensity.linear_atXYZ(x-Fx,y-Fy,z-Fz)) * expf(logdensity(x,y,z) - 0.5f * delta) +
      0.5f * ( flux.linear_atXYZ(x-Fx,y-Fy,z-Fz)*expf(logdensity.linear_atXYZ(x-Fx,y-Fy,z-Fz)) + flux(x,y,z)*expf(logdensity(x,y,z)));
  }

  return corr_map;
}

/**
 * Test if a point is simple using Euler number for 2D case
 * or using Malandain criterion for 3D case
 * @param img the image
 * @param x the x coordinate
 * @param y the y coordinate
 * @param z the z coordinate
 * @return true if simple
 */
bool _isSimple (const CImg<T> & img, int x, int y, int z ) const {
  if (img.depth()==1) { // 2D case
    int V = 0, E = 0;  // Number of vertices and edges

    for (int k = -1; k<=1; ++k)
      for (int l = -1; l<=1; ++l) {
        // Protection
        if (x+k<0 || x+k>=img.width() || y+l<0 || y+l>=img.height()) continue;

        // Count the number of vertices
        if (img(x+k,y+l)!=0 && !(k==0 && l==0)) {
          ++V;

          // Count the number of edges
          for (int k1 = -1; k1<=1; ++k1)
            for (int l1 = -1; l1<=1; ++l1) {
              // Protection
              if (x+k+k1<0 || x+k+k1>=img.width() || y+l+l1<0 || y+l+l1>=img.height()) continue;

              if (!(k1==0 && l1==0) && img(x+k+k1,y+l+l1)!=0 && k+k1>-2 && l+l1>-2 &&
                  k+k1<2 && l+l1<2 && !(k+k1==0 && l+l1==0))
                ++E;
            }
        }
      }

    // Remove the corner if exists
    if (x-1>=0 && y-1>=0 && img(x-1,y-1)!=0 && img(x,y-1)!=0 && img(x-1,y)!=0) E-=2;
    if (x-1>=0 && y+1<img.height() && img(x-1,y+1)!=0 && img(x,y+1)!=0 && img(x-1,y)!=0) E-=2;
    if (x+1<img.width() && y-1>=0 && img(x+1,y-1)!=0 && img(x,y-1)!=0 && img(x+1,y)!=0) E-=2;
    if (x+1<img.width() && y+1<img.height() && img(x+1,y+1)!=0 && img(x,y+1)!=0 && img(x+1,y)!=0) E-=2;

    // Final return true if it is a tree (eg euler number equal to 1)
    if ((V-E/2)==1) return true;

  } else  { // 3D case
    CImg<intT> visit(3,3,3,1,0); // Visitor table
    int C_asterix = 0, C_bar = 0, count = 0;
    visit(1,1,1) = -1;

    // Compute C^*

    // Seeking for a component
    for (int k = -1; k<=1; ++k)
      for (int l = -1; l<=1; ++l)
        for (int m = -1; m<=1; ++m) {
          int label = 0;

          // Protection
          if (x+k<0 || x+k>=img.width() ||
              y+l<0 || y+l>=img.height() ||
              z+m<0 || z+m>=img.depth() ||
              (k==0 && l==0 && m==0)) continue;

          if (visit(1+k,1+l,1+m)==0 && img(x+k,y+l,z+m)!=0) {
            // Look after the neightbor
            for (int k1 = -1; k1<=1; ++k1)
              for (int l1 = -1; l1<=1; ++l1)
                for (int m1 = -1; m1<=1; ++m1) {
                  // Protection
                  if (x+k+k1<0 || x+k+k1>=img.width() ||
                      y+l+l1<0 || y+l+l1>=img.height() ||
                      z+m+m1<0 || z+m+m1>=img.depth() ||
                      k+k1>1   || k+k1<-1 ||
                      l+l1>1   || l+l1<-1 ||
                      m+m1>1   || m+m1<-1 ) continue;

                  // Search for a already knew component
                  if (visit(1+k+k1,1+l+l1,1+m+m1)>0 &&
                      img(x+k+k1,y+l+l1,z+m+m1)!=0) {
                    if (label==0) label = visit(1+k+k1,1+l+l1,1+m+m1);
                    else if (label!=visit(1+k+k1,1+l+l1,1+m+m1)) {
                      // Meld component
                      --C_asterix;

                      int C = visit(1+k+k1,1+l+l1,1+m+m1);
                      cimg_forXYZ(visit,a,b,c) if (visit(a,b,c)==C) visit(a,b,c) = label;
                    }
                  }
                }

            // Label the point
            if (label==0) {
              // Find a new component
              ++C_asterix;
              ++count;
              visit(1+k,1+l,1+m) = count;
            } else visit(1+k,1+l,1+m) = label;
          }
        }

    if (C_asterix!=1) return false;

    // Compute \bar{C}

    // Reinit visit
    visit.fill(0);
    visit(1,1,1) = -1;

    // Seeking for a component

    // Look at X-axis
    for (int k = -1; k<=1; ++k) {
      if (x+k<0 || x+k>=img.width()) continue;

      if (img(x+k,y,z)==0 && visit(1+k,1,1)==0) {
        ++C_bar;
        ++count;
        visit(1+k,1,1) = count;

        // Follow component
        for (int l = -1; l<=1; ++l) {
          if (y+l<img.height() && y+l>=0 && img(x+k,y+l,z)==0 && visit(1+k,1+l,1)==0)
            visit(1+k,1+l,1) = count;
          if (z+l<img.depth() && z+l>=0 && img(x+k,y,z+l)==0 && visit(1+k,1,1+l)==0)
            visit(1+k,1,1+l) = count;
        }
      }
    }

    // Look at Y-axis
    for (int k = -1; k<=1; ++k) {
      if (y+k<0 || y+k>=img.height()) continue;

      if (img(x,y+k,z)==0 && visit(1,1+k,1)==0) {
        int label = 0;
        ++C_bar;
        ++count;
        visit(1,1+k,1) = count;
        label = count;

        // Follow component
        for (int l = -1; l<=1; ++l) {
          if (l==0) continue;

          if (x+l<img.width() && x+l>=0 && img(x+l,y+k,z)==0) {
            if (visit(1+l,1+k,1)!=0) {
              if (label!=visit(1+l,1+k,1)) {
                // Meld component
                --C_bar;

                int C = visit(1+l,1+k,1);
                cimg_forXYZ(visit,a,b,c)
                  if (visit(a,b,c)==C) visit(a,b,c) = label;
              }
            } else visit(1+l,1+k,1) = label;
          }

          if (z+l<img.depth() && z+l>=0 && img(x,y+k,z+l)==0) {
            if (visit(1,1+k,1+l)!=0) {
              if (label!=visit(1,1+k,1+l)) {
                // Meld component
                --C_bar;

                int C = visit(1,1+k,1+l);
                cimg_forXYZ(visit,a,b,c)
                  if (visit(a,b,c)==C) visit(a,b,c) = label;
              }
            } else visit(1,1+k,1+l) = label;
          }
        }
      }
    }

    // Look at Z-axis
    for (int k = -1; k<=1; ++k) {
      if (z+k<0 || z+k>=img.depth()) continue;

      if (img(x,y,z+k)==0 && visit(1,1,1+k)==0) {
        int label = 0;
        ++C_bar;
        ++count;
        visit(1,1,1+k) = count;
        label = count;

        // Follow component
        for (int l = -1; l<=1; ++l) {
          if (l==0) continue;

          if (x+l<img.width() && x+l>=0 && img(x+l,y,z+k)==0) {
            if (visit(1+l,1,1+k)!=0) {
              if (label!=visit(1+l,1,1+k)) {
                // Meld component
                --C_bar;

                int C = visit(1+l,1,1+k);
                cimg_forXYZ(visit,a,b,c)
                  if (visit(a,b,c)==C) visit(a,b,c) = label;
              }
            } else visit(1+l,1,1+k) = label;
          }

          if (y+l<img.height() && y+l>=0 && img(x,y+l,z+k)==0) {
            if (visit(1,1+l,1+k)!=0) {
              if (label!=visit(1,1+l,1+k)) {
                // Meld component
                --C_bar;

                int C = visit(1,1+l,1+k);
                cimg_forXYZ(visit,a,b,c)
                  if (visit(a,b,c)==C) visit(a,b,c) = label;
              }
            } else visit(1,1+l,1+k) = label;
          }
        }
      }
    }
    if (C_bar==1) return true;
  }

  return false;
}

/**
 * Test if a point is a end point
 * @param img the image
 * @param label the table of labels
 * @param curve set it to true for having medial curve
 * @param x the x coordinate
 * @param y the y coordinate
 * @param z the z coordinate
 * @return true if simple
 */
bool _isEndPoint(const CImg<T> & img, const CImg<T> & label,
                 const bool curve, const int x, const int y, const int z) const {
  if (label(x,y,z)==1) return true;

  if ((!curve) && (img.depth()!=1)) { // 3D case with medial surface
    // Use Pudney specification with the 9 plans
    const int plan9 [9][8][3] = { { {-1,0,-1}, {0,0,-1}, {1,0,-1}, {-1,0,0}, {1,0,0}, {-1,0,1}, {0,0,1}, {1,0,1} }, // Plan 1
                                  { {-1,1,0}, {0,1,0}, {1,1,0}, {-1,0,0}, {1,0,0}, {-1,-1,0}, {0,-1,0}, {1,-1,0} }, // Plan 2
                                  { {0,-1,-1}, {0,0,-1}, {0,1,-1}, {0,-1,0}, {0,1,0}, {0,-1,1}, {0,0,1}, {0,1,1} }, // Plan 3
                                  { {1,1,1}, {0,1,0}, {-1,1,-1}, {1,0,1}, {-1,0,-1}, {-1,-1,-1}, {0,-1,0}, {1,-1,1} }, // Plan 4
                                  { {-1,1,1}, {0,1,0}, {1,1,-1}, {-1,0,1}, {1,0,-1}, {-1,-1,1}, {0,-1,0}, {1,-1,-1} }, // Plan 5
                                  { {-1,1,1}, {0,1,1}, {1,1,1}, {-1,0,0}, {1,0,0}, {-1,-1,-1}, {0,-1,-1}, {1,-1,-1} }, // Plan 6
                                  { {-1,1,-1}, {0,1,-1}, {1,1,-1}, {-1,0,0}, {1,0,0}, {-1,-1,1}, {0,-1,1}, {1,-1,1} }, // Plan 7
                                  { {-1,1,-1}, {-1,1,0}, {-1,1,1}, {0,0,-1}, {0,0,1}, {1,-1,-1}, {1,-1,0}, {1,-1,1} }, // Plan 8
                                  { {1,1,-1}, {1,1,0}, {1,1,1}, {0,0,-1}, {0,0,1}, {-1,-1,-1}, {-1,-1,0}, {-1,-1,1} }  // Plan 9
    };

    // Count the number of neighbors on each plan
    for (int k = 0; k<9; ++k) {
      int count = 0;

      for (int l = 0; l<8; ++l) {
        if (x+plan9[k][l][0]<0 || x+plan9[k][l][0]>=img.width() ||
            y+plan9[k][l][1]<0 || y+plan9[k][l][1]>=img.height() ||
            z+plan9[k][l][2]<0 || z+plan9[k][l][2]>=img.depth()) continue;

        if (img(x+plan9[k][l][0],y+plan9[k][l][1],z+plan9[k][l][2])!=0) ++count;
      }

      if (count<2) return true;
    }
  } else { // 2D or 3D case with medial curve
    int isb = 0;

    for (int k = -1; k<=1; ++k)
      for (int l = -1; l<=1; ++l)
        for (int m = -1; m<=1; ++m) {
          // Protection
          if (x+k<0 || x+k>=img.width() || y+l<0 || y+l>=img.height() ||
              z+m<0 || z+m>=img.depth()) continue;

          if (img(x+k,y+l,z+m)!=0) ++isb;
        }

    if (isb==2) return true; // The pixel with one neighbor
  }

  // Else it's not...
  return false;
}

/**
 * Compute the skeleton of the shape using Hamilton-Jacobi scheme
 * @param flux the flux of the distance gradient
 * @param dist the euclidean distance of the object
 * @param curve create or not medial curve
 * @param thres the threshold on the flux
 * @return the skeleton
 */
CImg<T> get_skeleton (const CImg<floatT> & flux,
                      const CImg<floatT> & dist, const bool curve, const float thres) const {
  CImg<T>
    skeleton(*this),                      // The skeleton
    label(width(),height(),depth(),1,0),  // Save label
    count(width(),height(),depth(),1,0);  // A counter for the queue
  std::priority_queue< _PointFlux, std::vector<_PointFlux>, _compare_point > pqueue(curve);
  int isb = 0;

  // 1 - Init get the bound points
  cimg_forXYZ(*this,x,y,z) {
    if (skeleton(x,y,z)==0) continue;

    // Test bound condition
    isb = 0;
    for (int k = -1; k<=1; ++k)
      for (int l = -1; l<=1; ++l)
        for (int m = -1; m<=1; ++m) {
          // Protection
          if (x+k<0 || x+k>=width() || y+l<0 || y+l>=height() ||
              z+m<0 || z+m>=depth()) continue;
          if (skeleton(x+k,y+l,z+m)==0) isb = 1;
        }

    if (isb==1 && _isSimple(skeleton,x,y,z)) {
      _PointFlux p;
      p.pos[0] = x;
      p.pos[1] = y;
      p.pos[2] = z;
      p.flux = flux(x,y,z);
      p.dist = dist(x,y,z);
      pqueue.push(p);
      count(x,y,z) = 1;
    }
  }

  // 2 - Compute the skeleton
  while (!pqueue.empty()) {
    _PointFlux p = pqueue.top();     // Get the point with the max flux
    pqueue.pop();                    // Remove the point from the queue
    count(p.pos[0],p.pos[1],p.pos[2]) = 0; // Reinit counter

      // Test if the point is simple
    if (_isSimple(skeleton,p.pos[0],p.pos[1],p.pos[2])) {
      if ((! _isEndPoint(skeleton,label,curve,p.pos[0],p.pos[1],p.pos[2])) || p.flux>thres) {
        skeleton(p.pos[0],p.pos[1],p.pos[2]) = 0; // Remove the point

        for (int k = -1; k<=1; ++k)
          for (int l = -1; l<=1; ++l)
            for (int m = -1; m<=1; ++m) {
              // Protection
              if (p.pos[0]+k < 0 || p.pos[0]+k >= width() ||
                  p.pos[1]+l < 0 || p.pos[1]+l >= height() ||
                  p.pos[2]+m < 0 || p.pos[2]+m >= depth()) continue;
              if (skeleton(p.pos[0]+k,p.pos[1]+l,p.pos[2]+m)!=0 &&
                  count(p.pos[0]+k,p.pos[1]+l,p.pos[2]+m)<1 &&
                  _isSimple(skeleton,p.pos[0]+k,p.pos[1]+l,p.pos[2]+m)) {
                _PointFlux p1;
                p1.pos[0] = p.pos[0]+k;
                p1.pos[1] = p.pos[1]+l;
                p1.pos[2] = p.pos[2]+m;
                p1.flux = flux(p.pos[0]+k,p.pos[1]+l,p.pos[2]+m);
                p1.dist = dist(p.pos[0]+k,p.pos[1]+l,p.pos[2]+m);
                pqueue.push(p1);
                count(p.pos[0]+k,p.pos[1]+l,p.pos[2]+m) = 1;
              }
            }
      } else label(p.pos[0],p.pos[1],p.pos[2]) = 1; // Mark the point as skeletal
    }
  }

  return skeleton;
}

/**
 * In place version of get_skeleton
 */
CImg<T> skeleton(const CImg<floatT> & flux,
                 const CImg<floatT> & dist, bool curve ,float thres) {
  return get_skeleton(flux,dist,curve,thres).move_to(*this);
}

#endif /* cimg_skeleton_plugin */
