#include <stdio.h>
#include <math.h>

#include <real.h>
#include <co/memory.h>
#include <co/off.h>
#include <co/eigen.h>
#include <co/he.h>
#include <co/y.h>

static int nv;
static real *x, *y, *z;
static real vec[3*3];
static He *he;
static Eigen *eigen;

int main() {

  //Euler angles, see wikipedia
  real alpha, beta, gamma;
  real theta1, theta2, phi;
  real t, tt;
  
  y_inif(stdin, &he, &x, &y, &z);
  nv = he_nv(he);
  eigen_ini(he, &eigen);
  eigen_vector_surface(eigen, he, x, y, z, &vec[0]);
  eigen_fin(eigen);

  t=1-vec[6]*vec[6];
  t=sqrt(t);
  tt=-vec[3]/t;
  alpha=acos(tt);
  beta=acos(vec[6]);
  
  tt=-vec[7]/t;
  gamma=acos(tt);
  
  theta1=atan2(vec[0], vec[6]);
  theta2=atan2(vec[8], vec[2]);
  phi   =atan2(vec[3], vec[0]);
  printf("%g %g %g %g %g %g\n", alpha, beta, gamma, theta1, theta2, phi);
  
  y_fin(he, x, y, z);
  return 0;
}
