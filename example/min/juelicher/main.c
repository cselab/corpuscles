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

static void get3(int i, int j, int k, /**/ real a[3], real b[3], real c[3]) {

  vec_get(i, XX, YY, ZZ, a);
  vec_get(j, XX, YY, ZZ, b);
  vec_get(k, XX, YY, ZZ, c);

}

static void get4(int i, int j, int k, int l, /**/
          real a[3], real b[3], real c[3], real d[3]) {
  
  vec_get(i, XX, YY, ZZ, a);
  vec_get(j, XX, YY, ZZ, b);
  vec_get(k, XX, YY, ZZ, c);
  vec_get(l, XX, YY, ZZ, d);
  
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

static void force(const real *XX, const real *YY, const real *ZZ,
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
    
    get4(i, j, k, l, /**/ a, b, c, d);
    
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
      
    get3(i, j, k, a, b, c);
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
    
    get4(i, j, k, l, /**/ a, b, c, d);

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
      
    get3(i, j, k, a, b, c);

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

int main() {
  ini("/dev/stdin");
  force_ini();
  force(XX, YY, ZZ, fx, fy, fz);
  write(/*i*/ fx, fy, fz, AREA);
  
  force_ini();
  fin();
  return HE_OK;
}
