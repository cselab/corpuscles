#include <stdio.h>
#include <math.h>

#include "real.h"
#include "he/memory.h"
#include "he/err.h"
#include "he/he.h"
#include "he/vec.h"
#include "he/dih.h"
#include "he/ddih.h"
#include "he/tri.h"

#include "he/f/meyer.h"

#define T HeFMeyer

#    define  nxt(h)     he_nxt(he, h)
#    define  flp(h)     he_flp(he, h)
#    define  ver(h)     he_ver(he, h)
#    define  hdg_ver(v) he_hdg_ver(he, v)
#    define  hdg_edg(e) he_hdg_edg(he, e)
#    define  hdg_tri(t) he_hdg_tri(he, t)
#    define  bnd(h)     he_bnd(he, h)

static const real pi = 3.141592653589793115997964;
static const real epsilon = 1.e-2;

struct T {
  real Kb, C0, Kad, DA0D;
  int *T0, *T1, *T2;
  int *D0, *D1, *D2, *D3;
  real *lbx, *lby, *lbz;
  real *normx, *normy, *normz;
  real *curva_mean, *curva_gauss;
  real *energy, *area;
};

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
static real sum(int n, real *volume) {
  int t;
  real v;
  v = 0;
  for (t = 0; t < n; t++) v += volume[t];
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
int he_f_meyer_ini(real Kb, real C0, real Kad, real DA0D, He *he, T **pq) {
  T *q;
  int nv, nt, ne;
  MALLOC(1, &q);
  nv = he_nv(he);
  nt = he_nt(he);
  ne = he_ne(he);

  q->Kb   = Kb;
  q->C0   = C0;
  q->Kad  = Kad;
  q->DA0D = DA0D;

  MALLOC(nt, &q->T0); MALLOC(nt, &q->T1); MALLOC(nt, &q->T2);
  MALLOC(ne, &q->D0); MALLOC(ne, &q->D1); MALLOC(ne, &q->D2); MALLOC(ne, &q->D3);
  MALLOC(nv, &q->lbx); MALLOC(nv, &q->lby); MALLOC(nv, &q->lbz);
  MALLOC(nv, &q->normx); MALLOC(nv, &q->normy); MALLOC(nv, &q->normz);
  MALLOC(nv, &q->curva_mean);  MALLOC(nv, &q->curva_gauss);
  MALLOC(nv, &q->energy); MALLOC(nv, &q->area);

  *pq = q;
  return HE_OK;
}
int he_f_meyer_fin(T *q) {
  FREE(q->T0); FREE(q->T1); FREE(q->T2);
  FREE(q->D0); FREE(q->D1); FREE(q->D2); FREE(q->D3);
  FREE(q->lbx); FREE(q->lby); FREE(q->lbz);
  FREE(q->normx);FREE(q->normy);FREE(q->normz);
  FREE(q->curva_mean);FREE(q->curva_gauss);
  FREE(q->energy); FREE(q->area);
  return HE_OK;
}
int he_f_meyer_area_ver(T *q, /**/ real **pa) {
  *pa = q->area;
  return HE_OK;
}
int he_f_meyer_laplace_ver(T *q, /**/ real **px, real **py, real **pz ) {
    *px = q->lbx;
    *py = q->lby;
    *pz = q->lbz;
    return HE_OK;
}
int he_f_meyer_norm_ver(T *q, /**/ real **px, real **py, real **pz ) {
  *px = q->normx;
  *py = q->normy;
  *pz = q->normz;
  return HE_OK;
}
int he_f_meyer_curva_mean_ver(T *q, /**/ real **pa) {
  *pa = q->curva_mean;
  return HE_OK;
}
int he_f_meyer_curva_gauss_ver(T *q, real **pa) {
  *pa = q->curva_gauss;
  return HE_OK;
}
int he_f_meyer_energy_ver(T *q, /**/ real**pa) {
  *pa = q->energy;
  return HE_OK;
}
static real he_f_meyer_area(T *q, He *he,
                            const real *XX, const real *YY, const real *ZZ, /**/
                            real *area) {
  enum {X, Y, Z};
  int t, nt, nv;
  int i, j, k;
  real a[3], b[3], c[3], u[3];
  int *T0, *T1, *T2;
  real area0;
  real theta_a, theta_b, theta_c;
  real cota,cotb,cotc;
  real ab2, bc2, ca2;

  real area_tot_tri;
  real area_tot_mix;

  nt = he_nt(he);
  nv = he_nv(he);
  T0 = q->T0; T1 = q->T1; T2 = q->T2;

  for ( i = 0; i < nv; i ++ ) {
    area[i] = 0;
  }

  for ( t = 0; t < nt; t++ ) {

    i = T0[t]; j = T1[t]; k = T2[t];

    get3(XX, YY, ZZ, i, j, k, a, b, c);
    area0 = tri_area(a, b, c);

    area_tot_tri += area0;

    theta_a = tri_angle(c, a, b);
    theta_b = tri_angle(a, b, c);
    theta_c = tri_angle(b, c, a);

    /*check if the triangle has an obtuse angle*/
    if ( theta_a > pi/2.0 || theta_b > pi/2.0 || theta_c > pi/2.0 ) {
      /*check if angle b is obtuse*/
      if ( theta_a > pi/2.0 ) {
        area[i] += area0/2;
        area[j] += area0/4;
        area[k] += area0/4;
      }
      /*check if angle b is obtuse*/
      else if ( theta_b > pi/2.0 ) {
        area[j] += area0/2;
        area[i] += area0/4;
        area[k] += area0/4;
      }
      /*check if angle c is obtuse*/
      else if ( theta_c > pi/2.0 ) {
        area[k] += area0/2;
        area[i] += area0/4;
        area[j] += area0/4;
      }
    }
    /*no obtuse angle at al*/
    else {
      vec_minus(a, b,  u);
      ab2 = vec_dot(u, u);

      vec_minus(b, c, u);
      bc2 = vec_dot(u, u);

      vec_minus(c, a,  u);
      ca2 = vec_dot(u, u);

      cota = tri_cot(c, a, b);
      cotb = tri_cot(a, b, c);
      cotc = tri_cot(b, c, a);

      area[i] += ( ab2*cotc + ca2*cotb ) / 8;
      area[j] += ( bc2*cota + ab2*cotc ) / 8;
      area[k] += ( ca2*cotb + bc2*cota ) / 8;
    }

  }/*end for loop*/

  area_tot_mix = sum(nv, area);

  return area_tot_tri;

}
static int he_f_meyer_laplace(T *q, He *he,
                            const real *XX, const real *YY, const real *ZZ, /**/
                            real *lbx, real *lby, real *lbz) {
  enum {X, Y, Z};
  int t, nt;
  int i, j, k, nv;
  real a[3], b[3], c[3], u[3];
  int *T0, *T1, *T2;
  real *area;
  real area0;
  real theta_a, theta_b, theta_c;
  real cota,cotb,cotc;

  nt = he_nt(he);
  nv = he_nv(he);
  T0 = q->T0; T1 = q->T1; T2 = q->T2;
  area = q->area;

  for ( i = 0; i < nv; i ++ ) {
    lbx[i] = 0;
    lby[i] = 0;
    lbz[i] = 0;
  }

  for ( t = 0; t < nt; t++ ) {

    i = T0[t];
    j = T1[t];
    k = T2[t];

    get3(XX, YY, ZZ, i, j, k, a, b, c);

    cota = tri_cot(c, a, b);
    cotb = tri_cot(a, b, c);
    cotc = tri_cot(b, c, a);

    vec_minus(a, b,  u);

    lbx[i] += cotc*u[X]/2;
    lby[i] += cotc*u[Y]/2;
    lbz[i] += cotc*u[Z]/2;

    lbx[j] -= cotc*u[X]/2;
    lby[j] -= cotc*u[Y]/2;
    lbz[j] -= cotc*u[Z]/2;

    vec_minus(b, c, u);

    lbx[j] += cota*u[X]/2;
    lby[j] += cota*u[Y]/2;
    lbz[j] += cota*u[Z]/2;

    lbx[k] -= cota*u[X]/2;
    lby[k] -= cota*u[Y]/2;
    lbz[k] -= cota*u[Z]/2;

    vec_minus(c, a,  u);

    lbx[k] += cotb*u[X]/2;
    lby[k] += cotb*u[Y]/2;
    lbz[k] += cotb*u[Z]/2;

    lbx[i] -= cotb*u[X]/2;
    lby[i] -= cotb*u[Y]/2;
    lbz[i] -= cotb*u[Z]/2;

  }

  for ( i = 0; i < nv; i++ ) {

    lbx[i] /=area[i];
    lby[i] /=area[i];
    lbz[i] /=area[i];

  }

  return HE_OK;

}
static int he_f_meyer_norm(T *q, He *he,
                            const real *XX, const real *YY, const real *ZZ, /**/
                            real *normx, real *normy, real *normz) {
  enum {X, Y, Z};
  int t, nt;
  int i, j, k, nv;
  real a[3], b[3], c[3], u[3];
  int *T0, *T1, *T2;
  real *area;
  real area0;
  real theta_a, theta_b, theta_c;
  real len;

  nt = he_nt(he);
  nv = he_nv(he);
  T0 = q->T0;
  T1 = q->T1;
  T2 = q->T2;

  for ( i = 0; i < nv; i++ ) {
    normx[i] = 0;
    normy[i] = 0;
    normz[i] = 0;
  }

  for ( t = 0; t < nt; t++ ) {
    i = T0[t]; j = T1[t]; k = T2[t];
    get3(XX, YY, ZZ, i, j, k, a, b, c);

    theta_a = tri_angle(c, a, b);
    theta_b = tri_angle(a, b, c);
    theta_c = tri_angle(b, c, a);

    tri_normal(a, b, c, u);
    vec_scalar_append(u, theta_a, i, normx, normy, normz);
    vec_scalar_append(u, theta_b, j, normx, normy, normz);
    vec_scalar_append(u, theta_c, k, normx, normy, normz);
  }

  for ( i = 0; i < nv; i++ ) {
      vec_get(i, normx, normy, normz, /**/ u);

      len = vec_dot(u, u);
      len = sqrt(len);

      normx[i] = normx[i]/len;
      normy[i] = normy[i]/len;
      normz[i] = normz[i]/len;
  }

 return HE_OK;

}
static int he_f_meyer_curva_mean(T *q, He *he,
                            const real *XX, const real *YY, const real *ZZ, /**/
                            real *curva_mean) {
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
    u[X] = lbx[i];
    u[Y] = lby[i];
    u[Z] = lbz[i];
    v[X] = normx[i];
    v[Y] = normy[i];
    v[Z] = normz[i];
    curva_mean[i] = vec_dot(u, v)/2;
  }

  return HE_OK;

}
static int he_f_meyer_curva_gauss(T *q, He *he,
                            const real *XX, const real *YY, const real *ZZ, /**/
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

  for ( i = 0; i < nv; i ++ ) {

    curva_gauss[i] = 0;

  }

  for ( t = 0; t < nt; t++ ) {
    i = T0[t]; j = T1[t]; k = T2[t];

    get3(XX, YY, ZZ, i, j, k, a, b, c);

    theta_a = tri_angle(c, a, b);
    theta_b = tri_angle(a, b, c);
    theta_c = tri_angle(b, c, a);

    curva_gauss[i] -= theta_a;
    curva_gauss[j] -= theta_b;
    curva_gauss[k] -= theta_c;
  }

  for ( i = 0; i < nv; i++ ) {

    curva_gauss[i] = ( curva_gauss[i] + 2 * pi ) / area[i];
  }

  return HE_OK;
}
real he_f_meyer_energy(T *q, He *he,
                       const real *XX, const real *YY, const real *ZZ) {
  enum {X, Y, Z};
  int n;
  int v, t;
  int i, j, k;
  real a[3], b[3], c[3], u[3], uu[3], coord[3];
  int *T0, *T1, *T2;
  real *lbx, *lby, *lbz;
  real *normx, *normy, *normz;
  real *curva_mean, *curva_gauss;
  real *energy, *area;

  real Kb, C0, Kad, DA0D;
  real len, area0;
  real rxy, phi;
  int  obtuse;
  real theta0;
  real theta_a, theta_b, theta_c;
  real cota,cotb,cotc;
  real ab2, bc2, ca2;
  real area_tot_tri, area_tot_mix;
  int  nv, nt;

  real H0, cm_intga;
  real energy1, energy2, energy3a, energy3b, energy4, energy5;
  real energy_tot;

  Kb   = q->Kb;
  C0   = q->C0;
  Kad  = q->Kad;
  DA0D = q->DA0D;

  H0  = C0/2.0;

  nv = he_nv(he);
  nt = he_nt(he);

  T0 = q->T0; T1 = q->T1; T2 = q->T2;
  lbx = q->lbx; lby = q->lby; lbz = q->lbz;
  normx = q->normx; normy = q->normy; normz = q->normz;
  curva_mean  = q->curva_mean;
  energy  = q->energy;
  area    = q->area;

  for (t = 0; t < nt; t++) {
    get_ijk(t, he, /**/ &i, &j, &k);
    T0[t] = i; T1[t] = j; T2[t] = k;
  }

  for (v = 0; v < nv; v++) {
    normx[v] = 0; normy[v] = 0; normz[v] = 0;
    lbx[v] = 0; lby[v] = 0; lbz[v] = 0;
    curva_mean[v] = 0;
    energy[v] = 0; area[v] = 0;
  }

  area_tot_tri = he_f_meyer_area(q, he, XX, YY, ZZ, area);
  he_f_meyer_laplace(q, he, XX, YY, ZZ, lbx, lby, lbz);
  he_f_meyer_norm(q, he, XX, YY, ZZ, normx, normy, normz);
  he_f_meyer_curva_mean(q, he, XX, YY, ZZ, curva_mean);

  energy_tot = 0;
  cm_intga   = 0;

  for ( v = 0; v < nv; v++ ) {
    energy[v] = 2 * Kb* curva_mean[v] * curva_mean[v] * area[v];
    cm_intga += curva_mean[v] * area[v];
  }

  energy1 = sum(nv, energy);

  energy2 = 2*pi*Kad*cm_intga*cm_intga/area_tot_tri;

  energy3a =  -4*Kb*H0*cm_intga;

  energy3b =  -2*pi*Kad*DA0D*cm_intga;

  energy4 = 2*Kb*H0*H0*area_tot_tri;

  energy5 = pi*Kad*DA0D*DA0D/2/area_tot_tri;

  energy_tot = energy1 + energy2 + energy3a + energy3b + energy4 + energy5;

  return energy_tot;

}
int he_f_meyer_force(T *q, He *he,
                     const real *XX, const real *YY, const real *ZZ, /**/
                     real *fx, real *fy, real *fz) {
  enum {X, Y, Z};
  int v, e, t;
  int i, j, k, l;
  int nv, nt, ne;
  real a[3], b[3], c[3], d[3], u[3], n[3];
  int *T0, *T1, *T2;
  int *D0, *D1, *D2, *D3;
  real coti, cotl, cotil;
  real *lbx, *lby, *lbz;
  real *normx, *normy, *normz;
  real *area;
  real *curva_gauss, *curva_mean;
  real fm;
  //real *fxad, *fyad, *fzad;
  real area0;
  int  obtuse;
  real thetaa, thetab, thetac;
  real cota,cotb,cotc;
  real ab2, bc2, ca2;
  real area_tot_tri, area_tot_mix, curva_mean_area_tot;

  real Kb, C0, Kad, DA0D;
  real H0, cm_intga;

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
  lbx = q->lbx; lby = q->lby; lbz = q->lbz;
  normx = q->normx; normy = q->normy; normz = q->normz;
  curva_mean  = q->curva_mean;
  curva_gauss = q->curva_gauss;
  area    = q->area;

  for (t = 0; t < nt; t++) {
    get_ijk(t, he, /**/ &i, &j, &k);
    T0[t] = i; T1[t] = j; T2[t] = k;
  }

  for (e = 0; e < ne; e++) {
    get_ijkl(e, he, /**/ &i, &j, &k, &l);
    D0[e] = i; D1[e] = j; D2[e] = k; D3[e] = l;
  }

  for (v = 0; v < nv; v++) {
    normx[v] = 0; normy[v] = 0; normz[v] = 0;
    lbx[v] = 0; lby[v] = 0; lbz[v] = 0;
    curva_mean[v] = 0; curva_gauss[v] = 0;
    area[v] = 0;
    fx[v] = 0; fy[v]=0; fz[v]=0;
    //fxad[v] = 0; fyad[v]=0; fzad[v]=0;
  }

  area_tot_tri = he_f_meyer_area(q, he, XX, YY, ZZ, area);
  he_f_meyer_laplace(q, he, XX, YY, ZZ, lbx, lby, lbz);
  he_f_meyer_norm(q, he, XX, YY, ZZ, normx, normy, normz);
  he_f_meyer_curva_mean(q, he, XX, YY, ZZ, curva_mean);
  he_f_meyer_curva_gauss(q, he, XX, YY, ZZ, curva_gauss);

  for ( v = 0; v < nv; v++ ) {

    fm = 2*2*Kb*(curva_mean[v]-H0)*(curva_mean[v]*curva_mean[v]+curva_mean[v]*H0-curva_gauss[v]);

    fx[v] += fm * normx[v];
    fy[v] += fm * normy[v];
    fz[v] += fm * normz[v];

    cm_intga +=  curva_mean[v] * area[v];

  }

  cm_intga -= (DA0D/2);
  cm_intga *= (4*pi* Kad/ area_tot_tri);

  for ( v = 0; v < nv; v++ ) {

    fm = -cm_intga * curva_gauss[v];
    //fxad[v] += fm * nx[v];
    //fyad[v] += fm * ny[v];
    //fzad[v] += fm * nz[v];
    fx[v] += fm * normx[v];
    fy[v] += fm * normy[v];
    fz[v] += fm * normz[v];

  }

  /*5th loop*/
  for (e = 0; e < ne; e++) {

    if (bnd(e)) continue;

    i = D0[e]; j = D1[e]; k = D2[e]; l = D3[e];

    get4(XX, YY, ZZ, i, j, k, l, /**/ a, b, c, d);

    coti = tri_cot(c, a, b);
    cotl = tri_cot(b, d, c);
    cotil = coti + cotl;

    vec_minus(b, c,  u);

    fm = Kb*cotil * (curva_mean[j] - curva_mean[k]) / area[j];
    fx[j] += fm * normx[j];
    fy[j] += fm * normy[j];
    fz[j] += fm * normz[j];

    fm = Kb*cotil * (curva_mean[k] - curva_mean[j]) / area[k];
    fx[k] += fm * normx[k];
    fy[k] += fm * normy[k];
    fz[k] += fm * normz[k];

  }

  return HE_OK;
}
