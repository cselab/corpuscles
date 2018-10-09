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

typedef struct Param Param;
struct Param { real K, Kad, C0; };

typedef struct Size Size;
struct Size { int nv, ne, nt; };

struct T {
    Size size;
    Param param;

    real *lentheta, *area, *curva_mean, *energy;
    real *fx, *fy, *fz;
    real *fxad, *fyad, *fzad;
};

static void zero(int n, real *a) {
    int i;
    for (i = 0; i < n; i++) a[i] = 0;
}

static real sum(int n, real *volume) {
    int t;
    real v;
    v = 0;
    for (t = 0; t < n; t++) v += volume[t];
    return v;
}

int he_f_juelicher_ini(real K, real C0, real Kad, He *he, T **pq) {
    T *q;
    int nv, ne, nt;
    Param param;
    Size size;

    MALLOC(1, &q);

    nv = he_nv(he);
    ne = he_ne(he);
    nt = he_nt(he);

    param.K = K;
    param.C0 = C0;
    param.Kad = Kad;

    size.nv = nv;
    size.nt = nt;
    size.ne = ne;

    q->param = param;
    q->size = size;

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

static int compute_area(He *he, Size size, const real *xx, const real *yy, const real *zz, /**/ real *area) {
    int t, i, j, k;
    int nv, nt;
    real a[3], b[3], c[3], d[3];
    real area0;
    nt = size.nt;
    nv = size.nv;
    zero(nv, area);
    for (t = 0; t < nt; t++) {
        get_ijk(t, he, &i, &j, &k);
        get3(xx, yy, zz, i, j, k, a, b, c);
        area0 = tri_area(a, b, c);
        area[i] += area0/3;
        area[j] += area0/3;
        area[k] += area0/3;
    }
    return HE_OK;
}

static int compute_mean_curv(He *he, Size size, const real *xx, const real *yy, const real *zz,
                             /**/ real *curva_mean) {
    int nv, ne, e, h;
    int i, j, k, l;
    real a[3], b[3], c[3], d[3], u[3];
    real theta, len, cur;

    nv = size.nv;
    ne = size.ne;

    zero(nv, curva_mean);
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
    return HE_OK;
}

static int compute_energy(He *he, Param param, Size size, const real *xx, const real *yy, const real *zz,
                          real *area, real *curva_mean, /**/ real *energy) {
    int v, nv;
    real K, H0, energy0;

    K  = param.K;
    H0 = param.C0/2;

    nv = size.nv;
    for (v = 0; v < nv; v++) {
        curva_mean[v] /= area[v];
        energy0 = 2 * (curva_mean[v]-H0)*(curva_mean[v]-H0)*area[v];
        energy[v] = K*energy0;
    }
}

static int compute_energy0(He *he, Param param, Size size, const real *xx, const real *yy, const real *zz, /**/
                           real *curva_mean, real *area, real *energy) {
    enum {X, Y, Z};
    real Kb;
    int nv, ne, nt;
    int v, e, h, t;
    int i, j, k, l;
    real a[3], b[3], c[3], d[3], u[3];
    real cur, len, area0;
    real theta;
    real e0;
    real C0, H0;

    Kb = param.K;
    C0 = param.C0;

    nv = size.nv;
    ne = size.ne;
    nt = size.nt;

    if (nv != he_nv(he))
        ERR(HE_INDEX, "nv=%d != he_nv(he)=%d", nv, he_nv(he));
    if (nt != he_nt(he))
        ERR(HE_INDEX, "nt=%d != he_nt(he)=%d", nt, he_nt(he));

    compute_area(he, size, xx, yy, zz, /**/ area);
    compute_mean_curv(he, size, xx, yy, zz, curva_mean);
    compute_energy(he, param, size, xx, yy, zz, area, curva_mean, /**/ energy);

    return HE_OK;
}

real he_f_juelicher_energy(T *q, He *he,
                      const real *x, const real *y, const real *z) {
    Size size;
    Param param;
    real eng, *area, *curva_mean, *energy;
    int nv;

    size = q->size;
    param = q->param;

    area = q->area;
    curva_mean = q->curva_mean;
    energy = q->energy;

    nv = size.nv;

    compute_area(he, size, x, y, z, /**/ q->area);
    compute_mean_curv(he, size, x, y, z, q->curva_mean);
    compute_energy(he, param, size, x, y, z, area, curva_mean, /**/ energy);
    eng = sum(nv, q->energy);
    return eng;
}

int he_f_juelicher_energy_ver(T *q, /**/ real**pa) {
    *pa = q->energy;
    return HE_OK;
}
