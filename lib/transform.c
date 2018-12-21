#include <stdio.h>

#include "real.h"

#include "he/err.h"
#include "he/he.h"
#include "he/tri.h"
#include "he/sum.h"
#include "he/vec.h"

#include "he/transform.h"

struct Sum3 {
    HeSum *x, *y, *z;
    HeSum *a;
};
typedef struct Sum3 Sum3;

static int sum3_ini(Sum3 s) {
    he_sum_ini(&s.x);
    he_sum_ini(&s.y);
    he_sum_ini(&s.z);
    he_sum_ini(&s.a);
    return HE_OK;
}

static int sum3_fin(Sum3 s) {
    he_sum_fin(s.x);
    he_sum_fin(s.y);
    he_sum_fin(s.z);
    he_sum_fin(s.a);
    return HE_OK;
}

static int sum3_scalar_add(Sum3 s, real a, real r[3]) {
    enum {X, Y, Z};
    he_sum_add(s.x, a*r[X]);
    he_sum_add(s.y, a*r[Y]);
    he_sum_add(s.z, a*r[Z]);
    he_sum_add(s.a, a);
    return HE_OK;
}

static int sum3_get(Sum3 s, /**/ real r[3]) {
    enum {X, Y, Z};
    real a;
    a = he_sum_get(s.a);
    if (a == 0)
        ERR(HE_NUM, "a = 0");
    
    r[X] = he_sum_get(s.x)/a;
    r[Y] = he_sum_get(s.y)/a;
    r[Z] = he_sum_get(s.z)/a;

    return HE_OK;
}

int transform_centroid(He *he, const real *x, const real *y, const real *z, /**/ real com[3])
{
    int t, i, j, k, nt;
    real a[3], b[3], c[3], area;
    Sum3 s;

    nt = he_nt(he);
    sum3_ini(s);

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
    return HE_OK;
}

int transform_rotx(real rad, int n, /*io*/ real *x, real *y, real *z)
{
    return HE_OK;
}

int transform_roty(real rad, int n, /*io*/ real *x, real *y, real *z)
{
    return HE_OK;
}

int transform_rotz(real rad, int n, /*io*/ real *x, real *y, real *z)
{
    return HE_OK;
}

int transform_rot(const real rad[3], int n, /*io*/ real *x, real *y, real *z) {
    return HE_OK;
}

int transform_tranx(real s, int n, /*io*/ real *x, real *y, real *z) {
    return HE_OK;
}

int transform_trany(real s, int n, /*io*/ real *x, real *y, real *z) {
    return HE_OK;
}

int transform_tranz(real s, int n, /*io*/ real *x, real *y, real *z) {
    return HE_OK;
}

int transform_tran(const real s[3], int n, /*io*/ real *x, real *y, real *z) {
    return HE_OK;
}

int transform_scalx(real s, int n, /*io*/ real *x, real *y, real *z) {
    return HE_OK;
}

int transform_scaly(real s, int n, /*io*/ real *x, real *y, real *z) {
    return HE_OK;
}

int transform_scalz(real s, int n, /*io*/ real *x, real *y, real *z) {
    return HE_OK;
}

int transform_scal(const real s[3], int n, /*io*/ real *x, real *y, real *z) {
    return HE_OK;
}
