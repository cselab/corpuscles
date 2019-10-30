#include <stdio.h>
#include "real.h"
#include "co/dedg2.h"
#include "co/edg2.h"
#include "co/err.h"
#include "co/vec2.h"

int
dedg2_abs(const real a[2], const real b[2], real da[2], real db[2])
{
  vec2_minus(b, a, db);
  vec2_normalize(db);
  vec2_negative(db, da);
  return CO_OK;
}

int
dedg2_sq(const real a[2], const real b[2], real da[2], real db[2])
{
  vec2_minus(b, a, db);
  vec2_scale(2, db);
  vec2_negative(db, da);
  return CO_OK;
}

#define NOT_ZERO(x) if ((x) == 0) ERR(CO_NUM, "should not be zero");
int
dedg2_alpha(const real a[2], const real b[2], const real u[2],
            const real v[2], real du[2], real dv[2])
{
  real A, s;

  A = edg2_abs(a, b);
  NOT_ZERO(A);
  dedg2_abs(u, v, du, dv);
  s = 1 / A;
  vec2_scale(s, du);
  vec2_scale(s, dv);
  return CO_OK;
}

int
dedg2_strain(real Ka, real a3, real a4, const real a[2], const real b[2],
             const real u[2], const real v[2], real du[2], real dv[2])
{
  real l, s;

  l = edg2_alpha(a, b, u, v);
  s = (Ka * (4 * a4 * l * l * l + 3 * a3 * l * l + 2 * l)) / 2;
  dedg2_alpha(a, b, u, v, du, dv);
  vec2_scale(s, du);
  vec2_scale(s, dv);
  return CO_OK;
}
