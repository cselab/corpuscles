#include <stdio.h>

#include "real.h"
#include "he/memory.h"
#include "he/macro.h"
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

static real compute_energy(He *he, real *acos) {
    int n, m, h;
    real v;

    n = he_ne(he);
    v = 0;
    for (m = 0; m < n; m++) {
        h = he_hdg_edg(he, m);
        if (he_bnd(he, h)) continue;
        v += 1 - acos[m];
    }
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

enum {BULK, BND};
static int get_ijkl(int e, He *he, /**/ int *pi, int *pj, int *pk, int *pl) {
#    define  nxt(h)     he_nxt(he, h)
#    define  flp(h)     he_flp(he, h)
#    define  ver(h)     he_ver(he, h)
#    define  hdg_ver(v) he_hdg_ver(he, v)
#    define  hdg_edg(e) he_hdg_edg(he, e)
#    define  bnd(h)     he_bnd(he, h)
    int h, n, nn, nnf, i, j, k, l;
    h = he_hdg_edg(he, e);
    if (bnd(h)) return BND;

    h = hdg_edg(e); n = nxt(h); nn = nxt(nxt(h));
    nnf = nxt(nxt(flp(h)));
    j = ver(h); k = ver(n); i = ver(nn); l = ver(nnf);

    *pi = i; *pj = j; *pk = k; *pl = l;
    return BULK;
}

static int get(int e, He *he, const real *x, const real *y, const real *z,
               /**/ real a[3], real b[3], real c[3], real d[3]) {
    int status, i, j, k, l;
    status = get_ijkl(e, he, /**/ &i, &j, &k, &l);
    if (status == BND) return BND;
    vec_get(i, x, y, z, /**/ a);
    vec_get(j, x, y, z, /**/ b);
    vec_get(k, x, y, z, /**/ c);
    vec_get(l, x, y, z, /**/ d);
    return BULK;
}


static void compute_cos(He *he, const real *x, const real *y, const real *z, /**/ real *acos) {
    real a[3], b[3], c[3], d[3];
    int status, n, m;
    n = he_ne(he);
    for (m = 0; m < n; m++) {
        status = get(m, he, x, y, z, /**/ a, b, c, d);
        if (status == BND) continue;
        acos[m]  = dih_cos(a, b, c, d);
    }
}

static void compute_force(real K,
                          He *he, const real *x, const real *y, const real *z, /**/
                          real *fx, real *fy, real *fz) {
    int status, n, t, i, j, k, l;
    real a[3], b[3], c[3], d[3], da[3], db[3], dc[3], dd[3];
    n = he_ne(he);
    for (t = 0; t < n; t++) {
        status = get_ijkl(t, he, /**/ &i, &j, &k, &l);
        if (status == BND) continue;
        vec_get(i, x, y, z, /**/ a);
        vec_get(j, x, y, z, /**/ b);
        vec_get(k, x, y, z, /**/ c);
        vec_get(l, x, y, z, /**/ d);
        ddih_cos(a, b, c, d, /**/ da, db, dc, dd);
        vec_scalar_append(da, 2*K, i, /**/ fx, fy, fz);
        vec_scalar_append(db, 2*K, j, /**/ fx, fy, fz);
        vec_scalar_append(dc, 2*K, k, /**/ fx, fy, fz);
        vec_scalar_append(dd, 2*K, l, /**/ fx, fy, fz);
    }
}

int he_f_kantor_force(T *q, He *he,
                      const real *x, const real *y, const real *z, /**/
                      real *fx, real *fy, real *fz) {
    int n;
    real *acos, K;
    n = q->n;
    acos = q->acos;
    K  = q->K;
    if (he_ne(he) != n)
        ERR(HE_INDEX, "he_ne(he)=%d != n = %d", he_ne(he), n);
    compute_cos(he, x, y, z, /**/ acos);
    compute_force(K, he, x, y, z, /**/ fx, fy, fz);
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
    return 2*K*compute_energy(he, acos);
}

int he_f_kantor_energy_ver(__UNUSED T *q, /**/ __UNUSED real **pe) {
    ERR(HE_NOT, "he_f_kantor_energy_ver is not implimented");
}
