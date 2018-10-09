#include <stdio.h>

#include "real.h"
#include "he/memory.h"
#include "he/err.h"
#include "he/he.h"
#include "he/vec.h"
#include "he/dih.h"
#include "he/ddih.h"
#include "he/tri.h"

#include "he/f/juelicher.h"

#define T HeFJuelicher

#    define  nxt(h)     he_nxt(he, h)
#    define  flp(h)     he_flp(he, h)
#    define  ver(h)     he_ver(he, h)
#    define  hdg_ver(v) he_hdg_ver(he, v)
#    define  hdg_edg(e) he_hdg_edg(he, e)
#    define  bnd(h)     he_bnd(he, h)

struct T {
    int nv, ne, nt;
    real K, Kad, C0;

    real *lentheta, *area, *curva_mean, *energy;
    real *fx, *fy, *fz;
    real *fxad, *fyad, *fzad;
};

static void zero(int n, real *a) {
    int i;
    for (i = 0; i < n; i++) a[i] = 0;
}

int he_f_juelicher_ini(real K, real C0, real Kad, He *he, T **pq) {
    T *q;
    int nv, ne, nt;
    MALLOC(1, &q);

    nv = he_nv(he);
    ne = he_ne(he);
    nt = he_nt(he);

    q->K = K;
    q->C0 = C0;
    q->Kad = Kad;

    q->nv = nv;
    q->nt = nt;
    q->ne = ne;

    MALLOC(nv, &q->lentheta);
    MALLOC(nv, &q->curva_mean);
    MALLOC(nv, &q->area);
    MALLOC(nv, &q->energy);

    *pq = q;

    return HE_OK;
}

int he_f_juelicher_fin(T *q) {
    FREE(q->lentheta);
    FREE(q->curva_mean);
    FREE(q->area);
    FREE(q->energy);
    FREE(q);
    return HE_OK;
}

static int get_ijk(int t, He *he, /**/ int *pi, int *pj, int *pk) {
    int h, n, nn, i, j, k;
    h = he_hdg_tri(he, t);
    n = he_nxt(he, h);
    nn = he_nxt(he, n);
    i = he_ver(he, h); j = he_ver(he, n); k = he_ver(he, nn);
    *pi = i; *pj = j; *pk = k;
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

enum {BULK, BND};
static int get_ijkl(int h, He *he, /**/ int *pi, int *pj, int *pk, int *pl) {
    int n, nn, nnf, i, j, k, l;
    n = nxt(h); nn = nxt(nxt(h));
    nnf = nxt(nxt(flp(h)));
    j = ver(h); k = ver(n); i = ver(nn); l = ver(nnf);

    *pi = i; *pj = j; *pk = k; *pl = l;
    return BULK;
}

static int get4(const real *x, const real *y, const real *z,
                int i, int j, int k, int l,
               /**/ real a[3], real b[3], real c[3], real d[3]) {
    vec_get(i, x, y, z, /**/ a);
    vec_get(j, x, y, z, /**/ b);
    vec_get(k, x, y, z, /**/ c);
    vec_get(l, x, y, z, /**/ d);
    return HE_OK;
}


int he_f_juelicher_force(T *q, He *he,
                      const real *x, const real *y, const real *z, /**/
                      real *fx, real *fy, real *fz) {
    return HE_OK;
}

static real compute_energy(T *q, He *he, const real *xx, const real *yy, const real *zz) {
    enum {X, Y, Z};
    real Kb;
    int nv, ne, nt;
    int v, e, h, t;
    int i, j, k, l;
    real a[3], b[3], c[3], d[3], u[3];
    real cur, len, area0;
    real theta;
    real en, e0;
    real C0, H0;
    real *curva_mean, *area;

    Kb = q->K;
    C0 = q->C0;

    nv = q->nv;
    ne = q->ne;
    nt = q->nt;

    curva_mean = q->curva_mean;
    area = q->area;

    if (nv != he_nv(he))
        ERR(HE_INDEX, "nv=%d != he_nv(he)=%d", nv, he_nv(he));
    if (nt != he_nt(he))
        ERR(HE_INDEX, "nt=%d != he_nt(he)=%d", nt, he_nt(he));

    zero(nv, curva_mean);
    zero(nv, area);

    for (e = 0; e < ne; e++) {
        h = hdg_edg(e);
        if (bnd(h)) continue;
        get_ijkl(h, he, /**/ &i, &j, &k, &l);
        get4(xx, yy, zz, i, j, k, l, /**/ a, b, c, d);
        theta = tri_dih(a, b, c, d);
        vec_minus(b, c, u);
        len = vec_abs(u);
        cur = len*theta/4;
        curva_mean[j] += cur;
        curva_mean[k] += cur;
    }

    for (t = 0; t < nt; t++) {
        get_ijk(t, he, &i, &j, &k);
        get3(xx, yy, zz, i, j, k, a, b, c);
        area0 = tri_area(a, b, c);
        area[i] += area0/3;
        area[j] += area0/3;
        area[k] += area0/3;
    }

    en  = 0;
    for (v = 0; v < nv; v++) {
        curva_mean[v] /= area[v];
        e0 = 2 * (curva_mean[v]-H0)*(curva_mean[v]-H0)*area[v];
        en  += e0;
    }
    return Kb*en;
}

real he_f_juelicher_energy(T *q, He *he,
                      const real *x, const real *y, const real *z) {

    return compute_energy(q, he, x, y, z);
}

int he_f_juelicher_energy_ver(T *q, /**/ real**pa) {
    *pa = q->energy;
    return HE_OK;
}
