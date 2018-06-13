#include <stdio.h>

#include "real.h"
#include "he/memory.h"
#include "he/err.h"
#include "he/he.h"
#include "he/vec.h"
#include "he/tri.h"
#include "he/dtri.h"

#include "he/f/volume.h"

#define T HeFVolume

struct T {
    int n;
    real *volume, *dvolume;
    real a0, K;
};

int he_f_volume_ini(real a0, real K, He *he, T **pq) {
    T *q;
    int n;
    MALLOC(1, &q);
    n = he_nt(he);

    MALLOC(n, &q->dvolume);
    MALLOC(n, &q->volume);

    q->n = n;
    q->a0 = a0;
    q->K = K;

    *pq = q;
    return HE_OK;
}

int he_f_volume_fin(T *q) {
    FREE(q->volume); FREE(q->dvolume); FREE(q);
    return HE_OK;
}

int he_f_volume_a(T *q, /**/ real  **pa) {
    *pa = q->volume;
    return HE_OK;
}

int he_f_volume_da(T *q, /**/ real  **pa) {
    *pa = q->dvolume;
    return HE_OK;
}

static void get_ijk(int t, He *he, /**/ int *pi, int *pj, int *pk) {
    int h, n, nn, i, j, k;
    h = he_hdg_tri(he, t);
    n = he_nxt(he, h);
    nn = he_nxt(he, n);
    i = he_ver(he, h); j = he_ver(he, n); k = he_ver(he, nn);
    *pi = i; *pj = j; *pk = k;
}
static void get(int t, He *he,
                real *x, real *y, real *z, /**/
                real a[3], real b[3], real c[3]) {
    int i, j, k;
    get_ijk(t, he, &i, &j, &k);
    vec_get(i, x, y, z, /**/ a);
    vec_get(j, x, y, z, /**/ b);
    vec_get(k, x, y, z, /**/ c);
}
static void compute_volume(real a0, He *he, real *x, real *y, real *z, /**/ real *volume, real *dvolume) {
    real one, delta, a[3], b[3], c[3];
    int n, t;
    n = he_nt(he);
    for (t = 0; t < n; t++) {
        get(t, he, x, y, z, /**/ a, b, c);
        volume[t]  = one   = tri_volume(a, b, c);
        dvolume[t] = delta = a0 - one;
    }
}

static void compute_force(real K, real *dvolume, He *he, real *x, real *y, real *z, /**/ real *fx, real *fy, real *fz) {
    int n, t, i, j, k;
    real a[3], b[3], c[3], da[3], db[3], dc[3], coeff;
    n = he_nt(he);
    for (t = 0; t < n; t++) {
        get_ijk(t, he, /**/ &i, &j, &k);
        vec_get(i, x, y, z, /**/ a);
        vec_get(j, x, y, z, /**/ b);
        vec_get(k, x, y, z, /**/ c);

        dtri_volume(a, b, c, /**/ da, db, dc);
        coeff = 2*K*dvolume[t];
        vec_scalar_append(da, coeff, i, /**/ fx, fy, fz);
        vec_scalar_append(db, coeff, j, /**/ fx, fy, fz);
        vec_scalar_append(dc, coeff, k, /**/ fx, fy, fz);
    }
}

int he_f_volume_force(T *q, He *he,
                      real *x, real *y, real *z, /**/
                      real *fx, real *fy, real *fz) {
    int n;
    real *volume, *dvolume, a0, K;
    n = q->n;
    volume = q->volume;
    dvolume = q->dvolume;
    a0 = q->a0;
    K  = q->K;

    if (he_nt(he) != n)
        ERR(HE_INDEX, "he_nt(he)=%d != n = %d", he_nt(he), n);

    compute_volume(a0, he, x, y, z, /**/ volume, dvolume);
    compute_force(K, dvolume, he, x, y, z, /**/ fx, fy, fz);

    return HE_OK;
}

static real compute_energy(real K, real *dvolume, int n) {
    int t;
    real da, e;
    e = 0;
    for (t = 0; t < n; t++) {
        da = dvolume[t];
        e += da * da;
    }
    return K * e;
}

real he_f_volume_energy(T *q, He *he,
                      real *x, real *y, real *z) {
    int n;
    real *volume, *dvolume, a0, K;
    n = q->n;
    volume = q->volume;
    dvolume = q->dvolume;
    a0 = q->a0;
    K  = q->K;

    if (he_nt(he) != n)
        ERR(HE_INDEX, "he_nt(he)=%d != n = %d", he_nt(he), n);

    compute_volume(a0, he, x, y, z, /**/ volume, dvolume);
    return compute_energy(K, dvolume, n);
}
