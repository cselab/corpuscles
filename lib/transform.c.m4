include(`co.m4')dnl
#include <stdio.h>
#include <math.h>

#include "real.h"

#include "co/array.h"
#include "co/err.h"
#include "co/he.h"
#include "co/sum.h"
#include "co/tri.h"
#include "co/vec.h"

#include "co/transform.h"

enum {X, Y, Z};
struct Sum3 {
    HeSum *x, *y, *z;
    HeSum *a;
};
typedef struct Sum3 Sum3;

static int sum3_ini(Sum3 *s) {
    he_sum_ini(&s->x);
    he_sum_ini(&s->y);
    he_sum_ini(&s->z);
    he_sum_ini(&s->a);
    return CO_OK;
}

static int sum3_fin(Sum3 s) {
    he_sum_fin(s.x);
    he_sum_fin(s.y);
    he_sum_fin(s.z);
    he_sum_fin(s.a);
    return CO_OK;
}

static int sum3_scalar_add(Sum3 s, real a, const real r[3]) {
    he_sum_add(s.x, a*r[X]);
    he_sum_add(s.y, a*r[Y]);
    he_sum_add(s.z, a*r[Z]);
    he_sum_add(s.a, a);
    return CO_OK;
}

static int sum3_get(Sum3 s, /**/ real r[3]) {
    enum {X, Y, Z};
    real a;
    a = he_sum_get(s.a);
    if (a == 0)
        ERR(CO_NUM, "a = 0");
    
    r[X] = he_sum_get(s.x)/a;
    r[Y] = he_sum_get(s.y)/a;
    r[Z] = he_sum_get(s.z)/a;

    return CO_OK;
}

int transform_centroid(He *he, const real *x, const real *y, const real *z, /**/ real com[3])
{
    int t, i, j, k, nt;
    real a[3], b[3], c[3], area;
    Sum3 s;

    nt = he_nt(he);
    sum3_ini(&s);

    for (t = 0; t < nt; t++) {
        he_tri_ijk(he, t, &i, &j, &k);
        vec_get(i, x, y, z, /**/ a);
        vec_get(j, x, y, z, /**/ b);
        vec_get(k, x, y, z, /**/ c);
        area = tri_area(a, b, c);

        sum3_scalar_add(s, area, a);
        sum3_scalar_add(s, area, b);
        sum3_scalar_add(s, area, c);
    }
    sum3_get(s, com);
    sum3_fin(s);
    return CO_OK;
}

h_define(`ROT', `
int transform_rot$1(real rad, int n, /*io*/ real *x, real *y, real *z)
{
    enum $2;
    real c, s, p, q, r[3];
    int i;
    c = cos(rad); s = sin(rad);
    for (i = 0; i < n; i++) {
        vec_get(i, x, y, z, r);
        p = r[P]; q = r[Q];
        r[P] = c*p - s*q;
        r[Q] = s*p + c*q;
        vec_set(r, i, x, y, z);
    }
    return CO_OK;
}')
ROT(`x', `{P = Y, Q = Z}')dnl
ROT(`y', `{P = Z, Q = X}')dnl
ROT(`z', `{P = X, Q = Y}')dnl

h_define(`TRAN', `
int transform_tran$1(real s, int n, /*io*/ real *x, real *y, real *z) {
    int i;
    for (i = 0; i < n; i++)
        $1[i] += s;
    return CO_OK;
}')
TRAN(`x')dnl
TRAN(`y')dnl
TRAN(`z')dnl

int transform_tran(const real s[3], int n, /*io*/ real *x, real *y, real *z) {
    enum {X, Y, Z};
    transform_tranx(s[X], n, x, y, z);
    transform_trany(s[Y], n, x, y, z);
    transform_tranz(s[Z], n, x, y, z);
    return CO_OK;
}

h_define(`SCAL', `
int transform_scal$1(real s, int n, /*io*/ real *x, real *y, real *z) {
    int i;
    for (i = 0; i < n; i++)
        $1[i] *= s;
    return CO_OK;
}')
SCAL(`x')dnl
SCAL(`y')dnl
SCAL(`z')dnl

int transform_scal(const real s[3], int n, /*io*/ real *x, real *y, real *z) {
    enum {X, Y, Z};
    transform_scalx(s[X], n, x, y, z);
    transform_scaly(s[Y], n, x, y, z);
    transform_scalz(s[Z], n, x, y, z);
    return CO_OK;
}

int transform_center(int n, /*io*/ real * x, real * y, real * z)
{
  enum {X, Y, Z};
  real r[3];
  
  r[X] = -array_mean(n, x);
  r[Y] = -array_mean(n, y);
  r[Z] = -array_mean(n, z);
  transform_tran(r, n, x, y, z);
  return CO_OK;
}
