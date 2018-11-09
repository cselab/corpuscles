#include <stdio.h>

#include "real.h"
#include "he/memory.h"
#include "he/err.h"
#include "he/he.h"
#include "he/vec.h"
#include "he/edg.h"
#include "he/dedg.h"

#include "he/f/edg_sq.h"

#define T HeFEdgSq

struct T {
    int ne;
    real *edg;
    int *rank;
    real K;
};

static real sum_sq(int n, real *a) {
    int i;
    real v;
    v = 0;
    for (i = 0; i < n; i++) v += a[i]*a[i];
    return v;
}
int he_f_edg_sq_ini(real K, He *he, T **pq) {
    T *q;
    int nv, ne;
    MALLOC(1, &q);
    ne = he_ne(he);
    nv = he_nv(he);

    MALLOC(ne, &q->edg);
    MALLOC(nv, &q->rank);

    q->ne = ne;
    q->K = K;

    *pq = q;
    return HE_OK;
}

int he_f_edg_sq_fin(T *q) {
    FREE(q->edg);
    FREE(q->rank);
    FREE(q);
    return HE_OK;
}

int he_f_edg_sq_e(T *q, /**/ real  **pa) {
    *pa = q->edg;
    return HE_OK;
}

static int get_ij(int m, He *he, /**/ int *pi, int *pj) {
    int h, n, i, j;
    h = he_hdg_edg(he, m);
    n = he_nxt(he, h);
    i = he_ver(he, h); j = he_ver(he, n);
    *pi = i; *pj = j;
    return HE_OK;
}
static int get(int m, He *he,
                const real *x, const real *y, const real *z, /**/
                real a[3], real b[3]) {
    int i, j;
    get_ij(m, he, &i, &j);
    vec_get(i, x, y, z, /**/ a);
    vec_get(j, x, y, z, /**/ b);
    return HE_OK;
}
static int compute_edg(He *he, const real *x, const real *y, const real *z, /**/ real *edg) {
    real one, a[3], b[3];
    int n, m;
    n = he_ne(he);
    for (m = 0; m < n; m++) {
        get(m, he, x, y, z, /**/ a, b);
        edg[m]  = one = edg_abs(a, b);
    }
    return HE_OK;
}

static int compute_force(He *he, real K, const real *edg,
                         const real *x, const real *y, const real *z, /**/
                         real *fx, real *fy, real *fz) {
    int n, m, i, j;
    real a[3], b[3], da[3], db[3], coeff;
    n = he_ne(he);
    for (m = 0; m < n; m++) {
        get_ij(m, he, /**/ &i, &j);
        vec_get(i, x, y, z, /**/ a);
        vec_get(j, x, y, z, /**/ b);
        dedg_abs(a, b, /**/ da, db);
        coeff = 2*K*edg[m];
        vec_scalar_append(da, coeff, i, /**/ fx, fy, fz);
        vec_scalar_append(db, coeff, j, /**/ fx, fy, fz);
    }
    return HE_OK;
}

int he_f_edg_sq_force(T *q, He *he,
                      const real *x, const real *y, const real *z, /**/
                      real *fx, real *fy, real *fz) {
    int ne;
    real *edg, K;
    ne = q->ne;
    edg = q->edg;
    K  = q->K;
    if (he_ne(he) != ne)
        ERR(HE_INDEX, "he_ne(he)=%d != ne = %d", he_ne(he), ne);
    compute_edg(he, x, y, z, /**/ edg);
    compute_force(he, K, edg, x, y, z, /**/ fx, fy, fz);
    return HE_OK;
}

real he_f_edg_sq_energy(T *q, He *he,
                      const real *x, const real *y, const real *z) {
    int ne;
    real *edg, v, K;
    ne = q->ne;
    edg = q->edg;
    K  = q->K;

    if (he_ne(he) != ne)
        ERR(HE_INDEX, "he_ne(he)=%d != ne = %d", he_ne(he), ne);

    compute_edg(he, x, y, z, /**/ edg);
    v = sum_sq(ne, edg);
    return K*v;
}
