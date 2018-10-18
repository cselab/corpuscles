#include <stdio.h>
#include <math.h>

#include "real.h"
#include "he/memory.h"
#include "he/err.h"
#include "he/he.h"
#include "he/vec.h"
#include "he/tri.h"
#include "he/dtri.h"

#include "he/f/gompper_kroll.h"

#define T HeFGompper_Kroll

#define  nxt(h)     he_nxt(he, (h))
#define  flp(h)     he_flp(he, (h))
#define  ver(h)     he_ver(he, (h))
#define  hdg_ver(v) he_hdg_ver(he, (v))
#define  hdg_edg(e) he_hdg_edg(he, (e))
#define  hdg_tri(e) he_hdg_tri(he, (e))
#define  bnd(h)     he_bnd(he, (h))

static const real pi = 3.141592653589793115997964;
static const real epsilon = 1.0e-12;

struct T {
  real Kb, C0, Kad, DA0D;

  int *T0, *T1, *T2;
  
  real *len2, *cot;
  real *lbx, *lby, *lbz;
  real *normx, *normy, *normz;
  real *curva_mean, *curva_gauss;
  real *energy, *area;
 
  int nv, ne, nt, nh;
};
static void get_edg(const real *x, const real *y, const real *z,
		    int i, int j,
		    /**/ real r[3]) {
    real a[3], b[3];
    vec_get(i, x, y, z, a);
    vec_get(j, x, y, z, b);
    vec_minus(a, b, r);
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
void get4(const real *x, const real *y, const real *z,
	  int i, int j, int k, int l, /**/
          real a[3], real b[3], real c[3], real d[3]) {
  
  /*Given four indices i, j, k l of vertices,
    return their positions in a, b, c, d.*/
  vec_get(i, x, y, z, a);
  vec_get(j, x, y, z, b);
  vec_get(k, x, y, z, c);
  vec_get(l, x, y, z, d);  
}
static void zero(int n, real *a) {
    int i;
    for (i = 0; i < n; i++) a[i] = 0;
}
static real sum(int n, const  real *volume) {
    int t;
    real v;
    v = 0;
    for (t = 0; t < n; t++) v += volume[t];
    return v;
}
int he_f_gompper_kroll_ini(real Kb, real C0, real Kad, real DA0D, He *he, T **pq) {
  T *q;
  int nv, ne, nt, nh;
  MALLOC(1, &q);

  //printf("size of q %g", sizeof(q));
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
  
  MALLOC(nt, &q->T0); MALLOC(nt, &q->T1); MALLOC(nt, &q->T2);

  MALLOC(nh, &q->len2);
  MALLOC(nh, &q->cot);

  MALLOC(nv, &q->lbx); MALLOC(nv, &q->lby); MALLOC(nv, &q->lbz);
  MALLOC(nv, &q->normx); MALLOC(nv, &q->normy); MALLOC(nv, &q->normz);
  MALLOC(nv, &q->curva_mean);  MALLOC(nv, &q->curva_gauss);

  MALLOC(nv, &q->energy);
  MALLOC(nv, &q->area);
  
  *pq = q;
  return HE_OK;
}
int he_f_gompper_kroll_fin(T *q) {
  FREE(q->T0); FREE(q->T1); FREE(q->T2);
  FREE(q->len2); FREE(q->cot); 
  FREE(q->lbx); FREE(q->lby); FREE(q->lbz);
  FREE(q->normx);FREE(q->normy);FREE(q->normz);
  FREE(q->curva_mean);FREE(q->curva_gauss);
  FREE(q->energy); FREE(q->area);
  FREE(q);
  return HE_OK;
}
int he_f_gompper_kroll_area_ver(T *q, /**/ real **pa) {
  *pa = q->area;
  return HE_OK;
}
int he_f_gompper_kroll_laplace_ver(T *q, /**/ real **px, real **py, real **pz ) {
    *px = q->lbx;
    *py = q->lby;
    *pz = q->lbz;
    return HE_OK;
}
int he_f_gompper_kroll_norm_ver(T *q, /**/ real **px, real **py, real **pz ) {
  *px = q->normx;
  *py = q->normy;
  *pz = q->normz;
  return HE_OK;
}
int he_f_gompper_kroll_curva_mean_ver(T *q, /**/ real **pa) {
  *pa = q->curva_mean;
  return HE_OK;
}
int he_f_gompper_kroll_curva_gauss_ver(T *q, /**/ real **pa) {
  *pa = q->curva_gauss;
  return HE_OK;
}
int he_f_gompper_kroll_energy_ver(T *q, /**/ real**pa) {
  *pa = q->energy;
  return HE_OK;
}
static void compute_len2(He *he, const real *x, const real *y, const real *z, /**/ real *H) {
    int nh;
    int h, n;
    int i, j;
    real r[3];
    nh = he_nh(he);
    for (h = 0; h < nh; h++) {
        n = nxt(h);
        i = ver(h); j = ver(n);
        get_edg(x, y, z, i, j, /**/ r);
        H[h] = vec_dot(r, r);
    }
}
static void compute_cot(He *he, const real *x, const real *y, const real *z, /**/ real *H) {
    int nh, h, n, nn;
    int i, j, k;
    real a[3], b[3], c[3], cot;
    nh = he_nh(he);
    zero(nh, H);
    for (h = 0; h < nh; h++) {
        n = nxt(h); nn = nxt(n);
        i = ver(h); j = ver(n); k = ver(nn);
        get3(x, y, z, i, j, k, /**/ a, b, c);
        cot = tri_cot(b, c, a);
        H[h] += cot;
        if (!bnd(h)) H[flp(h)] += cot;
    }
}
static void compute_area_voronoi(He *he, const real *len2, const real *cot, /**/ real *V) {
    int nv, nh, h, i;
    nv = he_nv(he);
    zero(nv, V);
    nh = he_nh(he);
    for (h = 0; h < nh; h++) {
        i = ver(h);
        V[i] += cot[h]*len2[h]/8;
    }
}
static void compute_laplace(He *he, const real *V0, const real *cot, const real *area, /**/ real *V1) {
    int h, n, nv, nh, i, j;
    nv = he_nv(he);
    zero(nv, V1);
    nh = he_nh(he);
    for (h = 0; h < nh; h++) {
        n = nxt(h);
        i = ver(h); j = ver(n);
        V1[i] += cot[h]*(V0[i] - V0[j])/2;
    }
    for (i = 0; i < nv; i++)
        V1[i] /= area[i];
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
        vec_set(u0, i, /**/ normx, normy, normz);
    }
    return HE_OK;
}
static int compute_curva_mean(T *q, He *he,
			      real *lbx, real *lby, real *lbz,
			      real *normx, real *normy, real *normz,
			      /**/ real *curva_mean) {
  enum {X, Y, Z};
  int i, nv;
  real u[3], v[3];
  
  nv  = he_nv(he);
  
  for ( i = 0; i < nv; i++ ) {
    vec_get(i, lbx, lby, lbz, u);
    vec_get(i, normx, normy, normz, v);
    curva_mean[i] = vec_dot(u, v)/2;
  }
  
  return HE_OK;
  
}
static real compute_energy_local(T *q, const real *curva_mean, const real *area, /**/ real *energy) {
  real Kb, C0, H0;
  int i, nv;
  real energy_tot;
  
  Kb   = q->Kb;
  C0   = q->C0;
  nv   = q->nv;
  
  H0 = C0/2.0;
  
  energy_tot = 0;
  
  for (i = 0; i < nv; i++) {
    energy[i]   = 2*Kb*(curva_mean[i]-H0)*(curva_mean[i]-H0)*area[i];
    energy_tot += energy[i];
  }
  
  return energy_tot;
}
static real compute_curva_mean_integral(T *q, const real *curva_mean, const real *area) {

  int i, nv;
  real cm_integral;
  nv   = q->nv;

  cm_integral = 0;
  for (i = 0; i < nv; i++) {
    cm_integral += curva_mean[i]*area[i];
  }
  return cm_integral;
}
static real compute_energy_nonlocal(T *q, const real *curva_mean, const real *area) {
  
  real Kad, DA0D;
  int i, nv;
  real cm_integral, area_tot, energy_tot;
  
  Kad  = q->Kad;
  DA0D = q->DA0D;
  nv   = q->nv;
  
  cm_integral=compute_curva_mean_integral(q, curva_mean, area);
  area_tot   =sum(nv, area);
  energy_tot = 4*pi*Kad/area_tot*(cm_integral - DA0D/2)*(cm_integral - DA0D/2);
  
  return energy_tot;
}
real he_f_gompper_kroll_energy(T *q, He *he,
			 const real *x, const real *y, const real *z) {
  real Kad;
  int nv, nt;
  int i, j, k, l;
  int *T0, *T1, *T2;
  
  real *len2, *cot;
  real *lbx, *lby, *lbz;
  real *normx, *normy, *normz;
  real *curva_mean;
  real *area;
  real *energy;
  real energy_tot;
  
  T0 = q->T0; T1 = q->T1; T2 = q->T2;
  len2 = q->len2; cot = q->cot;
  lbx = q->lbx; lby = q->lby; lbz = q->lbz;
  normx = q->normx; normy = q->normy; normz = q->normz;
  curva_mean  = q->curva_mean;
  area = q->area;
  energy = q->energy;
  
  Kad = q->Kad;
  nv  = q->nv;
  nt  = he_nt(he);
  
  for (l = 0; l < nt; l++) {
    get_ijk(l, he, /**/ &i, &j, &k);
    T0[l] = i; T1[l] = j; T2[l] = k;
  }
  
  if (he_nv(he) != nv)
    ERR(HE_INDEX, "he_nv(he)=%d != nv = %d", he_nv(he), nv);
  
  compute_len2(he, x, y, z, /**/ len2);
  compute_cot(he, x, y, z, /**/ cot);
  compute_area_voronoi(he, len2, cot, /**/ area);
  compute_laplace(he, x, cot, area, /**/ lbx);
  compute_laplace(he, y, cot, area, /**/ lby);
  compute_laplace(he, z, cot, area, /**/ lbz);
  compute_norm(q, he, x, y, z, normx, normy, normz);
  compute_curva_mean(q, he, lbx, lby, lbz, normx, normy, normz, /**/ curva_mean);
  
  energy_tot  = compute_energy_local(q, curva_mean, area, /**/ energy);

  if (Kad > epsilon){
    energy_tot += compute_energy_nonlocal(q, curva_mean, area);
  }
  
  return energy_tot;
}
int he_f_gompper_kroll_force(T *q, He *he,
                      const real *x, const real *y, const real *z, /**/
                      real *fx, real *fy, real *fz) {
  int nv, ne, nt, nh;
  int i, j, k, l;
  int h, n, nn, fnf;
  int *T0, *T1, *T2;

  real Kb, C0, Kad, DA0D, H0;
  real *len2, *cot;
  real *lbx, *lby, *lbz;
  real *normx, *normy, *normz;
  real *curva_mean;
  real *area;
  real cm_integral, area_tot;

  real cot1, area1, rsq;
  real a[3], b[3], c[3], d[3], r[3];
  real lbi[3], lbisq, lbisq_der[3];
  real coef, coef1, coef2;
  real doef, doef1, doef2;
  real da1[3], db1[3], dc[3];
  real da2[3], db2[3], dd[3];
  real df[3], dcm[3];
  real coef3;

  T0 = q->T0; T1 = q->T1; T2 = q->T2;
  len2 = q->len2; cot = q->cot;
  lbx = q->lbx; lby = q->lby; lbz = q->lbz;
  normx = q->normx; normy = q->normy; normz = q->normz;
  curva_mean  = q->curva_mean;
  area = q->area;

  nv = q->nv;
  ne = q->ne;
  nt = q->nt;
  nh = q->nh;
  
  Kb   = q->Kb;
  C0   = q->C0;
  Kad  = q->Kad;
  DA0D = q->DA0D;

  H0 = C0/2.0;
  
  //printf("Kb, C0, Kad, DA0D: %f, %f, %f, %f\n", Kb, C0, Kad, DA0D);
  
  if (he_nv(he) != nv)
    ERR(HE_INDEX, "he_nv(he)=%d != nv = %d", he_nv(he), nv);
  if (he_ne(he) != ne)
    ERR(HE_INDEX, "he_nh(he)=%d != ne = %d", he_ne(he), ne);
  if (he_nt(he) != nt)
    ERR(HE_INDEX, "he_nt(he)=%d != nt = %d", he_nt(he), nt);
  if (he_nh(he) != nh)
    ERR(HE_INDEX, "he_nh(he)=%d != nh = %d", he_nh(he), nh);
  
  for (l = 0; l < nt; l++) {
    get_ijk(l, he, /**/ &i, &j, &k);
    T0[l] = i; T1[l] = j; T2[l] = k;
  }
  
  
  compute_len2(he, x, y, z, /**/ len2);
  compute_cot(he, x, y, z, /**/ cot);
  compute_area_voronoi(he, len2, cot, /**/ area);
  compute_laplace(he, x, cot, area, /**/ lbx);
  compute_laplace(he, y, cot, area, /**/ lby);
  compute_laplace(he, z, cot, area, /**/ lbz);
  compute_norm(q, he, x, y, z, normx, normy, normz);
  compute_curva_mean(q, he, lbx, lby, lbz, normx, normy, normz, /**/ curva_mean);
  cm_integral=compute_curva_mean_integral(q, curva_mean, area);
  area_tot   =sum(nv, area);

  for (h = 0; h < nh; h++) {
    
    n = nxt(h); nn = nxt(n); fnf = flp(nxt(flp(h)));
    i = ver(h); j = ver(n); k = ver(nn); l = ver(fnf);
    
    cot1  = cot[h];
    area1 = area[i];
    rsq   = len2[h];

    get4(x, y, z, i, j, k, l, a, b, c, d);
    get_edg(x, y, z, i, j, r);
    rsq = vec_dot(r, r);
 
    vec_get(i, lbx, lby, lbz, lbi);    
    lbisq = vec_dot(lbi, lbi);
    
    /*###################################
      ###################################
      force part I
      ###################################
      ###################################*/
    
    /*calculate derivative of Laplace-Beltrami operator: part I*/
    /*this is to calculate the derivative of relative position of i, j*/
    
    if (fabs(curva_mean[i]) < epsilon ) {
      //printf("__FILE__, __LINE__, curva_mean too small!\n");
      curva_mean[i] = epsilon;
    }
    coef  = Kb*(curva_mean[i]-H0)*area[i]/2.0/curva_mean[i];

    coef1 =  cot1 / area1;
    coef2 = -lbisq * cot1 /area1 / 2.0;
    
    vec_linear_combination(coef1, lbi, coef2, r, lbisq_der);

    vec_scalar(lbisq_der, coef, df);
    
    /*accumulate the force on vertices i and j*/
    vec_append(df, i, /**/ fx, fy, fz);
    vec_substr(df, j, /**/ fx, fy, fz);

    /*+++++++++++++++++++++++++++++++++++
      force due to area-difference elasticity: part I
      +++++++++++++++++++++++++++++++++++*/

    if ( Kad > epsilon ) {
      

      if (fabs(curva_mean[i]) < epsilon ) {
	printf("__FILE__, __LINE__, curva_mean too small!\n");
	curva_mean[i] = epsilon;
      }
      
      doef = Kad*pi/area_tot*(cm_integral-DA0D/2)*area[i]/2.0/curva_mean[i];
      
      vec_scalar(lbisq_der, doef, df);
      
      /*accumulate the force on vertices i and j*/
      vec_append(df, i, /**/ fx, fy, fz);
      vec_substr(df, j, /**/ fx, fy, fz);

    }
    
    /*###################################
      ###################################
      force part II
      ###################################
      ###################################*/
    
    /*calculate derivative of Laplace-Beltrami operator: part II*/
    
    /*this is to calculate derivative of cotangent of angle c 
      with respect to poisition b, c, a*/
    
    dtri_cot(b, c, a, /**/ db1, dc, da1);
    
    /*this is calculate derivative of cotangent of angle d 
      with respect to poisition a, d, b*/
    
    dtri_cot(a, d, b, /**/ da2, dd, db2);
    
    coef1 =  vec_dot(lbi, r) / area1;
    coef2 = -lbisq * rsq / area1 / 4.0;
    coef *= (coef1 + coef2);
    
    /*accumulate the force on vertices i, j, k*/
    vec_scalar_append(da1, coef, i, /**/ fx, fy, fz);
    vec_scalar_append(db1, coef, j, /**/ fx, fy, fz);
    vec_scalar_append(dc,  coef, k, /**/ fx, fy, fz);
    
    /*accumulate the force on vertices i, j, l*/
    vec_scalar_append(da2, coef, i, /**/ fx, fy, fz);
    vec_scalar_append(db2, coef, j, /**/ fx, fy, fz);
    vec_scalar_append(dd,  coef, l, /**/ fx, fy, fz);

    coef3 = coef1 + coef2;
    
    /*+++++++++++++++++++++++++++++++++++
      force due to area-difference elasticity: part II
      +++++++++++++++++++++++++++++++++++*/
    
    if ( Kad > epsilon ) {
      
      doef *= (coef1 + coef2);
      
      /*accumulate the force on vertices i, j, k*/
      vec_scalar_append(da1, doef, i, /**/ fx, fy, fz);
      vec_scalar_append(db1, doef, j, /**/ fx, fy, fz);
      vec_scalar_append(dc,  doef, k, /**/ fx, fy, fz);
      
      /*accumulate the force on vertices i, j, l*/
      vec_scalar_append(da2, doef, i, /**/ fx, fy, fz);
      vec_scalar_append(db2, doef, j, /**/ fx, fy, fz);
      vec_scalar_append(dd,  doef, l, /**/ fx, fy, fz);

    }
    
    /*###################################
      ###################################
      force part III
      ###################################      
      ###################################*/
    
    /*calculate derivative of local area with respective to position b, c, a
      This is similiar to the cotangent case above.*/
    coef  = Kb*2.0*(curva_mean[i]-H0)*(curva_mean[i]-H0);
    
    coef1 = coef * cot1 / 4.0;
    vec_scalar(r, coef1, df);
    
    /*accumulate the force on vertices i and j*/
    vec_append(df, i, /**/ fx, fy, fz);
    vec_substr(df, j, /**/ fx, fy, fz);
    
    coef2 = coef * rsq / 8.0;
    
    /*accumulate the force on vertices i, j, k*/
    vec_scalar_append(da1, coef2, i, /**/ fx, fy, fz);
    vec_scalar_append(db1, coef2, j, /**/ fx, fy, fz);
    vec_scalar_append(dc,  coef2, k, /**/ fx, fy, fz);

    /*accumulate the force on vertices i, j, l*/
    vec_scalar_append(da2, coef2, i, /**/ fx, fy, fz);
    vec_scalar_append(db2, coef2, j, /**/ fx, fy, fz);
    vec_scalar_append(dd,  coef2, l, /**/ fx, fy, fz);
    
    
    /*+++++++++++++++++++++++++++++++++++
      force due to area-difference elasticity: part III
      +++++++++++++++++++++++++++++++++++*/

    if ( Kad > epsilon ) {
      
      doef=4.0*Kad*pi/area_tot*(cm_integral-DA0D/2.0)*curva_mean[i];
      
      doef1 = doef * cot1 / 4.0;
      
      vec_scalar(r, doef1, df);
      
      /*accumulate the force on vertices i and j*/
      vec_append(df, i, /**/ fx, fy, fz);
      vec_substr(df, j, /**/ fx, fy, fz);    
      
      doef2 = doef * rsq / 8.0;
      
      /*accumulate the force on vertices i, j, k*/
      vec_scalar_append(da1, doef2, i, /**/ fx, fy, fz);
      vec_scalar_append(db1, doef2, j, /**/ fx, fy, fz);
      vec_scalar_append(dc,  doef2, k, /**/ fx, fy, fz);
      
      /*accumulate the force on vertices i, j, l*/
      vec_scalar_append(da2, doef2, i, /**/ fx, fy, fz);
      vec_scalar_append(db2, doef2, j, /**/ fx, fy, fz);
      vec_scalar_append(dd,  doef2, l, /**/ fx, fy, fz);

    }
      
  }
   
  return HE_OK;
}
