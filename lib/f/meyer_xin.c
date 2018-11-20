#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "real.h"
#include "he/memory.h"
#include "he/err.h"
#include "he/he.h"
#include "he/vec.h"
#include "he/dih.h"
#include "he/ddih.h"
#include "he/tri.h"
#include "he/sum.h"

#include "he/f/meyer_xin.h"

#define T HeFMeyerXin

#    define  nxt(h)     he_nxt(he, h)
#    define  flp(h)     he_flp(he, h)
#    define  ver(h)     he_ver(he, h)
#    define  hdg_ver(v) he_hdg_ver(he, v)
#    define  hdg_edg(e) he_hdg_edg(he, e)
#    define  hdg_tri(t) he_hdg_tri(he, t)
#    define  bnd(h)     he_bnd(he, h)

static const real pi = 3.141592653589793115997964;

static real mcot(const real a[3], const real b[3], const real c[3]) { return tri_cot(a, b, c); }

struct T {
  real Kb, C0, Kad, DA0D;

  int *T0, *T1, *T2;
  int *D0, *D1, *D2, *D3;

  real *cot;
  real *lbx, *lby, *lbz;
  real *normx, *normy, *normz;
  real *curva_mean, *curva_gauss;
  real *energy_local, *area;
  real *lbH;

  real energy_total;
  real energy_total_local;
  real energy_total_nonlocal;
  int nv, ne, nt, nh;
  /* how to compute area? */
  real (*Fare)(T*, He*, const real*, const real*, const real*, real *area);
};

static void zero(int n, real *a) {
    int i;
    for (i = 0; i < n; i++)
        a[i] = 0;
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
static int get4(const real *x, const real *y, const real *z,
                int i, int j, int k, int l, /**/
                real a[3], real b[3], real c[3], real d[3]) {

    vec_get(i, x, y, z, a);
    vec_get(j, x, y, z, b);
    vec_get(k, x, y, z, c);
    vec_get(l, x, y, z, d);
    return HE_OK;
}
static real sum(int n, real *a) {
    int i;
    real v;
    HeSum *sum;
    he_sum_ini(&sum);
    v = 0;
    for (i = 0; i < n; i++)
        he_sum_add(sum, a[i]);
    v = he_sum_get(sum);
    he_sum_fin(sum);
    return v;
}
enum {BULK, BND};
static int get_ijkl(int e, He *he, /**/ int *pi, int *pj, int *pk, int *pl) {
    int h, n, nn, nnf, i, j, k, l;
    h = he_hdg_edg(he, e);
    if (bnd(h)) return BND;

    h = hdg_edg(e); n = nxt(h); nn = nxt(nxt(h));
    nnf = nxt(nxt(flp(h)));
    j = ver(h); k = ver(n); i = ver(nn); l = ver(nnf);

    *pi = i; *pj = j; *pk = k; *pl = l;
    return BULK;
}
static real compute_area_voronoi(T *q, He *he,
                                 const real *x, const real *y, const real *z, /**/
                                 real *area) {
  enum {X, Y, Z};
  int t, nt, nv;
  int i, j, k;
  real a[3], b[3], c[3], u[3];
  int *T0, *T1, *T2;
  real area0;
  real cota,cotb,cotc;
  real ab2, bc2, ca2, area_tot_tri;
  HeSum *sum;

  nt = he_nt(he);
  nv = he_nv(he);
  T0 = q->T0; T1 = q->T1; T2 = q->T2;
  he_sum_ini(&sum);

  zero(nv, area);

  area_tot_tri = 0;
  for ( t = 0; t < nt; t++ ) {
    i = T0[t]; j = T1[t]; k = T2[t];

    get3(x, y, z, i, j, k, a, b, c);
    area0 = tri_area(a, b, c);

    he_sum_add(sum, area0);

    vec_minus(a, b,  u);
    ab2 = vec_dot(u, u);

    vec_minus(b, c, u);
    bc2 = vec_dot(u, u);

    vec_minus(c, a,  u);
    ca2 = vec_dot(u, u);

    cota = mcot(c, a, b);
    cotb = mcot(a, b, c);
    cotc = mcot(b, c, a);

    area[i] += ( ab2*cotc + ca2*cotb ) / 8;
    area[j] += ( bc2*cota + ab2*cotc ) / 8;
    area[k] += ( ca2*cotb + bc2*cota ) / 8;

  }/*end for loop*/

  area_tot_tri = he_sum_get(sum);

  he_sum_fin(sum);
  return area_tot_tri;

}
static real compute_area_mix(T *q, He *he,
                         const real *x, const real *y, const real *z, /**/
                         real *area) {
    enum {X, Y, Z};
    int t, nt, nv;
    int i, j, k;
    real a[3], b[3], c[3], u[3];
    int *T0, *T1, *T2;
    real area0;
    real theta_a, theta_b, theta_c;
    real cota,cotb,cotc;
    real ab2, bc2, ca2, area_tot_tri;
    HeSum *sum;

    nt = he_nt(he);
    nv = he_nv(he);
    T0 = q->T0; T1 = q->T1; T2 = q->T2;
    he_sum_ini(&sum);

    zero(nv, area);

    area_tot_tri = 0;
    for ( t = 0; t < nt; t++ ) {
        i = T0[t]; j = T1[t]; k = T2[t];

        get3(x, y, z, i, j, k, a, b, c);
        area0 = tri_area(a, b, c);

        he_sum_add(sum, area0);

        theta_a = tri_angle(c, a, b);
        theta_b = tri_angle(a, b, c);
        theta_c = tri_angle(b, c, a);

        if (theta_a > pi/2.0) {
            area[i] += area0/2;
            area[j] += area0/4;
            area[k] += area0/4;
        }
        else if ( theta_b > pi/2.0 ) {
            area[j] += area0/2;
            area[i] += area0/4;
            area[k] += area0/4;
        }
        else if ( theta_c > pi/2.0 ) {
            area[k] += area0/2;
            area[i] += area0/4;
            area[j] += area0/4;
        } else {
            vec_minus(a, b,  u);
            ab2 = vec_dot(u, u);

            vec_minus(b, c, u);
            bc2 = vec_dot(u, u);

            vec_minus(c, a,  u);
            ca2 = vec_dot(u, u);

            cota = mcot(c, a, b);
            cotb = mcot(a, b, c);
            cotc = mcot(b, c, a);

            area[i] += ( ab2*cotc + ca2*cotb ) / 8;
            area[j] += ( bc2*cota + ab2*cotc ) / 8;
            area[k] += ( ca2*cotb + bc2*cota ) / 8;
        }

    }/*end for loop*/
    area_tot_tri = he_sum_get(sum);

    he_sum_fin(sum);
    return area_tot_tri;
}

int he_f_meyer_xin_ini(real Kb, real C0, real Kad, real DA0D, He *he, T **pq) {
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
    q->nh = nh;

    q->Kb   = Kb;
    q->C0   = C0;
    q->Kad  = Kad;
    q->DA0D = DA0D;

    if (getenv("MIX")) {
        MSG("area_mixed");
        q->Fare = compute_area_mix;
    }
    else {
        MSG("area_voronoi");
        q->Fare = compute_area_voronoi;
    }

    MALLOC(nt, &q->T0); MALLOC(nt, &q->T1); MALLOC(nt, &q->T2);
    MALLOC(ne, &q->D0); MALLOC(ne, &q->D1); MALLOC(ne, &q->D2); MALLOC(ne, &q->D3);

    MALLOC(nh, &q->cot);
    MALLOC(nv, &q->lbx); MALLOC(nv, &q->lby); MALLOC(nv, &q->lbz);
    MALLOC(nv, &q->normx); MALLOC(nv, &q->normy); MALLOC(nv, &q->normz);
    MALLOC(nv, &q->curva_mean);  MALLOC(nv, &q->curva_gauss);
    MALLOC(nv, &q->energy_local); MALLOC(nv, &q->area);
    MALLOC(nv, &q->lbH);

    q->energy_total = 0;
    q->energy_total_local = 0;
    q->energy_total_nonlocal = 0;

    *pq = q;
    return HE_OK;
}
int he_f_meyer_xin_fin(T *q) {
    FREE(q->T0); FREE(q->T1); FREE(q->T2);
    FREE(q->D0); FREE(q->D1); FREE(q->D2); FREE(q->D3);
    FREE(q->cot);
    FREE(q->lbx); FREE(q->lby); FREE(q->lbz);
    FREE(q->normx);FREE(q->normy);FREE(q->normz);
    FREE(q->curva_mean);FREE(q->curva_gauss);
    FREE(q->energy_local); FREE(q->area);
    FREE(q->lbH);
    FREE(q);
    return HE_OK;
}
int he_f_meyer_xin_area_ver(T *q, /**/ real **pa) {
    *pa = q->area;
    return HE_OK;
}
int he_f_meyer_xin_laplace_ver(T *q, /**/ real **px, real **py, real **pz ) {
    *px = q->lbx;
    *py = q->lby;
    *pz = q->lbz;
    return HE_OK;
}
int he_f_meyer_xin_norm_ver(T *q, /**/ real **px, real **py, real **pz ) {
    *px = q->normx;
    *py = q->normy;
    *pz = q->normz;
    return HE_OK;
}
int he_f_meyer_xin_curva_mean_ver(T *q, /**/ real **pa) {
    *pa = q->curva_mean;
    return HE_OK;
}
int he_f_meyer_xin_curva_gauss_ver(T *q, /**/ real **pa) {
    *pa = q->curva_gauss;
    return HE_OK;
}
int he_f_meyer_xin_energy_ver(T *q, /**/ real**pa) {
    *pa = q->energy_local;
    return HE_OK;
}
int he_f_meyer_xin_laplace_H_ver(T *q, /**/ real **px ) {
    *px = q->lbH;
    return HE_OK;
}
static int compute_cot(T *q, He *he, const real *x, const real *y, const real *z,
                       /**/ real *cot) {
    int nh, h, n, nn;
    int i, j, k;
    real a[3], b[3], c[3], tt;
    nh = he_nh(he);
    zero(nh, cot);
    for (h = 0; h < nh; h++) {
        n = nxt(h); nn = nxt(n);
        i = ver(h); j = ver(n); k = ver(nn);
        get3(x, y, z, i, j, k, /**/ a, b, c);
        tt = mcot(b, c, a);
        cot[h] += tt;
        if (!bnd(h)) cot[flp(h)] += tt;
    }
    return HE_OK;
}
static int compute_lb(T *q, He *he, const real *x, /**/ real *lbx ) {
  enum {X, Y, Z};
  int i, j, h, n, nv, nh;
  real a[3], b[3], c[3], u[3];
  real *area, *cot;
  real tt;

  nh = he_nh(he);
  nv = he_nv(he);

  area = q->area;
  cot  = q->cot;

  zero(nv, lbx);

  for (h = 0; h < nh; h++) {
    n = nxt(h);
    i = ver(h); j = ver(n);
    lbx[i] -= cot[h]*(x[i] - x[j])/2;
  }

  for (i = 0; i < nv; i++ ) {
    lbx[i] /=area[i];
  }

  return HE_OK;

}
static int compute_norm(T *q, He *he,
                        const real *x, const real *y, const real *z, /**/
                        real *normx, real *normy, real *normz) {
    enum {X, Y, Z};
    int t, nt;
    int i, j, k, nv;
    real a[3], b[3], c[3], u[3], u0[3];
    int *T0, *T1, *T2;
    real theta_a, theta_b, theta_c;

    nt = he_nt(he);
    nv = he_nv(he);
    T0 = q->T0;
    T1 = q->T1;
    T2 = q->T2;

    zero(nv, normx); zero(nv, normy); zero(nv, normz);
    for ( t = 0; t < nt; t++ ) {
        i = T0[t]; j = T1[t]; k = T2[t];
        get3(x, y, z, i, j, k, a, b, c);
        theta_a = tri_angle(c, a, b);
        theta_b = tri_angle(a, b, c);
        theta_c = tri_angle(b, c, a);
        tri_normal(a, b, c, u);
        vec_scalar_append(u, theta_a, i, normx, normy, normz);
        vec_scalar_append(u, theta_b, j, normx, normy, normz);
        vec_scalar_append(u, theta_c, k, normx, normy, normz);
    }

    for (i = 0; i < nv; i++) {
        vec_get(i, normx, normy, normz, /**/ u);
        vec_norm(u, /**/ u0);
        vec_negative(u0, u); /*This reverses the sign of norm to be inwards*/
        vec_set(u, i, /**/ normx, normy, normz);
    }
    return HE_OK;
}
static int compute_curva_mean(T *q, He *he, /**/ real *curva_mean) {
    enum {X, Y, Z};
    int i, nv;
    real *lbx, *lby, *lbz;
    real *normx, *normy, *normz;
    real u[3], v[3];

    nv  = he_nv(he);
    lbx = q->lbx;
    lby = q->lby;
    lbz = q->lbz;

    normx = q->normx;
    normy = q->normy;
    normz = q->normz;

    for ( i = 0; i < nv; i++ ) {
        vec_get(i, lbx, lby, lbz, u);
        vec_get(i, normx, normy, normz, v);
        curva_mean[i] = vec_dot(u, v)/2;
    }

    return HE_OK;

}
static int compute_curva_gauss(T *q, He *he,
                                  const real *x, const real *y, const real *z, /**/
                                  real *curva_gauss) {

    int *T0, *T1, *T2;
    real *area;
    int t, nt;
    int i, j, k, nv;
    real a[3], b[3], c[3];
    real theta_a, theta_b, theta_c;

    nt = he_nt(he);
    nv = he_nv(he);
    T0 = q->T0;
    T1 = q->T1;
    T2 = q->T2;
    area = q->area;
    zero(nv,  curva_gauss);

    for ( t = 0; t < nt; t++ ) {
        i = T0[t]; j = T1[t]; k = T2[t];

        get3(x, y, z, i, j, k, a, b, c);

        theta_a = tri_angle(c, a, b);
        theta_b = tri_angle(a, b, c);
        theta_c = tri_angle(b, c, a);

        curva_gauss[i] -= theta_a;
        curva_gauss[j] -= theta_b;
        curva_gauss[k] -= theta_c;
    }

    for ( i = 0; i < nv; i++ ) {
        curva_gauss[i] = ( curva_gauss[i] + 2 * pi ) / area[i];
//        if (curva_gauss[i] >  5) curva_gauss[i] = 5;
//        if (curva_gauss[i] < -5) curva_gauss[i] = -5;

    }

    return HE_OK;
}
real he_f_meyer_xin_energy(T *q, He *he,
                       const real *x, const real *y, const real *z) {
  enum {X, Y, Z};
  int v, t;
  int i, j, k;
  int *T0, *T1, *T2;
  real *lbx, *lby, *lbz;
  real *normx, *normy, *normz;
  real *curva_mean;
  real *energy_local, *area, *cot;

  real Kb, C0, Kad, DA0D;
  int  nv, nt;

  real H0;
  real mH0, mH1, mH2;
  real energy1, energy2, energy3, energy4, energy5, energy6;
  real energy_tot;
  real energy_tot_local, energy_tot_nonlocal;

  Kb   = q->Kb;
  C0   = q->C0;
  Kad  = q->Kad;
  DA0D = q->DA0D;

  H0  = C0/2.0;

  nv = he_nv(he);
  nt = he_nt(he);

  if (nv != q->nv )
    ERR(HE_INDEX, "he_nv(he)=%d != nv = %d", nv, q->nv);
  if (nt != q->nt )
        ERR(HE_INDEX, "he_nt(he)=%d != nt = %d", nt, q->nt);

  T0 = q->T0; T1 = q->T1; T2 = q->T2;
  lbx = q->lbx; lby = q->lby; lbz = q->lbz;
  normx = q->normx; normy = q->normy; normz = q->normz;
  curva_mean   = q->curva_mean;
  energy_local = q->energy_local;
  area = q->area;
  cot  = q->cot;

  for (t = 0; t < nt; t++) {
    get_ijk(t, he, /**/ &i, &j, &k);
    T0[t] = i; T1[t] = j; T2[t] = k;
  }

  mH0 = q->Fare(q, he, x, y, z, area);

  compute_cot(q, he, x, y, z, cot);
  compute_lb(q, he, x, lbx);
  compute_lb(q, he, y, lby);
  compute_lb(q, he, z, lbz);
  compute_norm(q, he, x, y, z, normx, normy, normz);
  compute_curva_mean(q, he, /**/ curva_mean);

  mH1 = 0;
  mH2 = 0;

  for ( v = 0; v < nv; v++ ) {
    mH1 += curva_mean[v]*area[v];
    mH2 += curva_mean[v]*curva_mean[v]*area[v];
    energy_local[v] = 2*Kb*(curva_mean[v]-H0)*(curva_mean[v]-H0)*area[v];
  }

  energy1 = 2*Kb*mH2;
  energy2 = 2*pi*Kad*mH1*mH1/mH0;
  energy3 =-4*Kb*H0*mH1;
  energy4 =-2*pi*Kad*DA0D*mH1/mH0;
  energy5 = 2*Kb*H0*H0*mH0;
  energy6 = pi*Kad*DA0D*DA0D/2/mH0;

  energy_tot_local = energy1 + energy3 + energy5;
  energy_tot_nonlocal = energy2 + energy4 + energy6;
  energy_tot = energy1 + energy2 + energy3 + energy4 + energy5+ energy6;

  //printf("mH0, mH1, mH2: %f, %f, %f\n", mH0, mH1, mH2);
  //printf("enegy local, nonlocal: %f, %f \n", energy_tot_local, energy_tot_nonlocal);
  return energy_tot;

}
int he_f_meyer_xin_force(T *q, He *he,
                     const real *x, const real *y, const real *z, /**/
                     real *fx, real *fy, real *fz) {
    enum {X, Y, Z};
    int v, e, t;
    int i, j, k, l;
    int nv, nt, ne;
    real a[3], b[3], c[3], d[3], u[3];
    int *T0, *T1, *T2;
    int *D0, *D1, *D2, *D3;
    real coti, cotl, cotil;
    real cota, cotb, cotc;
    real *lbx, *lby, *lbz;
    real *normx, *normy, *normz;
    real *area, *cot;
    real *curva_gauss, *curva_mean;
    real *lbH;
    real fm;

    real Kb, C0, Kad, DA0D;
    real H0;
    real mH0, mH1;
    real tt;

    HeSum *sum;

    Kb   = q->Kb;
    C0   = q->C0;
    Kad  = q->Kad;
    DA0D = q->DA0D;

    H0   = C0/2.0;

    nv = he_nv(he);
    nt = he_nt(he);
    ne = he_ne(he);

    T0 = q->T0; T1 = q->T1; T2 = q->T2;
    D0 = q->D0; D1 = q->D1; D2 = q->D2; D3 = q->D3;
    cot = q->cot;
    lbx = q->lbx; lby = q->lby; lbz = q->lbz;
    normx = q->normx; normy = q->normy; normz = q->normz;
    curva_mean  = q->curva_mean;
    curva_gauss = q->curva_gauss;
    area    = q->area;
    lbH = q->lbH;

    for (t = 0; t < nt; t++) {
        get_ijk(t, he, /**/ &i, &j, &k);
        T0[t] = i; T1[t] = j; T2[t] = k;
    }

    for (e = 0; e < ne; e++) {
        get_ijkl(e, he, /**/ &i, &j, &k, &l);
        D0[e] = i; D1[e] = j; D2[e] = k; D3[e] = l;
    }
    mH0 = q->Fare(q, he, x, y, z, area);

    compute_cot(q, he, x, y, z, cot);
    compute_lb(q, he, x, lbx);
    compute_lb(q, he, y, lby);
    compute_lb(q, he, z, lbz);
    compute_norm(q, he, x, y, z, normx, normy, normz);
    compute_curva_mean(q, he, curva_mean);
    compute_curva_gauss(q, he, x, y, z, curva_gauss);

    compute_lb(q, he, curva_mean, lbH);

    he_sum_ini(&sum);


    for (v = 0; v < nv; v++) {

      fm = +2*2*Kb*(curva_mean[v]-H0)*(curva_mean[v]*curva_mean[v]+curva_mean[v]*H0-curva_gauss[v]);

      fx[v] += fm * normx[v] * area[v];
      fy[v] += fm * normy[v] * area[v];
      fz[v] += fm * normz[v] * area[v];

      fm = +2*Kb*lbH[v];

      fx[v] += fm * normx[v] * area[v];
      fy[v] += fm * normy[v] * area[v];
      fz[v] += fm * normz[v] * area[v];

      he_sum_add(sum, curva_mean[v] * area[v]);
    }

    mH1 = he_sum_get(sum);
    he_sum_fin(sum);

    tt = 2*mH1-DA0D;

    for ( v = 0; v < nv; v++ ) {

      fm = -pi*Kad*(tt*2*curva_gauss[v]/mH0 - tt*tt*curva_mean[v]/mH0/mH0);
      fx[v] += fm * normx[v] * area[v];
      fy[v] += fm * normy[v] * area[v];
      fz[v] += fm * normz[v] * area[v];

    }


    return HE_OK;
}
