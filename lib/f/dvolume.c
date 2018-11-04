#include <stdio.h>

#include "real.h"
#include "he/memory.h"
#include "he/err.h"
#include "he/he.h"
#include "he/vec.h"
#include "he/tri.h"
#include "he/dtri.h"
#include "he/sum.h"
#include "he/f/dvolume.h"

#define T HeFDvolume

struct T {
    int n;
    real *volume;
};

static real sum(int n, real *a) {
    int i;
    real v;
    HeSum *sum;
    he_sum_ini(&sum);
    for (i = 0; i < n; i++)
        he_sum_add(sum, a[i]);
    v = he_sum_get(sum);
    he_sum_fin(sum);
    return v;
}

int he_f_dvolume_ini(He *he, T **pq) {
    T *q;
    int n;
    MALLOC(1, &q);
    
    n = he_nt(he);
    MALLOC(n, &q->volume);
    q->n = n;

    *pq = q;
    return HE_OK;
}

int he_f_dvolume_fin(T *q) {
    FREE(q->volume); FREE(q);
    return HE_OK;
}

int he_f_dvolume_v(T *q, /**/ real  **pa) {
    *pa = q->volume;
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
                const real *x, const real *y, const real *z, /**/
                real a[3], real b[3], real c[3]) {
    int i, j, k;
    get_ijk(t, he, &i, &j, &k);
    vec_get(i, x, y, z, /**/ a);
    vec_get(j, x, y, z, /**/ b);
    vec_get(k, x, y, z, /**/ c);
}
static void compute_volume(He *he, const real *x, const real *y, const real *z, /**/ real *volume) {
    real a[3], b[3], c[3];
    int n, t;
    n = he_nt(he);
    for (t = 0; t < n; t++) {
        get(t, he, x, y, z, /**/ a, b, c);
        volume[t]  = tri_volume(a, b, c);
    }
}

static void compute_force(real v,
                          He *he, const real *x, const real *y, const real *z, /**/
                          real *fx, real *fy, real *fz) {
    int n, t, i, j, k;
    real a[3], b[3], c[3], da[3], db[3], dc[3], coeff;
    n = he_nt(he);
    for (t = 0; t < n; t++) {
        get_ijk(t, he, /**/ &i, &j, &k);
        vec_get(i, x, y, z, /**/ a);
        vec_get(j, x, y, z, /**/ b);
        vec_get(k, x, y, z, /**/ c);
        dtri_volume(a, b, c, /**/ da, db, dc);
        vec_append(da, i, /**/ fx, fy, fz);
        vec_append(db, j, /**/ fx, fy, fz);
        vec_append(dc, k, /**/ fx, fy, fz);
    }
}

int he_f_dvolume_force(T *q, He *he,
                      const real *x, const real *y, const real *z, /**/
                      real *fx, real *fy, real *fz) {
    int n;
    real *volume, v;
    n = q->n;
    volume = q->volume;
    if (he_nt(he) != n)
        ERR(HE_INDEX, "he_nt(he)=%d != n = %d", he_nt(he), n);
    compute_volume(he, x, y, z, /**/ volume);
    v = sum(n, volume);
    compute_force(v, he, x, y, z, /**/ fx, fy, fz);
    return HE_OK;
}

real he_f_dvolume_energy(T *q, He *he,
                         const real *x, const real *y, const real *z) {
    int n;
    real *volume;
    n = q->n;
    volume = q->volume;

    if (he_nt(he) != n)
        ERR(HE_INDEX, "he_nt(he)=%d != n = %d", he_nt(he), n);

    compute_volume(he, x, y, z, /**/ volume);
    return sum(n, volume);
}
