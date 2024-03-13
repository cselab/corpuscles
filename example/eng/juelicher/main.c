#include <stdio.h>
#include <math.h>

#include <real.h>
#include <co/err.h>
/* #include <co/x.h> */
#include <co/vec.h>
#include <co/tri.h>
#include <co/memory.h>

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

void energy_juelicher() {
    /*This routine calculates bending energy
      according to Juelicher, J. Phys. II France, 1996
    
    It traverses each edge/dihedral to calculate energy.
    It traverses each vertex to calculate area.
    
    Version: Xin Bian, 05 June 2018 @CSE Lab, ETH Zurich*/
  
  enum {X, Y, Z};
  int v, e, h, t;
  int i, j, k, l;
  real a[3], b[3], c[3], d[3], u[3], coord[3];
  real *curva_mean, *energy, *area;
  real cur, len, len2, area0;
  real theta, rxy, phi;
  real area_tot_tri, area_tot_split;
  real energy_tot;

  real C0, H0;
  
  MALLOC(NV, &curva_mean);
  MALLOC(NV, &energy);
  MALLOC(NV, &area);


  C0=0;
  H0=C0/2.0;
  
  for (v = 0; v < NV; v++) {
    curva_mean[v] = 0;
    energy[v]     = 0;
    area[v]       = 0;
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
    
    cur = len*theta/4;
    curva_mean[j] += cur;
    curva_mean[k] += cur;
    
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
  
  printf("#1 azimuth angle; 2 axis dist; 3 enegy; 4 energy density; 5 curvature mean; 6 area\n");

  for (v = 0; v < NV; v++) {
      
    curva_mean[v] /= area[v];
    energy[v] = 2 * (curva_mean[v]-H0)*(curva_mean[v]-H0)*area[v];

    /*for verification*/
    area_tot_split += area[v];
    energy_tot     += energy[v];
    
    vec_get(v, XX, YY, ZZ, coord);
    rxy = vec_cylindrical_r(coord);
    
    phi = TH[v];
    if ( phi > pi / 2) {
      phi = pi - phi;
    }
    
    printf("%g %g %g %g %g %g\n", phi, rxy, energy[v], energy[v]/area[v], curva_mean[v], area[v]);
    
  }

  printf("###NT, area_tot_tri, area_tot_split, energy_tot\n");
  printf("##%i %g %g %g\n", NT, area_tot_tri, area_tot_split, energy_tot);

  FREE(curva_mean);
  FREE(energy);
  FREE(area);
  
}

int main() {
  ini("/dev/stdin");
  energy_juelicher();
  fin();
  return CO_OK;
}
