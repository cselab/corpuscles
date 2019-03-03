#include <tgmath.h>
#include <stdio.h>

#include "real.h"
#include "co/err.h"
#include "co/he.h"
#include "co/vec.h"
#include "co/tri.h"
#include "co/sum.h"

#include "co/area.h"

static void get(int t, He *he,
                const real *x, const real *y, const real *z,
                /**/ real a[3], real b[3], real c[3]) {
    int h;
    int i, j, k;
    h = he_hdg_tri(he, t);
    he_ijk(he, h, &i, &j, &k);
    vec_get(i, x, y, z, /**/ a);
    vec_get(j, x, y, z, /**/ b);
    vec_get(k, x, y, z, /**/ c);
}

real he_area(He *he, const real *x, const real *y, const real *z) {
    int n, m;
    real s;
    real a[3], b[3], c[3];
    HeSum *sum;

    n = he_nt(he);
    he_sum_ini(&sum);
    for (m = 0; m < n; m++) {
        get(m, he, x, y, z, /**/ a, b, c);
        s = tri_area(a, b, c);
        he_sum_add(sum, s);
    }

    s = he_sum_get(sum);
    he_sum_fin(sum);
    return s;
}

static int get3(const real *x, const real *y, const real *z,
                int i, int j, int k,  /**/
                real a[3], real b[3], real c[3]) {
    vec_get(i, x, y, z, /**/ a);
    vec_get(j, x, y, z, /**/ b);
    vec_get(k, x, y, z, /**/ c);
    return CO_OK;
}
static void zero(int n, real *a) {
    int i;
    for (i = 0; i < n; i++) a[i] = 0;
}
int he_area_ver(He *he, const real *x, const real *y, const real *z, /**/ real *area) {
    int nv, nt, t;
    int i, j, k;
    real area0;
    real a[3], b[3], c[3];

    nt = he_nt(he);
    nv = he_nv(he);

    zero(nv, area);
    for (t = 0; t < nt; t++) {
        he_tri_ijk(he, t, &i, &j, &k);
        get3(x, y, z, i, j, k, a, b, c);
        area0 = tri_area(a, b, c)/3;
        area[i] += area0;
        area[j] += area0;
        area[k] += area0;
    }
    return CO_OK;
}

int he_area_tri(He *he, const real *x, const real *y, const real *z, /**/ real *area) {
    int n, m;
    real a[3], b[3], c[3];

    n = he_nt(he);
    for (m = 0; m < n; m++) {
        get(m, he, x, y, z, /**/ a, b, c);
        area[m] = tri_area(a, b, c);
    }

    return CO_OK;
}
