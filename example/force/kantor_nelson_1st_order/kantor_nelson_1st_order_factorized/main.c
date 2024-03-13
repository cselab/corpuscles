#include <stdio.h>
#include <math.h>

#include <real.h>
#include <co/err.h>
#include <co/x.h>
#include <co/vec.h>
#include <co/tri.h>
#include <co/memory.h>
#include <co/punto.h>
#include <co/ddih.h>


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
  const real *queue[] = {TH, RR, ZZ, fm, fx, fy, fz, A, NULL};
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

void force_kantor_nelson() {
  /*This routine calculates bending force
    according to Kantor and Nelson, Phys. Rev. A 1987
    
    It traverses each edge/dihedral to calculate force.

    Version: Xin Bian, 14 June 2018 @CSE Lab, ETH Zurich*/
  
  enum {X, Y, Z};
  int e, hedge, t;
  int i, j, k, l;
  real a[3], b[3], c[3], d[3];
  real da[3], db[3], dc[3], dd[3];   
  real *area;
  real *fx, *fy, *fz;
  real coef, coef1, coef2;
  real theta0, area0, aream, arean;
  real theta;

  
  //coef1   = 2.0*sqrt(3.0);
  coef1   = 2.0;
  
  theta0 = 0;
  
  MALLOC(NV, &area);
  MALLOC(NV, &fx);
  MALLOC(NV, &fy);
  MALLOC(NV, &fz);

  for (i = 0; i < NV; i++) {
    
    fx[i] = 0;
    fy[i] = 0;
    fz[i] = 0;
    area[i] = 0;
    
  }
  
  for (e = 0; e < NE; e++) {
    
    hedge = hdg_edg(e);

    if ( bnd(hedge) ) continue;
    
    i = D0[e]; j = D1[e]; k = D2[e]; l = D3[e];


    get4(i, j, k, l, /**/ a, b, c, d);

    /*If the angle theta between the two normal vectors
    is greater than pi/2, it returns a negative value theta
    instead of (pi-theta)*/
    theta = tri_dih(a, b, c, d);

    //printf("%f\n", theta);

    coef2 = -(theta - theta0);

    /*dihedral_derivative(a,b,c,d, da, db, dc, dd);
    printf("%f %f %f\n", a[0], a[1], a[2]);
    printf("%f %f %f\n", da[0], da[1], da[2]);*/
    ddih_angle(a,b,c,d, da, db, dc, dd);
    //printf("%f %f %f\n\n", da[0], da[1], da[2]);
    
    coef = coef1 * coef2 ;

    vec_scalar_append(da, coef, i, fx, fy, fz);
    vec_scalar_append(db, coef, j, fx, fy, fz);
    vec_scalar_append(dc, coef, k, fx, fy, fz);
    vec_scalar_append(dd, coef, l, fx, fy, fz);

  }

  /*calculate area attached to each vertex,
    we use convention of Juelicher 1996 so that
    the area of each triangle is equally splitted 
    into three vertices.*/
  
  for (t = 0; t < NT; t++) {
    
    i = T0[t]; j = T1[t]; k = T2[t];
      
    get3(i, j, k, a, b, c);
    area0 = tri_area(a, b, c);
    
    area[i] += area0/3;
    area[j] += area0/3;
    area[k] += area0/3;

  }

  write(/*i*/ fx, fy, fz, area);

  
  FREE(area);
  FREE(fx); FREE(fy); FREE(fz);
}

int main() {
  ini("/dev/stdin");
  force_kantor_nelson();
  fin();
  return CO_OK;
}
