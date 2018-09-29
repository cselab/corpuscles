#include <stdio.h>
#include <math.h>

#include <real.h>
#include <he/err.h>
#include <he/x.h>
#include <he/vec.h>
#include <he/tri.h>
#include <he/memory.h>
#include <he/punto.h>
#include <he/ddih.h>
#include <he/dedg.h>

void get3(int i, int j, int k, /**/ real a[3], real b[3], real c[3]) {
  vec_get(i, XX, YY, ZZ, a);
  vec_get(j, XX, YY, ZZ, b);
  vec_get(k, XX, YY, ZZ, c);
}

void get4(int i, int j, int k, int l, /**/
          real a[3], real b[3], real c[3], real d[3]) {
  vec_get(i, XX, YY, ZZ, a);
  vec_get(j, XX, YY, ZZ, b);
  vec_get(k, XX, YY, ZZ, c);
  vec_get(l, XX, YY, ZZ, d);
}

static void write(real *fx, real *fy, real *fz) {
  real *queue[] = {XX, YY, ZZ, fx, fy, fz, NULL};
  punto_fwrite(NV, queue, stdout);
}

void edge_length_derivative(const real a[3], const real b[3],
                            /**/ real da[3], real db[3]){

  real u[3], e[3];

  vec_minus(a, b, u);
  vec_norm(u, e);

  vec_copy(e, da);
  vec_negative(e, db);

}
void area_derivative(const real a[3], const real b[3], const real c[3],
                     /**/ real da[3], real db[3], real dc[3]){


  real area0, coef;
  real u[3], v[3], w[3], n[3], f[3];

  area0 = tri_area(a, b, c);

  vec_minus(b, a, u);
  vec_minus(c, a, v);
  vec_minus(b, c, w);

  vec_cross(u, v, n);

  coef = 1.0/area0/4.0;

  vec_cross(w, n, f);
  vec_scalar(f, coef, da);

  vec_cross(v, n, f);
  vec_scalar(f, coef, db);

  vec_cross(n, u, f);
  vec_scalar(f, coef, dc);

}

void force_juelicher() {
  enum {X, Y, Z};

  real kb, C0, H0, kad;
  real area_tot, curva_mean_area_tot;
  int e, he, t;
  int i, j, k, l;
  real a[3], b[3], c[3], d[3];
  real da[3], db[3], dc[3], dd[3];
  real u[3];
  real *lentheta, *area;
  real len0, theta0, lentheta0, area0;
  real coef, coef1, coef2;
  real *fx, *fy, *fz;
  real *fxad, *fyad, *fzad;


  MALLOC(NV, &lentheta);
  MALLOC(NV, &area);

  MALLOC(NV, &fx);
  MALLOC(NV, &fy);
  MALLOC(NV, &fz);

  MALLOC(NV, &fxad);
  MALLOC(NV, &fyad);
  MALLOC(NV, &fzad);

  kb = 1.0;
  C0 = -1.0;
  H0 = C0/2.0;
  kad= 2.0 * kb / pi;

  for (i = 0; i < NV; i++) {

    lentheta[i] = 0;
    area[i]     = 0;

    fx[i] = 0;
    fy[i] = 0;
    fz[i] = 0;

    fxad[i] = 0;
    fyad[i] = 0;
    fzad[i] = 0;

  }
  curva_mean_area_tot = 0;
  for (e = 0; e < NE; e++) {

    he = hdg_edg(e);

    if ( bnd(he) ) continue;

    i = D0[e]; j = D1[e]; k = D2[e]; l = D3[e];

    get4(i, j, k, l, /**/ a, b, c, d);

    theta0 = tri_dih(a, b, c, d);
    vec_minus(c, b, u);
    len0 = vec_abs(u);

    lentheta0    = len0*theta0;
    lentheta[j] += lentheta0;
    lentheta[k] += lentheta0;

    curva_mean_area_tot += lentheta0/2.0;

  }

  //2nd loop;
  area_tot = 0;
  for (t = 0; t < NT; t++) {

    i = T0[t]; j = T1[t]; k = T2[t];

    get3(i, j, k, a, b, c);
    area0 = tri_area(a, b, c);

    area[i] += area0/3;
    area[j] += area0/3;
    area[k] += area0/3;

    area_tot+= area0;

  }

  curva_mean_area_tot -= H0 *area_tot;
  curva_mean_area_tot = curva_mean_area_tot * (4 * kad * pi / area_tot);

  for (e = 0; e < NE; e++) {

    he = hdg_edg(e);

    if ( bnd(he) ) continue;

    i = D0[e]; j = D1[e]; k = D2[e]; l = D3[e];

    get4(i, j, k, l, /**/ a, b, c, d);

    theta0 = tri_dih(a, b, c, d);

    coef = - ( (lentheta[j]/area[j]/4.0 - H0) + (lentheta[k]/area[k]/4.0 - H0) ) * theta0;

    dedg_abs(b,c, db, dc);

    vec_scalar_append(db, coef, j, fx, fy, fz);
    vec_scalar_append(dc, coef, k, fx, fy, fz);

    coef = -curva_mean_area_tot/4.0 * theta0;

    vec_scalar_append(db, coef, j, fxad, fyad, fzad);
    vec_scalar_append(dc, coef, k, fxad, fyad, fzad);
    ddih_angle(a, b, c, d, da, db, dc, dd);

    vec_minus(c, b, u);
    len0 = vec_abs(u);

    coef =  -(  (lentheta[j]/area[j]/4.0 - H0) + (lentheta[k]/area[k]/4.0 - H0) ) * len0 ;

    vec_scalar_append(da, coef, i, fx, fy, fz);
    vec_scalar_append(db, coef, j, fx, fy, fz);
    vec_scalar_append(dc, coef, k, fx, fy, fz);
    vec_scalar_append(dd, coef, l, fx, fy, fz);

    coef = -curva_mean_area_tot/4.0 *len0;
    vec_scalar_append(da, coef, i, fxad, fyad, fzad);
    vec_scalar_append(db, coef, j, fxad, fyad, fzad);
    vec_scalar_append(dc, coef, k, fxad, fyad, fzad);
    vec_scalar_append(dd, coef, l, fxad, fyad, fzad);

  }

  for (t = 0; t < NT; t++) {
    i = T0[t]; j = T1[t]; k = T2[t];

    get3(i, j, k, a, b, c);
    area_derivative(a, b, c, da, db, dc);

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

  write(fx, fy, fz);

  FREE(lentheta);
  FREE(area);
  FREE(fx);
  FREE(fy);
  FREE(fz);
  FREE(fxad);
  FREE(fyad);
  FREE(fzad);

}

int main() {
  ini("/dev/stdin");
  force_juelicher();
  fin();
  return HE_OK;
}
