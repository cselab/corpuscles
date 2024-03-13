#include <stdio.h>
#include <math.h>

#include <real.h>
#include <co/err.h>
#include <co/x.h>
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

void energy_kantor_nelson() {
  /*This routine calculates bending energy
    according to Kantor and Nelson, Phys. Rev. A 1987
    
    It traverses each edge, which has a dihedral angle, to calculate energy.
    Splitting energy of each dihedral to vertices has ambiguity and
    we simply split equaly 1/4 to each vertex involved.

    Version: Xin Bian, 17 September 2018 @CSE Lab, ETH Zurich*/

  enum {X, Y, Z};
  int v, e, h, t;
  int i, j, k, l;
  real a[3], b[3], c[3], d[3], coord[3];
  real *energy, *area;
  real coef, area0;
  real theta, eng, rxy, phi;
  real area_tot_tri, area_tot_split;
  real energy_tot;
  real theta0;
  
  coef   = 2.0*sqrt(3.0);
  theta0 = 0.0;
  
  MALLOC(NV, &energy);
  MALLOC(NV, &area);

  for (v = 0; v < NV; v++) {

    energy[v] = 0;
    area[v]   = 0;
    
  }
  
  for (e = 0; e < NE; e++) {
    
    h = hdg_edg(e);

    if ( bnd(h) ) continue;
    
    i = D0[e]; j = D1[e]; k = D2[e]; l = D3[e];
    
    get4(i, j, k, l, /**/ a, b, c, d);
    
    theta = tri_dih(a, b, c, d);
    eng = coef*(theta-theta0)*(theta-theta0)/2.0;
    eng /= 4;
    energy[i] += eng;
    energy[j] += eng;
    energy[k] += eng;
    energy[l] += eng;
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

  printf("#1 azimuth angle 2 axis dist; 3 enegy; 4 energy density; 5 curvature mean; 6 area\n");
  
  for (v = 0; v < NV; v++) {

    /*for verification*/
    area_tot_split += area[v];
    energy_tot     += energy[v];
    
    vec_get(v, XX, YY, ZZ, coord);
    
    vec_get(v, XX, YY, ZZ, coord);
    rxy = vec_cylindrical_r(coord);
    
    phi = TH[v];
    if ( phi > pi / 2) {
      phi = pi - phi;
    }
    
    printf("%g %g %g %g %g %g\n", phi, rxy, energy[v], energy[v]/area[v], 0.0, area[v]);
    
  }

  printf("###NT, area_tot_tri, area_tot_split, energy_tot\n");
  printf("##%i %g %g %g\n", NT, area_tot_tri, area_tot_split, energy_tot);
  
  FREE(energy);
  FREE(area);

}

int main() {
  ini("/dev/stdin");
  energy_kantor_nelson();
  fin();
  return CO_OK;
}
