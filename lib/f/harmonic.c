#include <stdio.h>

#include "real.h"
#include "he/memory.h"
#include "he/err.h"
#include "he/he.h"
#include "he/vec.h"
#include "he/tri.h"
#include "he/dtri.h"

#include "he/f/harmonic.h"

#define T HeFHarmonic

struct T {
    int n;
    real *edg, *dedg;
    real v0, K;
};

static real sum_sq(int n, real *a) {
    int i;
    real v;
    v = 0;
    for (i = 0; i < n; i++) v += a[i]*a[i];
    return v;
}
int he_f_harmonic_ini(real v0, real K, He *he, T **pq) {
    T *q;
    int n;
    MALLOC(1, &q);
    n = he_ne(he);

    MALLOC(n, &q->edg);
    MALLOC(n, &q->dedg);

    q->n = n;
    q->v0 = v0;
    q->K = K;

    *pq = q;
    return HE_OK;
}

int he_f_harmonic_fin(T *q) {
    FREE(q->edg); FREE(q->dedg); FREE(q);
    return HE_OK;
}

int he_f_harmonic_e(T *q, /**/ real  **pa) {
    *pa = q->edg;
    return HE_OK;
}

static void get_ij(int t, He *he, /**/ int *pi, int *pj) {
    int h, n, i, j;
    h = he_hdg_tri(he, t);
    n = he_nxt(he, h);
    i = he_ver(he, h); j = he_ver(he, n);
    *pi = i; *pj = j;
}
static void get(int t, He *he,
                const real *x, const real *y, const real *z, /**/
                real a[3], real b[3]) {
    int i, j;
    get_ij(t, he, &i, &j);
    vec_get(i, x, y, z, /**/ a);
    vec_get(j, x, y, z, /**/ b);
}
static void compute_edg(He *he, const real *x, const real *y, const real *z, /**/ real *edg, real *dedg) {
    real a[3], b[3];
    int n, m;
    n = he_nt(he);
    for (m = 0; m < n; m++) {
        get(m, he, x, y, z, /**/ a, b);
        //dedg[t]  = tri_harmonic(a, b, c);
    }
}

static void compute_force(real v0, real K, real v,
                          He *he, const real *x, const real *y, const real *z, /**/
                          real *fx, real *fy, real *fz) {
    int n, t, i, j;
    real a[3], b[3], da[3], db[3], coeff;
    n = he_nt(he);
    coeff = 2*K/v0*(v - v0);
    for (t = 0; t < n; t++) {
        get_ij(t, he, /**/ &i, &j);
        vec_get(i, x, y, z, /**/ a);
        vec_get(j, x, y, z, /**/ b);
        /* dtri_edg(a, b, da, db); */
        vec_scalar_append(da, coeff, i, /**/ fx, fy, fz);
        vec_scalar_append(db, coeff, j, /**/ fx, fy, fz);
    }
}

int he_f_harmonic_force(T *q, He *he,
                      const real *x, const real *y, const real *z, /**/
                      real *fx, real *fy, real *fz) {
    int n;
    real *edg, *dedg, v0, K, v;
    n = q->n;
    edg = q->edg;
    dedg = q->dedg;
    K  = q->K;
    v0 = q->v0;
    if (he_nt(he) != n)
        ERR(HE_INDEX, "he_nt(he)=%d != n = %d", he_nt(he), n);
    compute_edg(he, x, y, z, /**/ edg, dedg);
    v = sum_sq(n, dedg);
    compute_force(v0, K, v, he, x, y, z, /**/ fx, fy, fz);
    return HE_OK;
}

real he_f_harmonic_energy(T *q, He *he,
                      const real *x, const real *y, const real *z) {
    int n;
    real *edg, *dedg, v0, v, K;
    n = q->n;
    edg = q->edg;
    dedg = q->dedg;    
    v0 = q->v0;
    K  = q->K;

    if (he_nt(he) != n)
        ERR(HE_INDEX, "he_nt(he)=%d != n = %d", he_nt(he), n);

    compute_edg(he, x, y, z, /**/ edg, dedg);
    v = sum_sq(n, dedg);

    return K*v;
}
