#include <stdio.h>

#include "real.h"
#include "he/memory.h"
#include "he/err.h"
#include "he/he.h"
#include "he/vec.h"
#include "he/tri.h"
#include "he/filter.h"

#define T Filter

struct T { real *B; };

int filter_ini(He *he, T **pq) {
    T *q;
    int n;
    MALLOC(1, &q);

    n = he_nv(he);
    MALLOC(n, &q->B);

    *pq = q;
    return HE_OK;
}

int filter_fin(T *q) {
    FREE(q->B);
    FREE(q);
    return HE_OK;
}

static int zero(int n, real *a) {
    int i;
    for (i = 0; i < n; i++)
        a[i] = 0;
    return HE_OK;
}

static int copy(int n, const real *a, real *b) {
    int i;
    for (i = 0; i < n; i++)
        b[i] = a[i];
    return HE_OK;
}

static int get3(const real *x, const real *y, const real *z,
                int i, int j, int k,  /**/
                real a[3], real b[3], real c[3]) {
    vec_get(i, x, y, z, /**/ a);
    vec_get(j, x, y, z, /**/ b);
    vec_get(k, x, y, z, /**/ c);
    return HE_OK;
}

int filter_apply(T *q, He *he,
                 const real *x, const real *y, const real *z,
                 /*io*/ real *A) {
    int nv, nt, t, i, j, k;
    real u, v, w;
    real *B;
    real a[3], b[3], c[3];
    nv = he_nv(he);
    nt = he_nt(he);
    B = q->B;
    zero(nv, B);
    for (t = 0; t < nt; t++) {
        he_tri_ijk(he, t, &i, &j, &k);
        get3(x, y, z, i, j, k, a, b, c);
        u = tri_angle(c, a, b);
        v = tri_angle(a, b, c);
        w = tri_angle(b, c, a);
        B[i] += u*a[i];
        B[j] += v*a[j];
        B[k] += w*a[k];
    }
    copy(nv, B, A);
    return HE_OK;
}
