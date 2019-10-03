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

  y_inif(stdin, &he, &x, &y, &z);
  nv = he_nv(he);
  eigen_ini(he, &eigen);
  eigen_vector(eigen, x, y, z, &vec[0]);
  eigen_fin(eigen);

  printf("%g %g %g\n", vec[0], vec[3], vec[6]);
  //printf("%f %f %f\n", vec[1], vec[4], vec[7]);
  //printf("%f %f %f\n", vec[2], vec[5], vec[8]);

  y_fin(he, x, y, z);
  return 0;
}
