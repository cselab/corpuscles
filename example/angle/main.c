#include <stdio.h>
#include <tgmath.h>

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
  eigen_vector(eigen, x, y, z, &vec[0]);
  eigen_fin(eigen);

  t=1-vec[6]*vec[6];
  t=sqrt(t);
  tt=-vec[3]/t;
  alpha=acos(tt);
  beta=acos(vec[6]);
  
  tt=-vec[7]/t;
  gamma=acos(tt);
  
  //printf("%f %f %f\n", vec[0], vec[3], vec[6]);
  //printf("%f %f %f\n", vec[1], vec[4], vec[7]);
  //printf("%f %f %f\n", vec[2], vec[5], vec[8]);
  theta1=atan(vec[0]/vec[6]);
  theta2=atan(vec[8]/vec[2]);
  phi   =atan(vec[3]/vec[0]);
  printf("%f %f %f %f %f %f\n", alpha, beta, gamma, theta1, theta2, phi);
  
  y_fin(he, x, y, z);
  return 0;
}
