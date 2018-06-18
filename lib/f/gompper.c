#include <stdio.h>

#include "real.h"
#include "he/memory.h"
#include "he/err.h"
#include "he/he.h"
#include "he/vec.h"
#include "he/tri.h"
#include "he/dtri.h"

#include "he/f/gompper.h"

#define T HeFGompper

#define  nxt(h)     he_nxt(he, (h))
#define  flp(h)     he_flp(he, (h))
#define  ver(h)     he_ver(he, (h))
#define  hdg_ver(v) he_hdg_ver(he, (v))
#define  hdg_edg(e) he_hdg_edg(he, (e))
#define  bnd(h)     he_bnd(he, (h))


struct T {
    int nv;
    real *area;
    real *energy;
    real *lx, *ly, *lz;

    real *l2, *t;
    real K;
};

int he_f_gompper_ini(real K, He *he, T **pq) {
    T *q;
    int nv, nh;
    MALLOC(1, &q);

    nh = he_nh(he);
    MALLOC(nh, &q->l2);
    MALLOC(nh, &q->t);

    nv = he_nv(he);
    MALLOC(nv, &q->energy);
    MALLOC(nv, &q->area);
    MALLOC(nv, &q->lx);
    MALLOC(nv, &q->ly);
    MALLOC(nv, &q->lz);

    q->nv = nv;
    q->K = K;

    *pq = q;
    return HE_OK;
}

int he_f_gompper_fin(T *q) {
    FREE(q->t); FREE(q->l2);
    FREE(q->energy); FREE(q->area); FREE(q->lx); FREE(q->ly); FREE(q->lz);
    FREE(q);
    return HE_OK;
}

int he_f_gompper_area(T *q, /**/ real **pa) {
    *pa = q->area;
    return HE_OK;
}

int he_f_gompper_energy_ver(T *q, /**/ real**pa) {
    *pa = q->energy;
    return HE_OK;
}

int he_f_gompper_laplace(T *q, /**/ real **px, real **py, real **pz ) {
    *px = q->lx;
    *py = q->ly;
    *pz = q->lz;
    return HE_OK;
}

static real sum(int n, real *volume) {
    int t;
    real v;
    v = 0;
    for (t = 0; t < n; t++) v += volume[t];
    return v;
}
static void get_edg(int i, int j, const real *x, const real *y, const real *z, /**/ real r[3]) {
    real a[3], b[3];
    vec_get(i, x, y, z, a);
    vec_get(j, x, y, z, b);
    vec_minus(a, b, r);
}
static void get3(int i, int j, int k,
                 const real *x, const real *y, const real *z,
                 /**/ real a[3], real b[3], real c[3]) {
    vec_get(i, x, y, z, a);
    vec_get(j, x, y, z, b);
    vec_get(k, x, y, z, c);
}

static void zero(int n, real *a) {
    int i;
    for (i = 0; i < n; i++) a[i] = 0;
}
static void compute_l2(He *he, const real *x, const real *y, const real *z, /**/ real *H) {
    int nh;
    int h, n;
    int i, j;
    real r[3];
    nh = he_nh(he);
    for (h = 0; h < nh; h++) {
        n = nxt(h);
        i = ver(h); j = ver(n);
        get_edg(i, j, x, y, z, /**/ r);
        H[h] = vec_dot(r, r);
    }
}

static void compute_t(He *he, const real *x, const real *y, const real *z, /**/ real *H) {
    int nh, h, n, nn;
    int i, j, k;
    real a[3], b[3], c[3], cot;
    nh = he_nh(he);
    zero(nh, H);
    for (h = 0; h < nh; h++) {
        n = nxt(h); nn = nxt(n);
        i = ver(h); j = ver(n); k = ver(nn);
        get3(i, j, k, x, y, z, /**/ a, b, c);
        cot = tri_cot(b, c, a);
        H[h] += cot;
        if (!bnd(h)) H[flp(h)] += cot;
    }
}

static void compute_area(He *he, const real *l2, const real *t, /**/ real *V) {
    int nv, nh, h, i;
    nv = he_nv(he);
    zero(nv, V);
    nh = he_nh(he);
    for (h = 0; h < nh; h++) {
        i = ver(h);
        V[i] += t[h]*l2[h]/8;
    }
}

static void compute_laplace(He *he, const real *V0, const real *t, const real *area, /**/ real *V1) {
    int h, n, nv, nh, i, j;
    nv = he_nv(he);
    zero(nv, V1);
    nh = he_nh(he);
    for (h = 0; h < nh; h++) {
        n = nxt(h);
        i = ver(h); j = ver(n);
        V1[i] += t[h]*(V0[i] - V0[j])/2;
    }
    for (i = 0; i < nv; i++)
        V1[i] /= area[i];
}

static void compute_force_t(real K, He *he, const real *x, const real *y, const real *z, const real *t, const real *lx, const real *ly, const real *lz, /**/ real *fx, real *fy, real *fz) {
    int nh;
    int h, n;
    int i, j;
    real r[3], ll[3], df[3];
    real t0, l2;
    nh = he_nh(he);
    for (h = 0; h < nh; h++) {
        n = nxt(h); i = ver(h); j = ver(n);
        t0 = t[h];
        get_edg(i, j, x, y, z, /**/  r);
        vec_get(i, lx, ly, lz, ll);
        l2 = vec_dot(ll, ll);
        vec_linear_combination(K*t0/2, ll, -K*t0*l2/8, r,  df);
        vec_append(df, i, /**/ fx, fy, fz);
        vec_substr(df, j, /**/ fx, fy, fz);
    }
}

static void compute_force_dt(real K, He *he, const real *x, const real *y, const real *z,
                             const real *lx, const real *ly, const real *lz,
                             /**/ real *fx, real *fy, real *fz) {
    int nh;
    int h, n, nn;
    int i, j, k;
    real r[3], a[3], b[3], c[3], da[3], db[3], dc[3];
    real li[3], lk[3];
    real dl, dd, r2, C;
    nh = he_nh(he);
    for (h = 0; h < nh; h++) {
        n = nxt(h); nn = nxt(n);
        i = ver(h); j = ver(n); k = ver(nn);
        get3(i, j, k, x, y, z,    a, b, c);
        dtri_cot(a, b, c, /**/ da, db, dc);
        get_edg(i, k, x, y, z, /**/ r);
        vec_get(k, lx, ly, lz, /**/ lk);
        vec_get(i, lx, ly, lz, /**/ li);
        r2 = vec_dot(r, r);
        dl = vec_dot(lk, lk) + vec_dot(li, li);
        dd = vec_dot(li, r)  - vec_dot(lk, r);
        C = K*dd/2 - K*r2*dl/16;
        vec_scalar_append(da,  C,  i, /**/ fx, fy, fz);
        vec_scalar_append(db,  C,  j, /**/ fx, fy, fz);
        vec_scalar_append(dc,  C,  k, /**/ fx, fy, fz);
    }
}

int he_f_gompper_force(T *q, He *he,
                      const real *x, const real *y, const real *z, /**/
                      real *fx, real *fy, real *fz) {
    int nv;
    real K;
    real *l2, *t, *area, *lx, *ly, *lz;

    area = q->area;
    l2 = q->l2; t = q->t;
    lx = q->lx; ly = q->ly; lz = q->lz;

    nv = q->nv;
    K  = q->K;
    if (he_nv(he) != nv)
        ERR(HE_INDEX, "he_nv(he)=%d != nv = %d", he_nv(he), nv);
    compute_l2(he, x, y, z, /**/ l2);
    compute_t(he, x, y, z, /**/ t);
    compute_area(he, l2, t, /**/ area);
    compute_laplace(he, x, t, area, /**/ lx);
    compute_laplace(he, y, t, area, /**/ ly);
    compute_laplace(he, z, t, area, /**/ lz);

    compute_force_t(K, he, x, y, z, t, lx, ly, lz,
                    /**/ fx, fy, fz);
    compute_force_dt(K, he, x, y, z, lx, ly, lz,
                     /**/ fx, fy, fz);
    return HE_OK;
}

void compute_energy(int n, const real *area, const real *lx, const real *ly, const real *lz, /**/ real *energy) {
    int i;
    real area0, curv, l[3];
    for (i = 0; i < n; i++) {
        area0 = area[i];
        vec_get(i, lx, ly, lz, /**/ l);
        curv = vec_abs(l);
        energy[i] = curv * area0;
    }
}

real he_f_gompper_energy(T *q, He *he,
                      const real *x, const real *y, const real *z) {
    int nv;
    real K;
    real *l2, *t, *area, *energy, *lx, *ly, *lz;

    area = q->area; energy = q->energy;
    l2 = q->l2; t = q->t;
    lx = q->lx; ly = q->ly; lz = q->lz;
    nv = q->nv;
    K  = q->K;

    if (he_nv(he) != nv)
        ERR(HE_INDEX, "he_nv(he)=%d != nv = %d", he_nv(he), nv);
    compute_l2(he, x, y, z, /**/ l2);
    compute_t(he, x, y, z, /**/ t);
    compute_area(he, l2, t, /**/ area);
    compute_laplace(he, x, t, area, /**/ lx);
    compute_laplace(he, y, t, area, /**/ ly);
    compute_laplace(he, z, t, area, /**/ lz);
    compute_energy(nv, area, lx, ly, lz, /**/ energy);

    return 2*K*sum(nv, energy);
}
