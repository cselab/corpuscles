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
  real Kb, C0, Kad, DA0D;
  real *lbx, *lby, *lbz;
  real *normx, *normy, *normz;
  real *curva_mean, *curva_gauss;
  real *energy, *area;
  
  real *l2, *t;
  
  int nv, ne, nt;
};
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
static real sum(int n, real *volume) {
    int t;
    real v;
    v = 0;
    for (t = 0; t < n; t++) v += volume[t];
    return v;
}
int he_f_gompper_ini(real Kb, real C0, real Kad, real DA0D, He *he, T **pq) {
  T *q;
  int nv, ne, nt, nh;
  MALLOC(1, &q);
  
  nv = he_nv(he);
  ne = he_ne(he);
  nt = he_nt(he);
  nh = he_nh(he);

  q->nv = nv;
  q->ne = ne;
  q->nt = nt;
  
  q->Kb   = Kb;
  q->C0   = C0;
  q->Kad  = Kad;
  q->DA0D = DA0D;

  MALLOC(nh, &q->l2);
  MALLOC(nh, &q->t);

  MALLOC(nv, &q->lbx); MALLOC(nv, &q->lby); MALLOC(nv, &q->lbz);
  MALLOC(nv, &q->normx); MALLOC(nv, &q->normy); MALLOC(nv, &q->normz);
  MALLOC(nv, &q->curva_mean);  MALLOC(nv, &q->curva_gauss);
  
  MALLOC(nv, &q->energy);
  MALLOC(nv, &q->area);
  MALLOC(nv, &q->lbx);
  MALLOC(nv, &q->lby);
  MALLOC(nv, &q->lbz);
  
  *pq = q;
  return HE_OK;
}
int he_f_gompper_fin(T *q) {
    FREE(q->t); FREE(q->l2);
    FREE(q->lbx); FREE(q->lby); FREE(q->lbz);
    FREE(q->normx);FREE(q->normy);FREE(q->normz);
    FREE(q->curva_mean);FREE(q->curva_gauss);
    FREE(q->energy); FREE(q->area);
    FREE(q);
    return HE_OK;
}
int he_f_gompper_area_ver(T *q, /**/ real **pa) {
  *pa = q->area;
  return HE_OK;
}
int he_f_gompper_laplace_ver(T *q, /**/ real **px, real **py, real **pz ) {
    *px = q->lbx;
    *py = q->lby;
    *pz = q->lbz;
    return HE_OK;
}
int he_f_gompper_norm_ver(T *q, /**/ real **px, real **py, real **pz ) {
  *px = q->normx;
  *py = q->normy;
  *pz = q->normz;
  return HE_OK;
}
int he_f_gompper_curva_mean_ver(T *q, /**/ real **pa) {
  *pa = q->curva_mean;
  return HE_OK;
}
int he_f_gompper_curva_gauss_ver(T *q, /**/ real **pa) {
  *pa = q->curva_gauss;
  return HE_OK;
}
int he_f_gompper_energy_ver(T *q, /**/ real**pa) {
  *pa = q->energy;
  return HE_OK;
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

static void compute_force_t(real Kb, He *he, const real *x, const real *y, const real *z, const real *t, const real *lbx, const real *lby, const real *lbz, /**/ real *fx, real *fy, real *fz) {
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
        vec_get(i, lbx, lby, lbz, ll);
        l2 = vec_dot(ll, ll);
        vec_linear_combination(Kb*t0/2, ll, -Kb*t0*l2/8, r,  df);
        vec_append(df, i, /**/ fx, fy, fz);
        vec_substr(df, j, /**/ fx, fy, fz);
    }
}

static void compute_force_dt(real Kb, He *he, const real *x, const real *y, const real *z,
                             const real *lbx, const real *lby, const real *lbz,
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
        vec_get(k, lbx, lby, lbz, /**/ lk);
        vec_get(i, lbx, lby, lbz, /**/ li);
        r2 = vec_dot(r, r);
        dl = vec_dot(lk, lk) + vec_dot(li, li);
        dd = vec_dot(li, r)  - vec_dot(lk, r);
        C = Kb*dd/2 - Kb*r2*dl/16;
        vec_scalar_append(da,  C,  i, /**/ fx, fy, fz);
        vec_scalar_append(db,  C,  j, /**/ fx, fy, fz);
        vec_scalar_append(dc,  C,  k, /**/ fx, fy, fz);
    }
}

int he_f_gompper_force(T *q, He *he,
                      const real *x, const real *y, const real *z, /**/
                      real *fx, real *fy, real *fz) {
    int nv;
    real Kb;
    real *l2, *t, *area, *lbx, *lby, *lbz;

    area = q->area;
    l2 = q->l2; t = q->t;
    lbx = q->lbx; lby = q->lby; lbz = q->lbz;

    nv = q->nv;
    Kb  = q->Kb;
    if (he_nv(he) != nv)
        ERR(HE_INDEX, "he_nv(he)=%d != nv = %d", he_nv(he), nv);
    compute_l2(he, x, y, z, /**/ l2);
    compute_t(he, x, y, z, /**/ t);
    compute_area(he, l2, t, /**/ area);
    compute_laplace(he, x, t, area, /**/ lbx);
    compute_laplace(he, y, t, area, /**/ lby);
    compute_laplace(he, z, t, area, /**/ lbz);

    compute_force_t(Kb, he, x, y, z, t, lbx, lby, lbz,
                    /**/ fx, fy, fz);
    compute_force_dt(Kb, he, x, y, z, lbx, lby, lbz,
                     /**/ fx, fy, fz);
    return HE_OK;
}

void compute_energy(int n, const real *area, const real *lbx, const real *lby, const real *lbz, /**/ real *energy) {
    int i;
    real area0, curv_sq, l[3];
    for (i = 0; i < n; i++) {
        area0 = area[i];
        vec_get(i, lbx, lby, lbz, /**/ l);
        curv_sq = vec_dot(l, l);
        energy[i] = curv_sq * area0;
    }
}

real he_f_gompper_energy(T *q, He *he,
                      const real *x, const real *y, const real *z) {
    int nv;
    real Kb;
    real *l2, *t, *area, *energy, *lbx, *lby, *lbz;

    area = q->area; energy = q->energy;
    l2 = q->l2; t = q->t;
    lbx = q->lbx; lby = q->lby; lbz = q->lbz;
    nv = q->nv;
    Kb = q->Kb;

    if (he_nv(he) != nv)
        ERR(HE_INDEX, "he_nv(he)=%d != nv = %d", he_nv(he), nv);
    compute_l2(he, x, y, z, /**/ l2);
    compute_t(he, x, y, z, /**/ t);
    compute_area(he, l2, t, /**/ area);
    compute_laplace(he, x, t, area, /**/ lbx);
    compute_laplace(he, y, t, area, /**/ lby);
    compute_laplace(he, z, t, area, /**/ lbz);
    compute_energy(nv, area, lbx, lby, lbz, /**/ energy);

    return 2*Kb*sum(nv, energy);
}
