#include <stdio.h>

#include "real.h"
#include "he/memory.h"
#include "he/err.h"
#include "he/he.h"
#include "he/vec.h"
#include "he/edg.h"
#include "he/dedg.h"
#include "he/strain.h"
#include "he/f/strain.h"

#define T HeFStrain

struct T {
    real *x, *y, *z, *eng;
    Strain *strain;
};

static void get_ij(int m, He *he, /**/ int *pi, int *pj) {
    int h, n, i, j;
    h = he_hdg_edg(he, m);
    n = he_nxt(he, h);
    i = he_ver(he, h); j = he_ver(he, n);
    *pi = i; *pj = j;
}
static void get(int m, He *he,
                const real *x, const real *y, const real *z, /**/
                real a[3], real b[3]) {
    int i, j;
    get_ij(m, he, &i, &j);
    vec_get(i, x, y, z, /**/ a);
    vec_get(j, x, y, z, /**/ b);
}

static real sum_sq_norm(int n, real *a, const real *e0) {
    int i;
    real v;
    v = 0;
    for (i = 0; i < n; i++) v += a[i]*a[i]/e0[i];
    return v;
}

int he_f_strain_ini(const char *name, StrainParam param, const real *x, const real *y, const real *z, He *he, T **pq) {
    T *q;
    int nv, i;
    
    nv = he_nv(he);
    MALLOC(1, &q);
    MALLOC(nv, &q->x);
    MALLOC(nv, &q->y);
    MALLOC(nv, &q->z);
    MALLOC(nv, &q->eng);
    strain_ini(name, param, &q->strain);

    for (i = 0; i < nv; i++) {
        q->x[i] = x[i];
        q->y[i] = y[i];
        q->z[i] = z[i];
    }

    *pq = q;
    return HE_OK;
}

int he_f_strain_fin(T *q) {
    strain_fin(q->strain);
    FREE(q->x); FREE(q->y); FREE(q->z);
    FREE(q->eng);
    FREE(q);
    return HE_OK;
}

static void compute_edg(He *he, const real *e0, const real *x, const real *y, const real *z, /**/ real *edg, real *dedg) {
    real one, a[3], b[3];
    int n, m;
    n = he_ne(he);
    for (m = 0; m < n; m++) {
        get(m, he, x, y, z, /**/ a, b);
        edg[m]  = one = edg_abs(a, b);
        dedg[m] = one - e0[m];
    }
}

static void compute_force(const real *e0, real K, real *dedg,
                          He *he, const real *x, const real *y, const real *z, /**/
                          real *fx, real *fy, real *fz) {
    int n, m, i, j;
    real a[3], b[3], da[3], db[3], coeff;
    n = he_ne(he);
    for (m = 0; m < n; m++) {
        get_ij(m, he, /**/ &i, &j);
        vec_get(i, x, y, z, /**/ a);
        vec_get(j, x, y, z, /**/ b);
        dedg_abs(a, b, /**/ da, db);

        coeff = 2*K*dedg[m]/e0[m];

        vec_scalar_append(da, coeff, i, /**/ fx, fy, fz);
        vec_scalar_append(db, coeff, j, /**/ fx, fy, fz);
    }
}

int he_f_strain_force(T *q, He *he,
                            const real *x, const real *y, const real *z, /**/
                            real *fx, real *fy, real *fz) {
    return HE_OK;
}

real he_f_strain_energy(T *q, He *he,
                              const real *x, const real *y, const real *z) {
    int nt, t;

    nt = he_nt(he);
    for (t = 0; t < nt; t++) {
        
    }
    
    return 0.0;
}


int he_f_strain_energy_ver(T *q, /**/ real **pa) {
    *pa = q->eng;
    return HE_OK;
}
