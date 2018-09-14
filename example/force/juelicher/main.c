#include <stdio.h>
#include <math.h>

#include <real.h>
#include <he/err.h>
#include <he/x.h>
#include <he/vec.h>
#include <he/tri.h>
#include <he/memory.h>

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

static void write(real *fx, real *fy, real *fz,
                  real *A) {

  real *fm;;
  RZERO(NV, &fm);
  vabs(NV, fx, fy, fz, /**/ fm);
  
  printf("#1 azimuth angle; 2 axis dist; 3 zz; 4 |F|; 5 area; \n");
  real *queue[] = {TH, RR, ZZ, fm, A, NULL};
  punto_fwrite(NV, queue, stdout);
  
  FREE(fm);
  
}
void force_juelicher() {
    /*This routine calculates bending force
      according to Juelicher, J. Phys. II France, 1996
    
    It traverses each edge/dihedral to calculate bond length and dihedral angle.
    It traverses each vertex to calculate area and its derivatives.
    
    Version: Xin Bian, 14 September 2018 @CSE Lab, ETH Zurich*/
  
  enum {X, Y, Z};
  int v, e, h, t;
  int i, j, k, l;
  real a[3], b[3], c[3], d[3], u[3], coord[3];
  real *curva_mean, *energy, *area;

  real *lentheta; 
  real *fx, *fy, *fz;
  real temp;

  real xjk[3];
  
  real cur, len, len2, area0;
  real theta, rxy, phi;
  real area_tot_tri, area_tot_split;
  real energy_tot;
  
  MALLOC(NV, &curva_mean);
  MALLOC(NV, &energy);
  MALLOC(NV, &area);

  MALLOC(NV, lentheta);
  MALLOC(NV, &fx);
  MALLOC(NV, &fy);
  MALLOC(NV, &fz);

  
  for (v = 0; v < NV; v++) {
    curva_mean[v] = 0;
    energy[v]     = 0;
    area[v]       = 0;

    ltheta = 0;
    fx[v]  = 0;
    fy[v]  = 0;
    fz[v]  = 0;
  }
  
  for (e = 0; e < NE; e++) {
    
    h = hdg_edg(e);
    
    if ( bnd(h) ) continue;      
    
    i = D0[e]; j = D1[e]; k = D2[e]; l = D3[e];
    
    get4(i, j, k, l, /**/ a, b, c, d);
    
    theta = tri_dih(a, b, c, d);
    
    vec_minus(b, c, u);
    len2 = vec_dot(u, u);
    len  = sqrt(len2);
    
    temp = len*theta;
    lentheta[j] += temp;
    lentheta[k] += temp;
    
  }

  area_tot_tri = 0;

  for (t = 0; t < NT; t++) {
      
    i = T0[t]; j = T1[t]; k = T2[t];
      
    get3(i, j, k, a, b, c);
    area0 = tri_area(a, b, c);

    area[i] += area0/3;
    area[j] += area0/3;
    area[k] += area0/3;

    area_tot_tri += area0;
    
  }

  area_tot_split = 0;
  energy_tot     = 0;
  

  for (e = 0; e < NE; e++) {
    
    h = hdg_edg(e);
    
    if ( bnd(h) ) continue;      
    
    i = D0[e]; j = D1[e]; k = D2[e]; l = D3[e];
    
    get4(i, j, k, l, /**/ a, b, c, d);

    theta = tri_dih(a, b, c, d);

    vec_minus(b, c, xjk);
    
    coef = theta * lentheta[j]/A[j]/4.0;
    vec_scalar_append(xjk, coef, j, fx, fy, fz);
    
    coef = -theta * lentheta[k]/A[k]/4.0;
    
    vec_scalar_append(xjk, coef, k, fx, fy, fz);
    
  }


  for (t = 0; t < NT; t++) {
      
    i = T0[t]; j = T1[t]; k = T2[t];
      
    get3(i, j, k, a, b, c);


  }
  
  for (v = 0; v < NV; v++) {
      
    curva_mean[v] /= area[v];
    energy[v] = 2 * curva_mean[v]*curva_mean[v]*area[v];

    /*for verification*/
    area_tot_split += area[v];
    energy_tot     += energy[v];
    
    vec_get(v, XX, YY, ZZ, coord);
    rxy = vec_cylindrical_r(coord);
    
    phi = TH[v];
    if ( phi > pi / 2) {
      phi = pi - phi;
    }
    
    
  }


  write(/*i*/ fx, fy, fz, area);

  FREE(curva_mean);
  FREE(energy);
  FREE(area);
  
  FREE(lentheta);
  FREE(fx); FREE(fy); FREE(fz);
    
}

int main() {
  ini("/dev/stdin");
  force_juelicher();
  fin();
  return HE_OK;
}
