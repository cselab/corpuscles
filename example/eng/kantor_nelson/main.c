#include <stdio.h>
#include <math.h>

#include <real.h>
#include <he/err.h>
#include <he/x.h>
#include <he/vec.h>
#include <he/tri.h>
#include <he/memory.h>

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
    
    It traverses each edge/dihedral to calculate energy.
    Splitting energy of each dihedral to vertices has ambiguity and
    we simply split equaly 4 times to each vertex involved.

    Version: Xin Bian, 05 June 2018 @CSE Lab, ETH Zurich*/

  enum {X, Y, Z};
  int v, e, h;
  int i, j, k, l;
  real a[3], b[3], c[3], d[3], coord[3];
  real *energy;
  real coef;
  real theta, eng, rxy, phi;
  real energy_tot;

  coef = 2.0*sqrt(3.0);
  
  MALLOC(NV, &energy);
  
  for (v = 0; v < NV; v++) energy[v] = 0;
  
  for (e = 0; e < NE; e++) {
    
    h = hdg_edg(e);

    if ( bnd(h) ) continue;
    
    i = D0[e]; j = D1[e]; k = D2[e]; l = D3[e];
    
    get4(i, j, k, l, /**/ a, b, c, d);
    
    theta = tri_dih(a, b, c, d);
    eng = coef*(1 - cos(theta));
    eng /= 4;
    energy[i] += eng;
    energy[j] += eng;
    energy[k] += eng;
    energy[l] += eng;
  }

  energy_tot = 0;

  printf("#1azimuth angle 2 axis dist; 3 enegy;\n");
  
  for (v = 0; v < NV; v++) {

    /*for verification*/
    energy_tot     += energy[v];

    vec_get(v, XX, YY, ZZ, coord);
    
    vec_get(v, XX, YY, ZZ, coord);
    rxy = vec_cylindrical_r(coord);
    
    phi = TH[v];
    if ( phi > pi / 2) {
      phi = pi - phi;
    }
    
    printf("%g %g %g\n", phi, rxy, energy[v]);
    
    }

  printf("#NT, energy_tot\n");
  printf("#%i %g\n", NT, energy_tot);

    FREE(energy);
}

int main() {
  ini("/dev/stdin");
  energy_kantor_nelson();
  fin();
  return HE_OK;
}
