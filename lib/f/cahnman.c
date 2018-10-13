#include <stdio.h>

#include "real.h"
#include "he/memory.h"
#include "he/err.h"
#include "he/he.h"
#include "he/vec.h"
#include "he/dih.h"
#include "he/ddih.h"
#include "he/dedg.h"
#include "he/tri.h"
#include "he/dtri.h"
#include "he/sum.h"

#include "he/f/cahnman.h"

#define T HeFCahnman

#    define  nxt(h)     he_nxt(he, h)
#    define  flp(h)     he_flp(he, h)
#    define  ver(h)     he_ver(he, h)
#    define  hdg_ver(v) he_hdg_ver(he, v)
#    define  hdg_edg(e) he_hdg_edg(he, e)
#    define  hdg_tri(t) he_hdg_tri(he, t)
#    define  bnd(h)     he_bnd(he, h)

typedef struct Size Size;
struct Size { int nv, ne, nt; };

struct T {
    Size size;
    real K;
    real *area, *curva_mean, *energy;
    real *theta, *len;
    real *fx, *fy, *fz;
};

static void zero(int n, real *a) {
    int i;
    for (i = 0; i < n; i++)
        a[i] = 0;
}

static real sum(int n, real *a) {
    int i;
    real s;
    HeSum *sum;
    he_sum_ini(&sum);
    s = 0;
    for (i = 0; i < n; i++)
        he_sum_add(sum, a[i]);
    s = he_sum_get(sum);
    he_sum_fin(sum);
    return s;
}

static int plus(int n, const real *a, /*io*/ real *b) {
    int i;
    for (i = 0; i < n; i++)
        b[i] += a[i];
    return HE_OK;
}

int he_f_cahnman_ini(real K, He *he, T **pq) {
    T *q;
    int nv, ne, nt;
    Size size;

    MALLOC(1, &q);

    nv = he_nv(he);
    ne = he_ne(he);
    nt = he_nt(he);

    q->K = K;

    size.nv = nv;
    size.nt = nt;
    size.ne = ne;
    q->size = size;

    MALLOC(nv, &q->curva_mean);
    MALLOC(nv, &q->area);
    MALLOC(nv, &q->energy);
    MALLOC(ne, &q->theta);
    MALLOC(ne, &q->len);

    MALLOC(nv, &q->fx); MALLOC(nv, &q->fy); MALLOC(nv, &q->fz);

    *pq = q;

    return HE_OK;
}

int he_f_cahnman_fin(T *q) {
    FREE(q->curva_mean);
    FREE(q->area);
    FREE(q->energy);
    FREE(q->theta);
    FREE(q->len);
    FREE(q->fx); FREE(q->fy); FREE(q->fz);
    FREE(q);
    return HE_OK;
}

static int get_ijk(int t, He *he, /**/ int *pi, int *pj, int *pk) {
    int h, n, nn, i, j, k;
    h = hdg_tri(t);
    n = nxt(h);
    nn = nxt(n);
    i = ver(h); j = ver(n); k = ver(nn);
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
static int get2(int h, He *he, const real *x, const real *y, const real *z,
                /**/ real a[3], real b[3]) {
    int f, i, j;
    f = flp(h);
    i = ver(h);
    j = ver(f);
    vec_get(i, x, y, z, /**/ a);
    vec_get(j, x, y, z, /**/ b);
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

static int get_ij(int h, He *he, /**/ int *pi, int *pj) {
    int f, i, j;
    f = flp(h);
    i = ver(h); j = ver(f);
    *pi = i; *pj = j;
    return HE_OK;
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

static int compute_area(He *he, Size size, const real *xx, const real *yy, const real *zz, /**/ real *area) {
    int t, i, j, k;
    int nv, nt;
    real a[3], b[3], c[3];
    real area0;
    nt = size.nt;
    nv = size.nv;
    zero(nv, area);
    for (t = 0; t < nt; t++) {
        get_ijk(t, he, &i, &j, &k);
        get3(xx, yy, zz, i, j, k, a, b, c);
        area0 = tri_area(a, b, c)/3;
        area[i] += area0;
        area[j] += area0;
        area[k] += area0;
    }
    return HE_OK;
}

static int compute_len(He *he, Size size, const real *xx, const real *yy, const real *zz,
                             /**/ real *len) {
    int ne, e, h;
    real b[3], c[3], u[3];
    ne = size.ne;

    for (e = 0; e < ne; e++) {
        h = hdg_edg(e);
        if (bnd(h)) continue;
        get2(h, he, xx, yy, zz, /**/ c, b);
        vec_minus(c, b, /**/ u);
        len[e] = vec_abs(u);
    }
    return HE_OK;
}

static int compute_theta(He *he, Size size, const real *xx, const real *yy, const real *zz,
                             /**/ real *theta) {
    int ne, e, h;
    int i, j, k, l;
    real a[3], b[3], c[3], d[3];

    ne = size.ne;

    for (e = 0; e < ne; e++) {
        h = hdg_edg(e);
        if (bnd(h)) continue;
        get_ijkl(h, he, /**/ &i, &j, &k, &l);
        get4(xx, yy, zz, i, j, k, l, /**/ a, b, c, d);
        theta[e] = tri_dih(a, b, c, d);
    }
    return HE_OK;
}

static int compute_mean_curv(He *he, Size size,
                             real *len, real *theta,
                             /**/ real *curva_mean) {
    int nv, ne, e, h;
    int i, j;
    real cur;

    nv = size.nv;
    ne = size.ne;

    zero(nv, curva_mean);
    for (e = 0; e < ne; e++) {
        h = hdg_edg(e);
        get_ij(h, he, /**/ &i, &j);
        cur = len[e]*theta[e];
        curva_mean[i] += cur;
        curva_mean[j] += cur;
    }
    return HE_OK;
}

static int compute_energy(real K, Size size,
                          real *area, real *curva_mean, /**/ real *energy) {
    int v, nv;
    real energy0;
    nv = size.nv;
    for (v = 0; v < nv; v++)
        energy[v] = K*curva_mean[v]*curva_mean[v]/area[v]/8;
    return HE_OK;
}

real he_f_cahnman_energy(T *q, He *he,
                      const real *x, const real *y, const real *z) {
    Size size;
    real K;
    real eng, *area, *curva_mean, *energy, *theta, *len;
    int nv;

    size = q->size;
    K = q->K;

    area = q->area;
    curva_mean = q->curva_mean;
    energy = q->energy;
    len = q->len;
    theta = q->theta;

    nv = size.nv;

    compute_area(he, size, x, y, z, /**/ area);
    compute_len(he, size, x, y, z, /**/ len);
    compute_theta(he, size, x, y, z, /**/ theta);
    compute_mean_curv(he, size, len, theta, /**/ curva_mean);
    compute_energy(K, size, area, curva_mean, /**/ energy);
    eng = sum(nv, energy);
    return eng;
}

static int force_edg(He *he, Size size,
                     const real *theta,  const real *curva_mean, const real *area,
                     const real *xx, const real *yy, const real *zz, /**/
                     real *fx, real *fy, real *fz) {
    int h, e, j, k;
    int ne;
    real coef;
    real b[3], c[3], db[3], dc[3];
    ne = size.ne;
    for (e = 0; e < ne; e++) {
        h = hdg_edg(e);
        get_ij(h, he, /**/ &j, &k);
        vec_get(j, xx, yy, zz, /**/ b);
        vec_get(k, xx, yy, zz, /**/ c);
        dedg_abs(b, c, db, dc);
        coef =  -(curva_mean[j]/area[j] + curva_mean[k]/area[k])*theta[e]/4;
        vec_scalar_append(db, coef, j, fx, fy, fz);
        vec_scalar_append(dc, coef, k, fx, fy, fz);
    }
    return HE_OK;
}

static int force_theta(He *he, Size size,
                          const real *curva_mean, const real *area,
                          const real *xx, const real *yy, const real *zz,
                          /**/ real *fx, real *fy, real *fz) {
    int h, e, ne;
    int i, j, k, l;
    real len0, coef;
    real a[3], b[3], c[3], d[3];
    real da[3], db[3], dc[3], dd[3], u[3];

    ne = size.ne;

    for (e = 0; e < ne; e++) {
        h = hdg_edg(e);
        if (bnd(h)) continue;
        get_ijkl(h, he, /**/ &i, &j, &k, &l);
        get4(xx, yy, zz, i, j, k, l, /**/ a, b, c, d);
        ddih_angle(a, b, c, d, da, db, dc, dd);
        vec_minus(c, b, u);
        len0 = vec_abs(u);
        coef =  -(curva_mean[j]/area[j] + curva_mean[k]/area[k])*len0/4;
        vec_scalar_append(da, coef, i, fx, fy, fz);
        vec_scalar_append(db, coef, j, fx, fy, fz);
        vec_scalar_append(dc, coef, k, fx, fy, fz);
        vec_scalar_append(dd, coef, l, fx, fy, fz);
    }
    return HE_OK;
}

static int force_area(He *he, Size size, /**/
                      const real *curva_mean, const real *area,
                      const real *xx, const real *yy, const real *zz,
                      /**/ real *fx, real *fy, real *fz) {
    int nt, t, i, j, k;
    real a[3], b[3], c[3];
    real da[3], db[3], dc[3];
    real coef1, coef2, coef;

    nt = size.nt;

    for (t = 0; t < nt; t++) {
        get_ijk(t, he, &i, &j, &k);
        get3(xx, yy, zz, i, j, k, a, b, c);

        dtri_area(a, b, c, da, db, dc);
        coef1 = 2.0/3.0;

        coef2 = curva_mean[i]/4*curva_mean[i]/4/area[i]/area[i];
        coef = coef1 * coef2;
        vec_scalar_append(da, coef, i, fx, fy, fz);

        coef2 = curva_mean[j]/4*curva_mean[j]/4/area[j]/area[j];
        coef = coef1 * coef2;
        vec_scalar_append(db, coef, j, fx, fy, fz);

        coef2 = curva_mean[k]/4*curva_mean[k]/4/area[k]/area[k];
        coef = coef1 * coef2;
        vec_scalar_append(dc, coef, k, fx, fy, fz);
    }
    return HE_OK;
}


int he_f_cahnman_force(T *q, He *he,
                      const real *x, const real *y, const real *z, /**/
                      real *fx_tot, real *fy_tot, real *fz_tot) {
    Size size;
    int nv;
    real *area;
    real *theta, *len, *curva_mean;
    real *fx, *fy, *fz;
    size = q->size;

    area = q->area;
    theta = q->theta;
    len = q->len;
    curva_mean = q->curva_mean;
    fx = q->fx; fy = q->fy; fz = q->fz;

    nv = size.nv;
    zero(nv, fx); zero(nv, fy); zero(nv, fz);

    compute_area(he, size, x, y, z, /**/ area);
    compute_len(he, size, x, y, z, /**/ len);
    compute_theta(he, size, x, y, z, /**/ theta);
    compute_mean_curv(he, size, len, theta, /**/ curva_mean);

    force_edg(he, size, theta,  curva_mean, area, x, y, z, /**/ fx, fy, fz);
    force_theta(he, size, curva_mean, area, x, y, z, /**/ fx, fy, fz);
    force_area(he, size, curva_mean, area,  x, y, z, /**/ fx, fy, fz);
    plus(nv, fx, /*io*/ fx_tot);
    plus(nv, fy, /*io*/ fy_tot);
    plus(nv, fz, /*io*/ fz_tot);

    return HE_OK;
}

int he_f_cahnman_curva_mean(T *q, /**/ real **pa) {
    *pa = q->curva_mean;
    return HE_OK;
}

int he_f_cahnman_energy_ver(T *q, /**/ real **pa) {
    *pa = q->energy;
    return HE_OK;
}

int he_f_cahnman_area_ver(T *q, /**/ real **pa) {
    *pa = q->area;
    return HE_OK;
}
