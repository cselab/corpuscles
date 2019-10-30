#include <tgmath.h>
#include <stdio.h>
#ifdef _OPENMP
#include <omp.h>
#endif
#include "real.h"
#include <cubtri.h>
#include "co/err.h"
#include "co/he.h"
#include "co/macro.h"
#include "co/memory.h"
#include "co/normal.h"
#include "co/oseen3tri.h"
#include "co/matrix.h"
#include "co/i/matrix.h"
#include "co/i/vec.h"
#include "co/i/tri.h"
#include "co/tri.h"
#include "co/i/area.h"

#define T Oseen3Tri
static const real pi = 3.141592653589793115997964;
struct T {
  real *nx, *ny, *nz, *area;
  Cubtri *integral;

};

static int
oseen(const real a[3], const real b[3],
      real * xx, real * xy, real * xz, real * yy, real * yz, real * zz)
{
  enum {
    X, Y, Z
  };
  real d[3], r, r3, l;

  i_vec_minus(a, b, d);
  r = i_vec_abs(d);
  if (r == 0)
    ERR(CO_NUM, "r == 0");
  r3 = r * r * r;
  l = 1 / r;
  *xx = l + d[X] * d[X] / r3;
  *yy = l + d[Y] * d[Y] / r3;
  *zz = l + d[Z] * d[Z] / r3;

  *xy = d[X] * d[Y] / r3;
  *xz = d[X] * d[Z] / r3;
  *yz = d[Y] * d[Z] / r3;
  return CO_OK;
}

int
oseen3_tri_ini(He * he, T ** pq)
{
  T *q;
  real *nx, *ny, *nz, *area;
  int n;

  MALLOC(1, &q);
  n = he_nv(he);
  MALLOC3(n, &nx, &ny, &nz);
  MALLOC(n, &area);
  cubtri_ini(&q->integral);
  q->nx = nx;
  q->ny = ny;
  q->nz = nz;
  q->area = area;
  *pq = q;
  return CO_OK;
}

int
oseen3_tri_fin(T * q)
{
  FREE3(q->nx, q->ny, q->nz);
  FREE(q->area);
  FREE(q);
  cubtri_fin(q->integral);
  return CO_OK;
}

#define SET(i, j, s, a) i_matrix_set(n, n, i, j, s, a)
#define ADD(i, j, s, a) i_matrix_add(n, n, i, j, s, a)

int
oseen3_tri_apply(T * q, He * he, const real * x, const real * y,
                 const real * z, real * oxx, real * oxy, real * oxz,
                 real * oyy, real * oyz, real * ozz)
{
  int n, i;
  real e, s;

  n = he_nv(he);
#pragma omp parallel for
  for (i = 0; i < n; i++) {
    real a[3], b[3], xx, xy, xz, yy, yz, zz;
    int j;

    i_vec_get(i, x, y, z, a);
    SET(i, i, 0, oxx);
    SET(i, i, 0, oxy);
    SET(i, i, 0, oxz);
    SET(i, i, 0, oyy);
    SET(i, i, 0, oyz);
    SET(i, i, 0, ozz);
    for (j = i + 1; j < n; j++) {
      i_vec_get(j, x, y, z, b);
      oseen(a, b, &xx, &xy, &xz, &yy, &yz, &zz);
      SET(i, j, xx, oxx);
      SET(i, j, xy, oxy);
      SET(i, j, xz, oxz);
      SET(i, j, yy, oyy);
      SET(i, j, yz, oyz);
      SET(i, j, zz, ozz);

      SET(j, i, xx, oxx);
      SET(j, i, xy, oxy);
      SET(j, i, xz, oxz);
      SET(j, i, yy, oyy);
      SET(j, i, yz, oyz);
      SET(j, i, zz, ozz);
    }
  }
  s = 1 / (8 * pi);
  i_matrix_scale(n, n, s, oxx);
  i_matrix_scale(n, n, s, oxy);
  i_matrix_scale(n, n, s, oxz);
  i_matrix_scale(n, n, s, oyy);
  i_matrix_scale(n, n, s, oyz);
  i_matrix_scale(n, n, s, ozz);
  return CO_OK;
}

static int
stresslet(const real a[3], const real n[3], const real b[3],
          real * xx, real * xy, real * xz, real * yy, real * yz, real * zz)
{
  enum {
    X, Y, Z
  };
  real d[3], r, p, l;

  i_vec_minus(a, b, d);
  r = i_vec_abs(d);
  if (r == 0)
    ERR(CO_NUM, "r == 0");
  p = i_vec_dot(d, n);
  l = p / (r * r * r * r * r);
  *xx = d[X] * d[X] * l;
  *xy = d[X] * d[Y] * l;
  *xz = d[X] * d[Z] * l;
  *yy = d[Y] * d[Y] * l;
  *yz = d[Y] * d[Z] * l;
  *zz = d[Z] * d[Z] * l;
  if (r > 0.5) {
    *xx = *xy = *xz = *yy = *yz = *zz = 1 / (r * r);
  } else {
    *xx = *xy = *xz = *yy = *yz = *zz = 0;
  }
  return CO_OK;
}

#define TSET(k, m)							\
    do {								\
	ADD(k, m, A*xx, oxx);						\
	ADD(k, m, A*xy, oxy);						\
	ADD(k, m, A*xz, oxz);						\
	ADD(k, m, A*yy, oyy);						\
	ADD(k, m, A*yz, oyz);						\
	ADD(k, m, A*zz, ozz);						\
    } while (0);							\


static real
F(real x, real y, real z, void *vp)
{
  enum { X, Y, Z };
  real *p, r;

  p = vp;
  x -= p[X];
  y -= p[Y];
  z -= p[Z];
  r = x * x + y * y + z * z;
  return r > 0.1 ? 1 / (r * r) : 0;
}

int
oseen3_tri_stresslet(T * q, He * he, const real * x, const real * y,
                     const real * z, real * oxx, real * oxy, real * oxz,
                     real * oyy, real * oyz, real * ozz)
{
  real *nx, *ny, *nz, *area, A, s;
  int status, n, nt, i;

  nx = q->nx;
  ny = q->ny;
  nz = q->nz;
  area = q->area;
  status = normal_mwa(he, x, y, z, nx, ny, nz);
  if (status != CO_OK)
    ERR(CO_NUM, "normal_mwa failed");
  status = i_area_ver(he, x, y, z, area);
  if (status != CO_OK)
    ERR(CO_NUM, "area_ver failed");
  n = he_nv(he);
  nt = he_nt(he);
  matrix_zero(n, n, oxx);
  matrix_zero(n, n, oxy);
  matrix_zero(n, n, oxz);
  matrix_zero(n, n, oyy);
  matrix_zero(n, n, oyz);
  matrix_zero(n, n, ozz);
#pragma omp parallel for
  for (i = 0; i < n; i++) {
    int j, ia, ib, ic;
    real a[3], b[3], c[3], ea[3], eb[3], ec[3];
    real point[3], center[3], normal[3];
    real xx, xy, xz, yy, yz, zz;

    if (i % 100 == 0)
      MSG("i: %04d/%04d", i, n);
    i_vec_get(i, x, y, z, point);
    for (j = 0; j < nt; j++) {
      he_tri_ijk(he, j, &ia, &ib, &ic);
      //if (ia == i || ib == i || ic == i)
      //  continue;
      i_vec_get3(ia, ib, ic, x, y, z, a, b, c);
      tri_center(a, b, c, center);
      tri_normal(a, b, c, normal);
      tri_edg_center(a, b, c, ea, eb, ec);
      A = tri_area(a, b, c) / (8 * pi);

      real res;

      cubtri_apply(q->integral, a, b, c, F, point, &res);
      i_matrix_add(n, n, i, ia, res, oxx);

      /* stresslet(point, normal, ea, &xx, &xy, &xz, &yy, &yz, &zz);
         TSET(i, ib); TSET(i, ic);
         stresslet(point, normal, eb, &xx, &xy, &xz, &yy, &yz, &zz);
         TSET(i, ia); TSET(i, ic);
         stresslet(point, normal, ec, &xx, &xy, &xz, &yy, &yz, &zz);
         TSET(i, ia); TSET(i, ib); */

      /*stresslet(point, normal, center, &xx, &xy, &xz, &yy, &yz, &zz);
         TSET(i, ia); TSET(i, ib); TSET(i, ic); */
    }
  }
  return CO_OK;
}
