#include <stdio.h>
#include <math.h>

#include <real.h>
#include <co/memory.h>
#include <co/off.h>
#include <co/he.h>
#include <co/y.h>
#include "co/err.h"


#define FMT_IN CO_REAL_IN

static int nv;
static real *x, *y, *z;
static He *he;

static int mult(const real m[2*2], real *px, real *py) {
    enum {XX, XY, YX, YY};
    real x, y, z, x0, y0, z0;
    x = *px; y = *py;

    x0 = x*m[XX] + y*m[XY];
    y0 = x*m[YX] + y*m[YY];

    *px = x0;
    *py = y0; 

    return CO_OK;
}

static int num(char **v, /**/ int *p) {
  if (*v == NULL) {
        ER("not enough args");
    }
    if (sscanf(*v, "%d", p) != 1)
        ER("not a number '%s'", *v);
    return CO_OK;
}

static int scl(char **v, /**/ real *p) {
    if (*v == NULL) ER("not enough args");
    if (sscanf(*v, FMT_IN, p) != 1)
        ER("not a number '%s'", *v);
    return CO_OK;
}

int main(int argc, char **argv) {
  
  int nv, i, axis;
  real m[4], theta;

  axis=1;//1 x; 2 y; 3 z
  theta=0;//in radian
  num(++argv, &axis);
  scl(++argv, &theta);
  
  MSG("axis=%i theta=%f", axis, theta);
  
  y_inif(stdin, &he, &x, &y, &z);
  nv = he_nv(he);
  
  m[0]=cos(theta);
  m[2]=sin(theta);
  m[1]=-m[2];
  m[3]=m[0];

  if (axis == 1 ) {
    for (i = 0; i < nv; i++)
      mult(m, /**/  &y[i], &z[i]);
  }
  else if (axis == 2) {
    for (i = 0; i < nv; i++)
      mult(m, /**/  &z[i], &x[i]);
  }
  else if (axis == 3) {
    for (i = 0; i < nv; i++)
      mult(m, /**/  &x[i], &y[i]);
  }
  else {
    MSG("Wrong axis!");
  }
  
  off_he_xyz_fwrite(he, x, y, z, stdout);
  
  y_fin(he, x, y, z);
  return 0;
}
