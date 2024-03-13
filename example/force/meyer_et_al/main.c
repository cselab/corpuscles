#include <stdio.h>
#include <math.h>

#include <real.h>
#include <co/err.h>
#include <co/x.h>
#include <co/vec.h>
#include <co/tri.h>
#include <co/memory.h>
#include <co/punto.h>

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

void force_meyer_et_al() {
  /*This routine calculates bending energy
    according to Meyer et al, 2003
    
    It traverses each edge/dihedral.
    It traverses each triangle to calculate area.
    It traverses each vertex.
    It traverses each edge/dihedral.
    
    Version: Xin Bian, 19 September 2018 @CSE Lab, ETH Zurich*/

  enum {X, Y, Z};
  int v, e, t;
  int i, j, k, l;
  real a[3], b[3], c[3], d[3], u[3], n[3];
  real coti, cotl, cotil;
  real *lbx, *lby, *lbz, *curva_mean, *area;
  real *curva_gauss;
  real *nx, *ny, *nz;
  real fm;
  real *fx, *fy, *fz;
  real area0;
  int  obtuse;
  real thetaa, thetab, thetac;
  real cota,cotb,cotc;
  real ab2, bc2, ca2;
  real area_tot_tri, area_tot_mix;

  real C0, H0;
  
  MALLOC(NV, &lbx);
  MALLOC(NV, &lby);
  MALLOC(NV, &lbz);
  MALLOC(NV, &curva_mean);
  MALLOC(NV, &curva_gauss);
  MALLOC(NV, &nx);
  MALLOC(NV, &ny);
  MALLOC(NV, &nz); 
  MALLOC(NV, &area);
  MALLOC(NV, &fx);
  MALLOC(NV, &fy);
  MALLOC(NV, &fz); 

  C0 = 0;
  H0 = C0/2.0;
  
  for (v = 0; v < NV; v++) {
    
    lbx[v] = 0;
    lby[v] = 0;
    lbz[v] = 0;
    curva_mean[v] = 0;
    curva_gauss[v] = 0;
    nx[v] = 0;
    ny[v] = 0;
    nz[v] = 0;    
    area[v] = 0;
    fx[v] = 0;
    fy[v] = 0;
    fz[v] = 0;
    
  }
  
  for (e = 0; e < NE; e++) {
    
    if (bnd(e)) continue;
    
    i = D0[e]; j = D1[e]; k = D2[e]; l = D3[e];
    
    get4(i, j, k, l, /**/ a, b, c, d);
    
    coti = tri_cot(c, a, b);
    cotl = tri_cot(b, d, c);
    cotil = coti + cotl;
    
    vec_minus(b, c, u);
    
    lbx[j] += cotil*u[X]/2;
    lby[j] += cotil*u[Y]/2;
    lbz[j] += cotil*u[Z]/2;
    
    lbx[k] -= cotil*u[X]/2;
    lby[k] -= cotil*u[Y]/2;
    lbz[k] -= cotil*u[Z]/2;

  }

  area_tot_tri = 0;
  
  for ( t = 0; t < NT; t++ ) {
    
    /*At first, assume it is not an obtuse triangle*/
    obtuse = 0;
    
    i = T0[t]; j = T1[t]; k = T2[t];
    
    get3(i, j, k, a, b, c);
    area0 = tri_area(a, b, c);

    area_tot_tri += area0;
    
    /*check if angle a is obtuse*/
    thetaa = tri_angle(c, a, b);
    thetab = tri_angle(a, b, c);
    thetac = tri_angle(b, c, a);
      
    if ( thetaa > pi/2.0 ) {
      
      area[i] += area0/2;
      area[j] += area0/4;
      area[k] += area0/4;
      
      obtuse = 1;
    }
    else {
      /*check if angle b is obtuse*/
      
      if ( thetab > pi/2.0 ) {
	
	area[j] += area0/2;
	area[i] += area0/4;
	area[k] += area0/4;
	
	obtuse = 1;
      }
      else {
	/*check if angle c is obtuse*/
	
	if ( thetac > pi/2.0 ) {
	  
	  area[k] += area0/2;
	  area[i] += area0/4;
	  area[j] += area0/4;
	  
	  obtuse = 1;
	}
	
      }
      
    }
    
    /*if it is not obtuse triangle, 
      we calculte its area split due to voronoi tesselation*/
    if ( obtuse == 0 ) {

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
    
    /* calculate Gaussian curvature */
    curva_gauss[i] -= thetaa;
    curva_gauss[j] -= thetab;
    curva_gauss[k] -= thetac;
    
    tri_normal(a, b, c, n);
    
    nx[i] += thetaa * n[X];
    ny[i] += thetaa * n[Y];
    nz[i] += thetaa * n[Z];
    
    nx[j] += thetab * n[X];
    ny[j] += thetab * n[Y];
    nz[j] += thetab * n[Z];
    
    nx[k] += thetac * n[X];
    ny[k] += thetac * n[Y];
    nz[k] += thetac * n[Z];

  }
  
  area_tot_mix = 0;
  
  for ( v = 0; v < NV; v++ ) {
    
    lbx[v] /= area[v];
    lby[v] /= area[v];
    lbz[v] /= area[v];

    curva_gauss[v] = (curva_gauss[v]+2.0*pi)/area[v];

    n[X] = nx[v];
    n[Y] = ny[v];
    n[Z] = nz[v];

    vec_norm(n, u);

    nx[v] = u[X];
    ny[v] = u[Y];
    nz[v] = u[Z];

    n[X] = nx[v];
    n[Y] = ny[v];
    n[Z] = nz[v];

    u[X] = lbx[v];
    u[Y] = lby[v];
    u[Z] = lbz[v];
    
    curva_mean[v] = vec_dot(u, n)/2;

    fm = 2*2*(curva_mean[v]-H0)*(curva_mean[v]*curva_mean[v]-curva_gauss[v]+curva_mean[v]*H0);
    
    fx[v] += fm * nx[v];
    fy[v] += fm * ny[v];
    fz[v] += fm * nz[v];

    /*test*/
    /*printf("lbx, lby, lbz = %f, %f, %f\n", lbx[v], lby[v], lbz[v]);
    printf("nx, ny, nz    = %f, %f, %f\n", nx[v], ny[v], nz[v]);
    printf("cm, cg, fm    = %f, %f, %f\n", curva_mean[v], curva_gauss[v], fm);
    printf("fx, fy, fz    = %f, %f, %f\n\n", fx[v], fy[v], fz[v]);*/
    
  }

  for (e = 0; e < NE; e++) {
    
    if (bnd(e)) continue;
    
    i = D0[e]; j = D1[e]; k = D2[e]; l = D3[e];
    
    get4(i, j, k, l, /**/ a, b, c, d);
    
    coti = tri_cot(c, a, b);
    cotl = tri_cot(b, d, c);
    cotil = coti + cotl;
    
    vec_minus(b, c,  u);

    fm = cotil * (curva_mean[j] - curva_mean[k]) / area[j];
    fx[j] += fm * nx[j];
    fy[j] += fm * ny[j];
    fz[j] += fm * nz[j];

    fm = cotil * (curva_mean[k] - curva_mean[j]) / area[k];
    fx[k] += fm * nx[k];
    fy[k] += fm * ny[k];
    fz[k] += fm * nz[k];
    
  }
  
  write(/*i*/ fx, fy, fz, area);

  FREE(lbx);
  FREE(lby);
  FREE(lbz);
  FREE(curva_mean);
  FREE(curva_gauss);
  FREE(nx);
  FREE(ny);
  FREE(nz);
  FREE(area); 
  FREE(fx);
  FREE(fy);
  FREE(fz);
  
}


int main() {
  ini("/dev/stdin");
  force_meyer_et_al();
  fin();
  return CO_OK;
}
