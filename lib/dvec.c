#include <stdio.h>

#include "real.h"
#include "co/err.h"
#include "co/macro.h"
#include "co/ten.h"
#include "co/vec.h"
#include "co/dvec.h"

enum { X, Y, Z };

int
dvec_norm(const real a[3], Ten * t)
{
  real r, n[3];
  Ten nn;

  r = vec_abs(a);
  if (r == 0)
    ERR(CO_NUM, "r == 0");

  vec_norm(a, n);
  ten_one( /**/ t);
  ten_dyadic(n, n, &nn);
  ten_sub(&nn, t);
  ten_scale(1 / r, t);
  return CO_OK;
}

int
dvec_minus(__UNUSED const real a[3], __UNUSED const real b[3],
           Ten * dA, Ten * dB)
{
  ten_one(dA);

  ten_one(dB);
  ten_scale(-1, dB);
  return CO_OK;
}
