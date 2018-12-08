#include <stdio.h>

#include "real.h"
#include "he/memory.h"
#include "he/err.h"
#include "he/he.h"
#include "he/vec.h"
#include "he/tri.h"
#include "he/filter.h"

#define T Filter

struct T { real *B, *sum; };

int filter_ini(He *he, T **pq) {
    T *q;
    int n;
    MALLOC(1, &q);

    n = he_nv(he);
    MALLOC(n, &q->B);
    MALLOC(n, &q->sum);

    *pq = q;
    return HE_OK;
}

int filter_fin(T *q) {
    FREE(q->B);
    FREE(q->sum);
    FREE(q);
    return HE_OK;
}

static int zero(int n, real *a) {
    int i;
    for (i = 0; i < n; i++)
        a[i] = 0;
    return HE_OK;
}

static int div(int n, const real *a, const real *b, /*io*/ real *c) {
    int i;
    for (i = 0; i < n; i++) {
        if (b[i] == 0) ERR(HE_NUM, "b[%d] == 0", i);
        c[i] = a[i]/b[i];
    }
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
    real *B, *sum;
    real a[3], b[3], c[3];
    nv = he_nv(he);
    nt = he_nt(he);
    B = q->B;
    sum = q->sum;

    zero(nv, B);
    zero(nv, sum);

    for (t = 0; t < nt; t++) {
        he_tri_ijk(he, t, &i, &j, &k);
        get3(x, y, z, i, j, k, a, b, c);
        u = tri_angle(c, a, b);
        v = tri_angle(a, b, c);
        w = tri_angle(b, c, a);
        B[i] += u*A[i];
        B[j] += v*A[j];
        B[k] += w*A[k];

        sum[i] += u;
        sum[j] += v;
        sum[k] += w;
    }

    div(nv, B, sum, A);
    return HE_OK;
}
