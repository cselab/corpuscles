#include <stdio.h>

#include "real.h"
#include "he/memory.h"
#include "he/err.h"
#include "he/he.h"
#include "he/vec.h"
#include "he/dih.h"
#include "he/ddih.h"

#include "he/f/gompper.h"

#define T HeFGompper

struct T {
    int n;
    real *area;
    real *lx, *ly, *lz;

    real *l2, *t;
    real K;
};

int he_f_gompper_ini(real K, He *he, T **pq) {
    T *q;
    int n;
    MALLOC(1, &q);
    n = he_nv(he);

    MALLOC(n, &q->area);
    MALLOC(n, &q->l2);
    MALLOC(n, &q->t);

    MALLOC(n, &q->lx);
    MALLOC(n, &q->ly);
    MALLOC(n, &q->lz);
    MALLOC(n, &q->lz);

    q->n = n;
    q->K = K;

    *pq = q;
    return HE_OK;
}

int he_f_gompper_fin(T *q) {
    FREE(q->area);
    FREE(q->lx); FREE(q->ly); FREE(q->lz);
    FREE(q->t); FREE(q->l2);
    FREE(q);
    return HE_OK;
}

int he_f_gompper_area(T *q, /**/ real **pa) {
    *pa = q->area;
    return HE_OK;
}

int he_f_gompper_laplace(T *q, /**/ real **px, real **py, real **pz ) {
    *px = q->lx;
    *py = q->ly;
    *pz = q->lz;
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

static void compute_force(real K,
                          He *he, const real *x, const real *y, const real *z, /**/
                          real *fx, real *fy, real *fz) {
    int status, n, t, i, j, k, l;
    real a[3], b[3], c[3], d[3], da[3], db[3], dc[3], dd[3];
    n = he_nv(he);
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

int he_f_gompper_force(T *q, He *he,
                      const real *x, const real *y, const real *z, /**/
                      real *fx, real *fy, real *fz) {
    int n;
    real K;
    n = q->n;
    K  = q->K;
    if (he_nv(he) != n)
        ERR(HE_INDEX, "he_nv(he)=%d != n = %d", he_nv(he), n);
    compute_force(K, he, x, y, z, /**/ fx, fy, fz);
    return HE_OK;
}

real he_f_gompper_energy(T *q, He *he,
                      const real *x, const real *y, const real *z) {
    int n;
    real K;
    n = q->n;
    K  = q->K;

    if (he_nv(he) != n)
        ERR(HE_INDEX, "he_nv(he)=%d != n = %d", he_nv(he), n);
    return 2*K;
}
