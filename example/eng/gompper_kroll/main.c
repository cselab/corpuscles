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

void energy_gompper_kroll1() {
   /*This routine calculates bending energy
    according to Gompper and Kroll, J. Phys. I France 1996
    
    It traverses each vertex and its associated halfedge 
    to calculate energy and area.

    Version: Xin Bian, 05 June 2018 @CSE Lab, ETH Zurich*/
  
  enum {X, Y, Z};
  int v, t;
  int i, j, k, l;
  int h0, h, n, nn, fnn;
  real a[3], b[3], c[3], d[3], u[3], coord[3], ci, cl, cil;
  real *lbx, *lby, *lbz;
  real *curva_mean;
  real *energy, *area;
  real len, len2, area0;
  real rxy, phi;
  real area_tot_tri, area_tot_voronoi;
  real energy_tot;

  real C0, H0;
  
  MALLOC(NV, &lbx);
  MALLOC(NV, &lby);
  MALLOC(NV, &lbz);
  MALLOC(NV, &curva_mean);
  MALLOC(NV, &energy);
  MALLOC(NV, &area);

  C0=0;
  H0=C0/2.0;
  
  for (v = 0; v < NV; v++) {
    
    lbx[v] = 0;
    lby[v] = 0;
    lbz[v] = 0;
    curva_mean[v] = 0;
    energy[v]     = 0;
    area[v]       = 0;
    
  }

  for (v = 0; v < NV; v++) {
    
    h0 = h = hdg_ver(v);
    
    do {

      if (bnd(h)) break;

      n = nxt(h); nn = nxt(n); fnn = nxt(nxt(flp(h)));
      j = ver(h); k = ver(n); i = ver(nn); l = ver(fnn);

      get4(i, j, k, l, /**/ a, b, c, d);
      
      ci  = tri_cot(c, a, b);
      cl  = tri_cot(b, d, c);
      cil = ci + cl;
      
      vec_minus(b, c,  u);
      len2 = vec_dot(u, u);

      area0    = cil * len2 / 8;
      area[j] += area0;
      
      lbx[j]  += cil*u[0]/2;
      lby[j]  += cil*u[1]/2;
      lbz[j]  += cil*u[2]/2;
      
      h = nxt(flp(h));
      
    } while (h != h0);
    
  }

  /*for verification, to be deleted*/
  area_tot_tri = 0;
  
  for (t = 0; t < NT; t++) {
    
    i = T0[t]; j = T1[t]; k = T2[t];
    
    get3(i, j, k, a, b, c);
    area0 = tri_area(a, b, c);
    
    area_tot_tri += area0;
    
  }
  
  area_tot_voronoi = 0;
  energy_tot       = 0;
  
  printf("#1 azimuth angle; 2 axis dist; 3 enegy; 4 energy density; 5 curvature mean; 6 area\n");

  for ( v = 0; v < NV; v++ ) {

    lbx[v] /=area[v];
    lby[v] /=area[v];
    lbz[v] /=area[v];

    u[0] = lbx[v];
    u[1] = lby[v];
    u[2] = lbz[v];
    len = vec_dot(u, u);
    curva_mean[v] = len;
    len = sqrt(len);
    curva_mean[v] /= len;
    curva_mean[v] /= 2;
    
    energy[v] = 2 * ( curva_mean[v] - H0) * ( curva_mean[v] - H0) * area[v];

    /*for verification, to be deleted*/
    area_tot_voronoi += area[v];
    energy_tot       += energy[v];

    vec_get(v, XX, YY, ZZ, coord);
    rxy = vec_cylindrical_r(coord);

    phi = TH[v];
    if ( phi > pi / 2) {
      phi = pi - phi;
    }
    
    printf("%g %g %g %g %g %g\n", phi, rxy, energy[v], energy[v]/area[v], curva_mean[v], area[v]);
    
  }

  printf("#NT, area_tot_tri, area_tot_voronoi, energy_tot\n");
  printf("#%i %g %g %g\n", NT, area_tot_tri, area_tot_voronoi, energy_tot);
  FREE(lbx);
  FREE(lby);
  FREE(lbz);
  FREE(curva_mean);
  FREE(energy);
  FREE(area);
  
}

void energy_gompper_kroll2() {
  /*This routine calculates bending energy
    according to Gompper and Kroll, J. Phys. I, France 1996
    
    It traverseseach edge/dihedral to calculate energy and area.

    Version: Xin Bian, 05 June 2018 @CSE Lab, ETH Zurich
  */
  int v, e, t;
  int i, j, k, l;
  real a[3], b[3], c[3], d[3], u[3], coord[3], ci, cl, cil;
  real *lbx, *lby, *lbz, *curva_mean, *energy, *area;
  real len, len2, area0;
  real rxy, phi;
  real area_tot_tri, area_tot_voronoi;
  real energy_tot;

  real C0, H0;
  
  MALLOC(NV, &lbx);
  MALLOC(NV, &lby);
  MALLOC(NV, &lbz);
  MALLOC(NV, &curva_mean);
  MALLOC(NV, &energy);
  MALLOC(NV, &area);


  C0 = 0;
  H0 = C0/2.0;
  
  for (v = 0; v < NV; v++) {
    
    lbx[v] = 0;
    lby[v] = 0;
    lbz[v] = 0;
    curva_mean[v] = 0;
    energy[v]     = 0;
    area[v]       = 0;
    
  }
  
  for (e = 0; e < NE; e++) {
    
    if (bnd(e)) continue;
    
    i = D0[e]; j = D1[e]; k = D2[e]; l = D3[e];
    
    get4(i, j, k, l, /**/ a, b, c, d);
    
    ci = tri_cot(c, a, b);
    cl = tri_cot(b, d, c);
    cil = ci + cl;
    
    vec_minus(b, c,  u);
    len2 = vec_dot(u, u);

    area0    = cil * len2 / 8;
    area[j] += area0;
    area[k] += area0;

    lbx[j]  += cil*u[0]/2;
    lby[j]  += cil*u[1]/2;
    lbz[j]  += cil*u[2]/2;

    lbx[k]  -= cil*u[0]/2;
    lby[k]  -= cil*u[1]/2;
    lbz[k]  -= cil*u[2]/2;

  }

  /*for verification, to be deleted*/
  area_tot_tri = 0;
  
  for (t = 0; t < NT; t++) {
    
    i = T0[t]; j = T1[t]; k = T2[t];
      
    get3(i, j, k, a, b, c);
    area0 = tri_area(a, b, c);
    
    area_tot_tri += area0;
    
  }
  
  area_tot_voronoi = 0;
  energy_tot       = 0;
  
  printf("#1 azimuth angle; 2 axis dist; 3 enegy; 4 energy density; 5 curvature mean; 6 area\n");
  
  for ( v = 0; v < NV; v++ ) {

    lbx[v] /=area[v];
    lby[v] /=area[v];
    lbz[v] /=area[v];

    u[0] = lbx[v];
    u[1] = lby[v];
    u[2] = lbz[v];
    len = vec_dot(u, u);
    curva_mean[v] = len;
    len = sqrt(len);
    curva_mean[v] /= len;
    curva_mean[v] /= 2;
    
    energy[v] = 2 * ( curva_mean[v] - H0) * ( curva_mean[v] - H0)  * area[v];

    /*for verification, to be deleted*/
    area_tot_voronoi += area[v];
    energy_tot       += energy[v];

    vec_get(v, XX, YY, ZZ, coord);
    rxy = vec_cylindrical_r(coord);

    phi = TH[v];
    if ( phi > pi / 2) {
      phi = pi - phi;
    }
    
    printf("%g %g %g %g %g %g\n", phi, rxy, energy[v], energy[v]/area[v], curva_mean[v], area[v]);
    
  }

  printf("###NT, area_tot_tri, area_tot_voronoi, energy_tot\n");
  printf("##%i %g %g %g\n", NT, area_tot_tri, area_tot_voronoi, energy_tot);

  FREE(lbx);
  FREE(lby);
  FREE(lbz);
  FREE(curva_mean);
  FREE(energy);
  FREE(area); 
  
}


int main() {
  ini("/dev/stdin");
  energy_gompper_kroll1();
  energy_gompper_kroll2();
  fin();
  return CO_OK;
}
