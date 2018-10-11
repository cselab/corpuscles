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

struct T {
  real Kb, Kad, Da0;
  int *T0, *T1, *T2;
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
static real sum(int n, real *volume) {
    int t;
    real v;
    v = 0;
    for (t = 0; t < n; t++) v += volume[t];
    return v;
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
int he_f_meyer_ini(real Kb, real Kad, real Da0, He *he, T **pq) {
    T *q;
    int nv, nt;
    MALLOC(1, &q);
    nv = he_nv(he);
    nt = he_nt(he);

    q->Kb  = Kb;
    q->Kad = Kad;
    q->Da0 = Da0;

    MALLOC(nt, &q->T0); MALLOC(nt, &q->T1); MALLOC(nt, &q->T2);
    MALLOC(nv, &q->lbx); MALLOC(nv, &q->lby); MALLOC(nv, &q->lbz);
    MALLOC(nv, &q->normx); MALLOC(nv, &q->normy); MALLOC(nv, &q->normz);
    MALLOC(nv, &q->curva_mean);  MALLOC(nv, &q->curva_gauss);
    MALLOC(nv, &q->energy); MALLOC(nv, &q->area);
    
    *pq = q;
    return HE_OK;
}
int he_f_meyer_fin(T *q) {
    FREE(q->T0); FREE(q->T1); FREE(q->T2);
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
int he_f_meyer_curva_gauss_ver(T *q, real **curva_gauss) {
  *curva_gauss = q->curva_gauss;
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
  int t, NT, NV;
  int i, j, k;
  real a[3], b[3], c[3], u[3];
  int *T0, *T1, *T2;    
  real area0;
  real theta_a, theta_b, theta_c;
  real cota,cotb,cotc;
  real ab2, bc2, ca2;

  real area_tot_tri;
  real area_tot_mix;
  
  NT = he_nt(he);
  NV = he_nv(he);
  T0 = q->T0; T1 = q->T1; T2 = q->T2;
  
  for ( t = 0; t < NT; t++ ) {
    
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

  area_tot_mix = sum(NV, area);

  return area_tot_tri;
  
}
static int he_f_meyer_laplace(T *q, He *he,
			    const real *XX, const real *YY, const real *ZZ, /**/
			    real *lbx, real *lby, real *lbz) {
  enum {X, Y, Z};
  int t, NT;
  int i, j, k, NV;
  real a[3], b[3], c[3], u[3];
  int *T0, *T1, *T2;
  real *area;  
  real area0;
  real theta_a, theta_b, theta_c;
  real cota,cotb,cotc;
  real ab2, bc2, ca2;

  NT = he_nt(he);
  NV = he_nv(he);
  T0 = q->T0; T1 = q->T1; T2 = q->T2;
  area = q->area;
  
  for ( t = 0; t < NT; t++ ) {
    
    i = T0[t]; j = T1[t]; k = T2[t];
    
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
    
    ab2 = vec_dot(u, u);
    
    vec_minus(b, c, u);
    
    lbx[j] += cota*u[X]/2;
    lby[j] += cota*u[Y]/2;
    lbz[j] += cota*u[Z]/2;
    
    lbx[k] -= cota*u[X]/2;
    lby[k] -= cota*u[Y]/2;
    lbz[k] -= cota*u[Z]/2;
    
    bc2 = vec_dot(u, u);
    
    vec_minus(c, a,  u);
    
    lbx[k] += cotb*u[X]/2;
    lby[k] += cotb*u[Y]/2;
    lbz[k] += cotb*u[Z]/2;
    
    lbx[i] -= cotb*u[X]/2;
    lby[i] -= cotb*u[Y]/2;
    lbz[i] -= cotb*u[Z]/2;
    
    ca2 = vec_dot(u, u);
    
  }

  for ( i = 0; i < NV; i++ ) {
    
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
  int t, NT;
  int i, j, k, NV;
  real a[3], b[3], c[3], u[3];
  int *T0, *T1, *T2;    
  real *area;
  real area0;
  real theta_a, theta_b, theta_c;
  real len;
  
  NT = he_nt(he);
  NV = he_nv(he);
  T0 = q->T0; T1 = q->T1; T2 = q->T2;
  
  for ( t = 0; t < NT; t++ ) {    
    i = T0[t]; j = T1[t]; k = T2[t];
    
    get3(XX, YY, ZZ, i, j, k, a, b, c);
    
    theta_a = tri_angle(c, a, b);
    theta_b = tri_angle(a, b, c);
    theta_c = tri_angle(b, c, a);

    tri_normal(a, b, c, u);
    
    normx[i] += theta_a * u[X];
    normy[i] += theta_a * u[Y];
    normz[i] += theta_a * u[Z];
    
    normx[j] += theta_b * u[X];
    normy[j] += theta_b * u[Y];
    normz[j] += theta_b * u[Z];
    
    normx[k] += theta_c * u[X];
    normy[k] += theta_c * u[Y];
    normz[k] += theta_c * u[Z];    
  }

  for ( i = 0; i < NV; i++ ) {    
    u[X] = normx[i];
    u[Y] = normy[i];
    u[Z] = normz[i];
    
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
  int i, NV;
  real *lbx, *lby, *lbz;
  real *normx, *normy, *normz;
  real u[3], v[3];

  NV = nv(he);
  lbx = q->lbx;
  lby = q->lby;
  lbz = q->lbz;

  normx = q->normx;
  normy = q->normy;
  normz = q->normz;
  
  for ( i = 0; i < NV; i++ ) {
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
  int t, NT;
  int i, j, k, NV;
  real a[3], b[3], c[3];
  real theta_a, theta_b, theta_c;
  
  NT = he_nt(he);
  NV = he_nv(he);
  T0 = q->T0; T1 = q->T1; T2 = q->T2;
  area = q->area;
  
  for ( t = 0; t < NT; t++ ) {    
    i = T0[t]; j = T1[t]; k = T2[t];
    
    get3(XX, YY, ZZ, i, j, k, a, b, c);
    
    theta_a = tri_angle(c, a, b);
    theta_b = tri_angle(a, b, c);
    theta_c = tri_angle(b, c, a);

    curva_gauss[i] -= theta_a;
    curva_gauss[j] -= theta_b;
    curva_gauss[k] -= theta_c;
  }
  
  for ( i = 0; i < NV; i++ ) {    
    curva_gauss[i] = ( curva_gauss[i] + 2 * pi ) / area[i];    
  }
  
  return HE_OK;
}
static void compute_force(real K,
                          He *he, const real *x, const real *y, const real *z, /**/
                          real *fx, real *fy, real *fz){
}
int he_f_meyer_force(T *q, He *he,
                     const real *x, const real *y, const real *z, /**/
                     real *fx, real *fy, real *fz) {
   return HE_OK;
}
real he_f_meyer_energy(T *q, He *he,
                       const real *XX, const real *YY, const real *ZZ) {
  enum {X, Y, Z};
  int n;
  real *acos; //, K;
  int v, t;
  int i, j, k;
  real a[3], b[3], c[3], u[3], uu[3], coord[3];
  int *T0, *T1, *T2;    
  real *lbx, *lby, *lbz, *normx, *normy, *normz;
  real *curva_mean, *curva_gauss;
  real *energy, *area;
  
  real Kb, Kad, Da0;
  real len, area0;
  real rxy, phi;
  int  obtuse;
  real theta0;
  real theta_a, theta_b, theta_c;
  real cota,cotb,cotc;
  real ab2, bc2, ca2;
  real area_tot_tri, area_tot_mix;
  int  NV, NT;

  real C0, H0, D, DA0, cm_intga;
  real energy1, energy2, energy3, energy4, energy5;
  real energy_tot;

  Kb  = q->Kb;
  Kad = q->Kad;
  Da0 = q->Da0;

  Kb  = 1.0;
  Kad = 2*Kb/pi;

  /*define and work out backwards for auxiliary parameters*/
  C0  = -1.0;
  H0  = C0/2.0;
  D   = 3.0e-3/3.91;
    
  NV = he_nv(he);
  NT = he_nt(he);
  
  T0 = q->T0; T1 = q->T1; T2 = q->T2;
  lbx = q->lbx; lby = q->lby; lbz = q->lbz;
  normx = q->normx; normy = q->normy; normz = q->normz;
  curva_mean  = q->curva_mean;
  curva_gauss = q->curva_gauss;
  energy  = q->energy;
  area    = q->area;
  
  for (t = 0; t < NT; t++) {
    get_ijk(t, he, /**/ &i, &j, &k);
    T0[t] = i; T1[t] = j; T2[t] = k;
  }
  
  for (v = 0; v < NV; v++) {
    normx[v] = 0; normy[v] = 0; normz[v] = 0;
    lbx[v] = 0; lby[v] = 0; lbz[v] = 0;
    curva_mean[v] = 0; curva_gauss[v] = 0;      
    energy[v] = 0; area[v] = 0;
  }

  area_tot_tri = he_f_meyer_area(q, he, XX, YY, ZZ, area);
  he_f_meyer_laplace(q, he, XX, YY, ZZ, lbx, lby, lbz);
  he_f_meyer_norm(q, he, XX, YY, ZZ, normx, normy, normz);
  he_f_meyer_curva_mean(q, he, XX, YY, ZZ, curva_mean);
  he_f_meyer_curva_gauss(q, he, XX, YY, ZZ, curva_gauss);

  energy_tot = 0;
  cm_intga   = 0;
  
  for ( v = 0; v < NV; v++ ) {
    energy[v] = 2 * curva_mean[v] * curva_mean[v] * area[v];
    cm_intga +=  curva_mean[v] * area[v];
  }

  DA0 = (Da0 - 2*Kb*H0*D/pi/Kad) * area_tot_tri;
  
  energy2 = 2*pi*Kad*cm_intga*cm_intga/area_tot_tri;
  energy3 = -2*pi*Kad*Da0*cm_intga/D;
  //energy4 = 2*Kb*H0*H0*area_tot_tri;
  //energy5 = pi*Kad*DA0*DA0/2/area_tot_tri/D/D;
  
  energy1 =  Kb*sum(NV, energy);

  energy_tot = energy1 + energy2 + energy3;
  //energy_tot = energy1 + energy2 + energy3 + energy4 + energy5;
  
  return energy_tot;
  
}
