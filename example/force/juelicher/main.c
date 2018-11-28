#include <stdio.h>
#include <tgmath.h>

#include <real.h>
#include <he/err.h>
#include <he/x.h>
#include <he/vec.h>
#include <he/tri.h>
#include <he/memory.h>
#include <he/punto.h>
#include <he/ddih.h>

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

  printf("#1 azimuth angle; 2 axis dist; 3 zz; 4 |F|; 5 fx; 6 fy; 7 fz; 8 area; \n");
  real *queue[] = {TH, RR, ZZ, fm, fx, fy, fz, A, NULL};

  punto_fwrite(NV, queue, stdout);
  
  FREE(fm);
  
}

void dihedral_derivative(const real a[3], const real b[3], const real c[3], const real d[3],
			 /**/ real da[3], real db[3], real dc[3], real dd[3]){

  real aream, arean;
  real u[3], v[3], w[3], g[3], h[3], f[3];
  real mm[3], nn[3];
  real m[3], n[3];
  real mndot;
  real mnmn[3], nmnm[3];
  real tvec1[3], tvec2[3];
  real coef;

  aream = tri_area(a, b, c);
  arean = tri_area(d, c, b);
  
  vec_minus(c, b, u);
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

  vec_linear_combination(1.0, m, -mndot, n, tvec1);
  vec_norm(tvec1, mnmn);
  
  vec_linear_combination(1.0, n, -mndot, m, tvec2);
  vec_norm(tvec2, nmnm);
  
  vec_cross(g, nmnm, tvec1);
  vec_scalar(tvec1, 1.0/aream/2.0, da);
  
  vec_cross(h, mnmn, tvec1);
  vec_cross(w, nmnm, tvec2);
  vec_linear_combination(1.0/arean/2.0, tvec1, 1.0/aream/2.0, tvec2, db);

  vec_cross(f, mnmn, tvec1);
  vec_cross(v, nmnm, tvec2);
  vec_linear_combination(1.0/arean/2.0, tvec1, 1.0/aream/2.0, tvec2, dc);

  vec_cross(u, mnmn, tvec1);
  vec_scalar(tvec1, 1.0/arean/2.0, dd);
  
}


void edge_length_derivative(const real a[3], const real b[3],
			    /**/ real da[3], real db[3]){

  real u[3], e[3];
  real len0;
  
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
  real da[3], db[3], dc[3], dd[3];
  real u[3];
  //real v[3], w[3], g[3], h[3], f[3];

  real unorm[3];
  real mm[3], nn[3];
  real m[3], n[3];
  real mndot;
  real mnmn[3], nmnm[3];
  real temp_vec[3];
  real theta_der[3];

  real *lentheta, *area;
  real len0, theta0, lentheta0, area0;
  real aream, arean;
  real coef, coef1, coef2;
  real *fx, *fy, *fz;

  
  MALLOC(NV, &lentheta);
  MALLOC(NV, &area);

  MALLOC(NV, &fx);
  MALLOC(NV, &fy);
  MALLOC(NV, &fz);

  C0 = 0;
  H0 = C0/2.0;
  
  for (i = 0; i < NV; i++) {

    lentheta[i] = 0;
    area[i]     = 0;

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
    len0 = vec_abs(u);

    lentheta0    = len0*theta0;
    lentheta[j] += lentheta0;
    lentheta[k] += lentheta0;
    
  }

  //2nd loop;
  for (t = 0; t < NT; t++) {
      
    i = T0[t]; j = T1[t]; k = T2[t];
      
    get3(i, j, k, a, b, c);
    area0 = tri_area(a, b, c);

    area[i] += area0/3;
    area[j] += area0/3;
    area[k] += area0/3;

  }

  
  //3rd loop; force due to edge length and dihedral angle
  for (e = 0; e < NE; e++) {
    
    he = hdg_edg(e);
    
    if ( bnd(he) ) continue;      
    
    i = D0[e]; j = D1[e]; k = D2[e]; l = D3[e];
    
    get4(i, j, k, l, /**/ a, b, c, d);

    theta0 = tri_dih(a, b, c, d);



    /*calculate force from derivative of edge length*/
    
    coef = - ( (lentheta[j]/area[j]/4.0 - H0) + (lentheta[k]/area[k]/4.0 - H0) ) * theta0;

    edge_length_derivative(b,c, db, dc);
    
    vec_scalar_append(db, coef, j, fx, fy, fz);
    vec_scalar_append(dc, coef, k, fx, fy, fz);

    vec_minus(c, b, u);
    len0 = vec_abs(u);


    /*calculate force from derivative of dihedral angle*/
    
    /*dihedral_derivative(a, b, c, d, da, db, dc, dd);
    printf("%f %f %f\n", a[0], a[1], a[2]);
    printf("%f %f %f\n", da[0], da[1], da[2]);*/
    ddih_angle(a, b, c, d, da, db, dc, dd);
    //printf("%f %f %f\n\n", da[0], da[1], da[2]);

    coef =  -(  (lentheta[j]/area[j]/4.0 - H0) + (lentheta[k]/area[k]/4.0 - H0) ) * len0 ;

    vec_scalar_append(da, coef, i, fx, fy, fz);
    vec_scalar_append(db, coef, j, fx, fy, fz);
    vec_scalar_append(dc, coef, k, fx, fy, fz);
    vec_scalar_append(dd, coef, l, fx, fy, fz);    
    
  }
  
  //4th loop; 
  for (t = 0; t < NT; t++) {
    
    i = T0[t]; j = T1[t]; k = T2[t];
      
    get3(i, j, k, a, b, c);


    /* calculate force due to derivative of area*/
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
  
  write(/*i*/ fx, fy, fz, area);

  
  FREE(lentheta);
  FREE(area);
  FREE(fx);
  FREE(fy);
  FREE(fz);
    
}

int main() {
  ini("/dev/stdin");
  force_juelicher();
  fin();
  return HE_OK;
}
