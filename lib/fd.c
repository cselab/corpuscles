#include <stdio.h>

#include "real.h"
#include "co/err.h"
#include "co/fd.h"
#include "co/force.h"
#include "co/he.h"

static real
fd0(real * p, Force * f, He * he, real delta, real * x, real * y, real * z)
{
  real t, hi, lo;

  t = *p;
  *p += delta;
  hi = force_energy(f, he, x, y, z);
  *p = t;
  *p -= delta;
  lo = force_energy(f, he, x, y, z);
  *p = t;
  return (hi - lo) / (2 * delta);
}

int
fd(Force * f, He * he, real delta, real * x, real * y, real * z,
   /**/ real * fx, real * fy, real * fz)
{
  enum { X, Y, Z };
  int n, i;

  n = he_nv(he);
  for (i = 0; i < n; i++) {
    fx[i] = fd0(&x[i], f, he, delta, x, y, z);
    fy[i] = fd0(&y[i], f, he, delta, x, y, z);
    fz[i] = fd0(&z[i], f, he, delta, x, y, z);
  }

  return CO_OK;
}
