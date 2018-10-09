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

#include "he/f/juelicher.h"

#define T HeFJuelicher

#    define  nxt(h)     he_nxt(he, h)
#    define  flp(h)     he_flp(he, h)
#    define  ver(h)     he_ver(he, h)
#    define  hdg_ver(v) he_hdg_ver(he, v)
#    define  hdg_edg(e) he_hdg_edg(he, e)
#    define  hdg_tri(t) he_hdg_tri(he, t)
#    define  bnd(h)     he_bnd(he, h)

typedef struct Param Param;
struct Param { real K, Kad, C0; };

typedef struct Size Size;
struct Size { int nv, ne, nt; };

struct T {
    Size size;
    Param param;
    real *area, *curva_mean, *energy;
    real *theta, *len, *lentheta;
    real *fx, *fy, *fz;
    real *fxad, *fyad, *fzad;
};

static void zero(int n, real *a) {
    int i;
    for (i = 0; i < n; i++) a[i] = 0;
}

static real sum(int n, real *a) {
    int i;
    real s;
    s = 0;
    for (i = 0; i < n; i++) s += a[i];
    return s;
}

static int plus(int n, const real *a, /*io*/ real *b) {
    int i;
    for (i = 0; i < n; i++)
        b[i] += a[i];
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

    MALLOC(nv, &q->curva_mean);
    MALLOC(nv, &q->area);
    MALLOC(nv, &q->energy);
    MALLOC(ne, &q->theta);
    MALLOC(ne, &q->len);
    MALLOC(ne, &q->lentheta);

    MALLOC(nv, &q->fx); MALLOC(nv, &q->fy); MALLOC(nv, &q->fz);
    MALLOC(nv, &q->fxad); MALLOC(nv, &q->fyad); MALLOC(nv, &q->fzad);

    *pq = q;

    return HE_OK;
}

int he_f_juelicher_fin(T *q) {
    FREE(q->curva_mean);
    FREE(q->area);
    FREE(q->energy);
    FREE(q->theta);
    FREE(q->len);
    FREE(q->lentheta);
    FREE(q->fx); FREE(q->fy); FREE(q->fz);
    FREE(q->fxad); FREE(q->fyad); FREE(q->fzad);
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
    real a[3], b[3], c[3], d[3];
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
    int nv, ne, e, h;
    int i, j, k, l;
    real b[3], c[3], u[3];

    nv = size.nv;
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
    int nv, ne, e, h;
    int i, j, k, l;
    real a[3], b[3], c[3], d[3], u[3];
    real theta0, len, cur;

    nv = size.nv;
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

static int compute_mean_curv(He *he, Size size, const real *xx, const real *yy, const real *zz,
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
        cur = len[e]*theta[e]/4;
        curva_mean[i] += cur;
        curva_mean[j] += cur;
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
        energy0 = 2*(curva_mean[v]-H0)*(curva_mean[v]-H0)*area[v];
        energy[v] = K*energy0;
    }
}

real he_f_juelicher_energy(T *q, He *he,
                      const real *x, const real *y, const real *z) {
    Size size;
    Param param;
    real eng, *area, *curva_mean, *energy, *theta, *len;
    int nv;

    size = q->size;
    param = q->param;

    area = q->area;
    curva_mean = q->curva_mean;
    energy = q->energy;
    len = q->len;
    theta = q->theta;

    nv = size.nv;

    compute_area(he, size, x, y, z, /**/ area);
    compute_len(he, size, x, y, z, /**/ len);
    compute_theta(he, size, x, y, z, /**/ theta);
    compute_mean_curv(he, size, x, y, z, len, theta, /**/ curva_mean);
    compute_energy(he, param, size, x, y, z, area, curva_mean, /**/ energy);
    eng = sum(nv, energy);
    return eng;
}

static int compute_lentheta(He *he, Size size, const real *len, const real *theta, /**/ real *lentheta) {
    int nv, ne, e, h, i, j;
    real lentheta0;
    nv = size.nv;
    ne = size.ne;
    zero(nv, lentheta);
    for (e = 0; e < ne; e++) {
        h = hdg_edg(e);
        get_ij(h, he, /**/ &i, &j);
        lentheta0 = len[e]*theta[e];
        lentheta[i] += lentheta0;
        lentheta[j] += lentheta0;
    }

    return HE_OK;
}

static int force_edg(He *he, real H0, Size size, real curva_mean_area_tot,
                     const real *theta,  const real *lentheta, const real *area,
                     const real *xx, const real *yy, const real *zz, /**/
                     real *fx, real *fy, real *fz, real *fxad, real *fyad, real *fzad) {
    int h, e, j, k;
    int ne;
    real theta0, coef;
    real b[3], c[3], db[3], dc[3];
    ne = size.ne;
    for (e = 0; e < ne; e++) {
        h = hdg_edg(e);
        get_ij(h, he, /**/ &j, &k);
        vec_get(j, xx, yy, zz, /**/ b);
        vec_get(k, xx, yy, zz, /**/ c);
        dedg_abs(b, c, db, dc);
        theta0 = theta[e];
        coef = - ( (lentheta[j]/area[j]/4 - H0) + (lentheta[k]/area[k]/4 - H0) ) * theta0;
        vec_scalar_append(db, coef, j, fx, fy, fz);
        vec_scalar_append(dc, coef, k, fx, fy, fz);
        coef = -curva_mean_area_tot/4 * theta0;
        vec_scalar_append(db, coef, j, fxad, fyad, fzad);
        vec_scalar_append(dc, coef, k, fxad, fyad, fzad);
    }
    return HE_OK;
}

static int force_lentheta(He *he, real H0, Size size,
                          real curva_mean_area_tot, const real *lentheta, const real *area,
                          const real *xx, const real *yy, const real *zz,
                          /**/ real *fx, real *fy, real *fz, real *fxad, real *fyad, real *fzad) {
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
        coef =  -(  (lentheta[j]/area[j]/4 - H0) + (lentheta[k]/area[k]/4 - H0) ) * len0 ;
        vec_scalar_append(da, coef, i, fx, fy, fz);
        vec_scalar_append(db, coef, j, fx, fy, fz);
        vec_scalar_append(dc, coef, k, fx, fy, fz);
        vec_scalar_append(dd, coef, l, fx, fy, fz);
        coef = -curva_mean_area_tot/4.0*len0;
        vec_scalar_append(da, coef, i, fxad, fyad, fzad);
        vec_scalar_append(db, coef, j, fxad, fyad, fzad);
        vec_scalar_append(dc, coef, k, fxad, fyad, fzad);
        vec_scalar_append(dd, coef, l, fxad, fyad, fzad);

    }
    return HE_OK;
}

static int force_area(He *he, real H0, Size size, /**/
                      const real *lentheta, const real *area,
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
        coef1 = 1.0/3.0;

        coef2 = lentheta[i]*lentheta[i]/8.0/area[i]/area[i] - 2.0*H0*H0;
        coef = coef1 * coef2;
        vec_scalar_append(da, coef, i, fx, fy, fz);

        coef2 = lentheta[j]*lentheta[j]/8.0/area[j]/area[j] - 2.0*H0*H0;
        coef = coef1 * coef2;
        vec_scalar_append(db, coef, j, fx, fy, fz);

        coef2 = lentheta[k]*lentheta[k]/8.0/area[k]/area[k] - 2.0*H0*H0;
        coef = coef1 * coef2;
        vec_scalar_append(dc, coef, k, fx, fy, fz);
    }
    return HE_OK;
}


int he_f_juelicher_force(T *q, He *he,
                      const real *x, const real *y, const real *z, /**/
                      real *fx_tot, real *fy_tot, real *fz_tot) {
    Size size;
    Param param;
    int nv;
    real H0, Kad;
    real eng, *area, *curva_mean;
    real *energy, *theta, *len, *lentheta;
    real area_tot, lentheta_tot, curva_mean_area_tot;
    real *fx, *fy, *fz, *fxad, *fyad, *fzad;
    const real pi = 3.141592653589793115997964;

    size = q->size;
    param = q->param;

    H0 = param.C0/2;
    Kad = param.Kad;

    area = q->area;
    curva_mean = q->curva_mean;
    energy = q->energy;
    theta = q->theta;
    len = q->len;
    lentheta = q->lentheta;
    fx = q->fx; fy = q->fy; fz = q->fz;
    fxad = q->fxad; fyad = q->fyad; fzad = q->fzad;

    nv = size.nv;
    zero(nv, fx); zero(nv, fy); zero(nv, fz);
    zero(nv, fxad); zero(nv, fyad); zero(nv, fzad);

    compute_area(he, size, x, y, z, /**/ area);
    compute_len(he, size, x, y, z, /**/ len);
    compute_theta(he, size, x, y, z, /**/ theta);
    compute_lentheta(he, size, len, theta, /**/ lentheta);

    area_tot = sum(nv, area);
    lentheta_tot = sum(nv, lentheta);
    curva_mean_area_tot = (lentheta_tot/4 - H0*area_tot)*(4*Kad*pi/area_tot);

    force_edg(he, H0, size, curva_mean_area_tot,
              theta,  lentheta, area,
              x, y, z, /**/
              fx, fy, fz, fxad, fyad, fzad);
    force_lentheta(he, H0, size,
                   curva_mean_area_tot, lentheta, area,
                   x, y, z, /**/
                   fx, fy, fz, fxad, fyad, fzad);
    force_area(he, H0, size, /**/
               lentheta, area,
               x, y, z, /**/ fx, fy, fz);

    MSG("lentheta_tot: %g", lentheta_tot);
    MSG("curva_mean_tot: %g", curva_mean_area_tot);
    MSG("area_tot: %g", area_tot);
    MSG("fx: %g %g", fx[0], fx[nv-1]);
    MSG("fx: %g %g", fy[0], fy[nv-1]);

    plus(nv, fx, /*io*/ fx_tot);
    plus(nv, fy, /*io*/ fy_tot);
    plus(nv, fz, /*io*/ fz_tot);

    plus(nv, fxad, /*io*/ fx_tot);
    plus(nv, fyad, /*io*/ fy_tot);
    plus(nv, fzad, /*io*/ fz_tot);

    return HE_OK;
}

int he_f_juelicher_curva_mean(T *q, /**/ real**pa) {
    *pa = q->curva_mean;
    return HE_OK;
}

int he_f_juelicher_energy_ver(T *q, /**/ real**pa) {
    *pa = q->energy;
    return HE_OK;
}

int he_f_juelicher_area_ver(T *q, /**/ real**pa) {
    *pa = q->area;
    return HE_OK;
}
