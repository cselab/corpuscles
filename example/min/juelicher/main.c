#include <stdio.h>
#include <math.h>

#include <real.h>
#include <he/err.h>
#include <he/x.h>
#include <he/vec.h>
#include <he/tri.h>
#include <he/memory.h>
#include <he/punto.h>

static real *fx, *fy, *fz;
static real *lentheta, *AREA;
static real *curva_mean, *ENERGY;

static void get3(const real *xx, const real *yy, const real *zz,
                 int i, int j, int k,
                 /**/ real a[3], real b[3], real c[3]) {

  vec_get(i, xx, yy, zz, a);
  vec_get(j, xx, yy, zz, b);
  vec_get(k, xx, yy, zz, c);
}

static void get4(const real *xx, const real *yy, const real *zz,
                 int i, int j, int k, int l, /**/
                 real a[3], real b[3], real c[3], real d[3]) {
  vec_get(i, xx, yy, zz, a);
  vec_get(j, xx, yy, zz, b);
  vec_get(k, xx, yy, zz, c);
  vec_get(l, xx, yy, zz, d);
}

static void vabs(int n, real *x, real *y, real *z, /**/ real *r) {
  /*Given n vectors with x, y, z components,
    calculate the absolute value/Euclidean length for each vector.*/

    int i;

    for (i = 0; i < n; i++)
        r[i] = sqrt(x[i]*x[i] + y[i]*y[i] + z[i]*z[i]);

}

static void write(real *fx, real *fy, real *fz,
                  real *A) {

  real *fm;;
  RZERO(NV, &fm);
  vabs(NV, fx, fy, fz, /**/ fm);

  real *queue[] = {TH, RR, ZZ, fm, A, NULL};
  punto_fwrite(NV, queue, stdout);

  FREE(fm);

}

static real energy() {
    /*This routine calculates bending energy
      according to Juelicher, J. Phys. II France, 1996

    It traverses each edge/dihedral to calculate energy.
    It traverses each vertex to calculate area.

    Version: Xin Bian, 05 June 2018 @CSE Lab, ETH Zurich*/

  enum {X, Y, Z};
  int v, e, h, t;
  int i, j, k, l;
  real a[3], b[3], c[3], d[3], u[3], coord[3];
  real cur, len, len2, area0;
  real theta, rxy, phi;
  real area_tot_tri, area_tot_split;
  real energy_tot;
  real C0, H0;

  C0=0;
  H0=C0/2.0;
  for (v = 0; v < NV; v++) {
    curva_mean[v] = 0;
    ENERGY[v]     = 0;
    AREA[v]       = 0;
  }

  for (e = 0; e < NE; e++) {

    h = hdg_edg(e);

    if ( bnd(h) ) continue;

    i = D0[e]; j = D1[e]; k = D2[e]; l = D3[e];

    get4(XX, YY, ZZ, i, j, k, l, /**/ a, b, c, d);

    theta = tri_dih(a, b, c, d);

    vec_minus(b, c, u);
    len2 = vec_dot(u, u);
    len  = sqrt(len2);

    cur = len*theta/4;
    curva_mean[j] += cur;
    curva_mean[k] += cur;

  }

  area_tot_tri = 0;

  for (t = 0; t < NT; t++) {

    i = T0[t]; j = T1[t]; k = T2[t];

    get3(XX, YY, ZZ, i, j, k, a, b, c);
    area0 = tri_area(a, b, c);

    AREA[i] += area0/3;
    AREA[j] += area0/3;
    AREA[k] += area0/3;

    area_tot_tri += area0;

  }

  area_tot_split = 0;
  energy_tot     = 0;

  for (v = 0; v < NV; v++) {

    curva_mean[v] /= AREA[v];
    ENERGY[v] = 2 * (curva_mean[v]-H0)*(curva_mean[v]-H0)*AREA[v];
    /*for verification*/
    area_tot_split += AREA[v];
    energy_tot     += ENERGY[v];

    vec_get(v, XX, YY, ZZ, coord);
    rxy = vec_cylindrical_r(coord);

    phi = TH[v];
    if ( phi > pi / 2) {
      phi = pi - phi;
    }
  }
  return energy_tot;
}

static void force(const real *xx, const real *yy, const real *zz,
                  /**/ real *fx, real *fy, real *fz) {
    /*This routine calculates bending force
      according to Juelicher, J. Phys. II France, 1996

    1st loop;
    it traverses each edge, which has a dihedral angle,
    to calculate bond length, dihedral angle and their product.
    2nd loop;
    it traverses each triangle to calculate its area and split it
    1/3 into each of its vertex.
    3rd lopp;
    it traverses each edge again, to calculate derivatives.
    4th lopp;
    it traverses each triangle again, to calculate derivatives.

    Version: Xin Bian, 14 September 2018 @CSE Lab, ETH Zurich*/

  enum {X, Y, Z};

  real C0, H0;
  int e, he, t;
  int i, j, k, l;
  real a[3], b[3], c[3], d[3];
  real u[3], v[3], w[3], g[3], h[3], f[3];

  real unorm[3];
  real mm[3], nn[3];
  real m[3], n[3];
  real mndot;
  real mnmn[3], nmnm[3];
  real temp_vec[3];
  real theta_der[3];

  real len0, theta0, lentheta0, area0;
  real aream, arean;
  real coef, coef1;
  C0 = 0;
  H0 = C0/2.0;

  for (i = 0; i < NV; i++) {

    lentheta[i] = 0;
    AREA[i]     = 0;

    fx[i]  = 0;
    fy[i]  = 0;
    fz[i]  = 0;

  }


  //1st loop;
  for (e = 0; e < NE; e++) {

    he = hdg_edg(e);

    if ( bnd(he) ) continue;

    i = D0[e]; j = D1[e]; k = D2[e]; l = D3[e];

    get4(xx, yy, zz, i, j, k, l, /**/ a, b, c, d);

    theta0 = tri_dih(a, b, c, d);

    vec_minus(c, b, u);
    len0 = vec_dot(u, u);
    len0 = sqrt(len0);

    lentheta0    = len0*theta0;
    lentheta[j] += lentheta0;
    lentheta[k] += lentheta0;

  }

  //2nd loop;
  for (t = 0; t < NT; t++) {

    i = T0[t]; j = T1[t]; k = T2[t];

    get3(xx, yy, zz, i, j, k, a, b, c);
    area0 = tri_area(a, b, c);

    AREA[i] += area0/3;
    AREA[j] += area0/3;
    AREA[k] += area0/3;

  }


  //3rd loop;
  for (e = 0; e < NE; e++) {

    he = hdg_edg(e);

    if ( bnd(he) ) continue;

    i = D0[e]; j = D1[e]; k = D2[e]; l = D3[e];

    get4(xx, yy, zz, i, j, k, l, /**/ a, b, c, d);

    theta0 = tri_dih(a, b, c, d);

    vec_minus(c, b, u);

    vec_norm(u, unorm);

    coef = -(lentheta[j]/AREA[j]/4.0 - H0) * theta0;
    vec_scalar_append(unorm, -coef, j, fx, fy, fz);
    vec_scalar_append(unorm, coef, k, fx, fy, fz);

    coef = -(lentheta[k]/AREA[k]/4.0 - H0) * theta0;
    vec_scalar_append(unorm, -coef, j, fx, fy, fz);
    vec_scalar_append(unorm, coef, k, fx, fy, fz);

    len0 = vec_dot(u, u);
    len0 = sqrt(len0);

    aream = tri_area(a, b, c);
    arean = tri_area(d, c, b);

    vec_minus(a, b, v);
    vec_minus(c, a, w);

    vec_minus(b, d, f);
    vec_minus(b, c, g);
    vec_minus(d, c, h);

    vec_cross(u, v, mm);
    vec_cross(g, h, nn);

    vec_norm(mm, m);
    vec_norm(nn, n);

    mndot = vec_dot(m,n);

    vec_linear_combination(1.0, m, -mndot, n, mnmn);
    vec_norm(mnmn, temp_vec);
    vec_negative(temp_vec, mnmn);

    vec_linear_combination(1.0, n, -mndot, m, nmnm);
    vec_norm(nmnm, temp_vec);
    vec_negative(temp_vec, nmnm);

    coef =  -(lentheta[j]/AREA[j]/4.0 - H0) * len0 ;

    vec_cross(g, nmnm, theta_der);
    coef1 = coef / aream / 2.0;
    vec_scalar_append(theta_der, coef1, i, fx, fy, fz);

    vec_cross(h, mnmn, theta_der);
    coef1 = coef / arean / 2.0;
    vec_scalar_append(theta_der, coef1, j, fx, fy, fz);

    vec_cross(w, nmnm, theta_der);
    coef1 = coef / aream / 2.0;
    vec_scalar_append(theta_der, coef1, j, fx, fy, fz);

    vec_cross(f, mnmn, theta_der);
    coef1 = coef / arean / 2.0;
    vec_scalar_append(theta_der, coef1, k, fx, fy, fz);

    vec_cross(v, nmnm, theta_der);
    coef1 = coef / aream / 2.0;
    vec_scalar_append(theta_der, coef1, k, fx, fy, fz);

    vec_cross(u, mnmn, theta_der);
    coef1 = coef / arean / 2.0;
    vec_scalar_append(theta_der, coef1, l, fx, fy, fz);

    coef =  -(lentheta[k]/AREA[k]/4.0 - H0) * len0 ;

    vec_cross(g, nmnm, theta_der);
    coef1 = coef / aream / 2.0;
    vec_scalar_append(theta_der, coef1, i, fx, fy, fz);

    vec_cross(h, mnmn, theta_der);
    coef1 = coef / arean / 2.0;
    vec_scalar_append(theta_der, coef1, j, fx, fy, fz);

    vec_cross(w, nmnm, theta_der);
    coef1 = coef / aream / 2.0;
    vec_scalar_append(theta_der, coef1, j, fx, fy, fz);

    vec_cross(f, mnmn, theta_der);
    coef1 = coef / arean / 2.0;
    vec_scalar_append(theta_der, coef1, k, fx, fy, fz);

    vec_cross(v, nmnm, theta_der);
    coef1 = coef / aream / 2.0;
    vec_scalar_append(theta_der, coef1, k, fx, fy, fz);

    vec_cross(u, mnmn, theta_der);
    coef1 = coef / arean / 2.0;
    vec_scalar_append(theta_der, coef1, l, fx, fy, fz);

  }

  //4th loop
  for (t = 0; t < NT; t++) {

    i = T0[t]; j = T1[t]; k = T2[t];

    get3(xx, yy, zz, i, j, k, a, b, c);

    area0 = tri_area(a, b, c);

    vec_minus(b, a, u);
    vec_minus(c, a, v);
    vec_minus(b, c, w);

    vec_cross(u, v, n);

    coef = 1.0/area0/4.0/3.0;

    coef1 = (lentheta[i]*lentheta[i]/8.0/AREA[i]/AREA[i] - 2.0*H0*H0) * coef;
    vec_cross(w, n, f);
    vec_scalar_append(f, coef1, i, fx, fy, fz);

    coef1 = (lentheta[j]*lentheta[j]/8.0/AREA[j]/AREA[j] - 2.0*H0*H0) * coef;
    vec_cross(v, n, f);
    vec_scalar_append(f, coef1, j, fx, fy, fz);

    coef1 = (lentheta[k]*lentheta[k]/8.0/AREA[k]/AREA[k] - 2.0*H0*H0) * coef;
    vec_cross(n, u, f);
    vec_scalar_append(f, coef1, k, fx, fy, fz);

  }
}

static void force_ini() {
  MALLOC(NV, &lentheta);
  MALLOC(NV, &AREA);
  MALLOC(NV, &fx);
  MALLOC(NV, &fy);
  MALLOC(NV, &fz);
}

static void force_fin() {
  FREE(lentheta);
  FREE(AREA);
  FREE(fx);
  FREE(fy);
  FREE(fz);
}

static void energy_ini() {
    MALLOC(NV, &curva_mean);
    MALLOC(NV, &ENERGY);
}

static void energy_fin() {
    FREE(curva_mean);
    FREE(ENERGY);
}

int main() {
  ini("/dev/stdin");
  force_ini();
  energy_ini();

  force(XX, YY, ZZ, fx, fy, fz);
  write(/*i*/ fx, fy, fz, AREA);
  printf("%g\n", energy());

  energy_fin();
  force_fin();
  fin();
  return HE_OK;
}
