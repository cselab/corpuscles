#include <stdio.h>

#include "real.h"
#include "he/memory.h"
#include "he/err.h"
#include "he/he.h"
#include "he/vec.h"
#include "he/dih.h"
#include "he/ddih.h"

#include "he/f/kantor.h"

#define T HeFKantor

struct T {
    int n;
    real *acos;
    real K;
};

static real compute_energy(int n, real *acos) {
    int m;
    real v;
    for (m = 0; m < n; m++)
        v += 1 - acos[m];
    return v;
}
int he_f_kantor_ini(real K, He *he, T **pq) {
    T *q;
    int n;
    MALLOC(1, &q);
    n = he_ne(he);

    MALLOC(n, &q->acos);

    q->n = n;
    q->K = K;

    *pq = q;
    return HE_OK;
}

int he_f_kantor_fin(T *q) {
    FREE(q->acos); FREE(q);
    return HE_OK;
}

int he_f_kantor_cos(T *q, /**/ real  **pa) {
    *pa = q->acos;
    return HE_OK;
}
static void get_ijkl(int t, He *he, /**/ int *pi, int *pj, int *pk, int *pl) {
    int h, n, nn, i, j, k, l;
    h = he_hdg_tri(he, t);
    n = he_nxt(he, h);
    nn = he_nxt(he, n);
    i = he_ver(he, h); j = he_ver(he, n); k = he_ver(he, nn);
    *pi = i; *pj = j; *pk = k; *pl = l;
}
static void get(int t, He *he,
                const real *x, const real *y, const real *z, /**/
                real a[3], real b[3], real c[3], real d[3]) {
    int i, j, k, l;
    get_ijkl(t, he, &i, &j, &k, &l);
    vec_get(i, x, y, z, /**/ a);
    vec_get(j, x, y, z, /**/ b);
    vec_get(k, x, y, z, /**/ c);
    vec_get(l, x, y, z, /**/ d);
}
static void compute_cos(He *he, const real *x, const real *y, const real *z, /**/ real *acos) {
    real a[3], b[3], c[3], d[3];
    int n, m;
    n = he_ne(he);
    for (m = 0; m < n; m++) {
        get(m, he, x, y, z, /**/ a, b, c, d);
        acos[m]  = dih_cos(a, b, c, d);
    }
}

static void compute_force(real v0, real K, real v,
                          He *he, const real *x, const real *y, const real *z, /**/
                          real *fx, real *fy, real *fz) {
    int n, t, i, j, k, l;
    real a[3], b[3], c[3], d[3], da[3], db[3], dc[3], dd[3], coeff;
    n = he_ne(he);
    coeff = 2*K/v0*(v - v0);
    for (t = 0; t < n; t++) {
        get_ijkl(t, he, /**/ &i, &j, &k, &l);
        vec_get(i, x, y, z, /**/ a);
        vec_get(j, x, y, z, /**/ b);
        vec_get(k, x, y, z, /**/ c);
        ddih_cos(a, b, c, d, /**/ da, db, dc, dd);
        vec_scalar_append(da, coeff, i, /**/ fx, fy, fz);
        vec_scalar_append(db, coeff, j, /**/ fx, fy, fz);
        vec_scalar_append(dc, coeff, k, /**/ fx, fy, fz);
    }
}

int he_f_kantor_force(T *q, He *he,
                      const real *x, const real *y, const real *z, /**/
                      real *fx, real *fy, real *fz) {
    int n;
    real *acos, v0, K, v;
    n = q->n;
    acos = q->acos;
    K  = q->K;
    if (he_ne(he) != n)
        ERR(HE_INDEX, "he_ne(he)=%d != n = %d", he_ne(he), n);
    compute_cos(he, x, y, z, /**/ acos);
    compute_force(v0, K, v, he, x, y, z, /**/ fx, fy, fz);
    return HE_OK;
}

real he_f_kantor_energy(T *q, He *he,
                      const real *x, const real *y, const real *z) {
    int n;
    real *acos, K;
    n = q->n;
    acos = q->acos;
    K  = q->K;

    if (he_ne(he) != n)
        ERR(HE_INDEX, "he_ne(he)=%d != n = %d", he_ne(he), n);

    compute_cos(he, x, y, z, /**/ acos);
    return 2*K*compute_energy(n, acos);
}
