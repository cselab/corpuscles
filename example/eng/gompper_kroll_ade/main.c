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

void energy_gompper_kroll() {
  /*This routine calculates bending energy
    according to Gompper and Kroll, J. Phys. I France 1996
    
    It traverses each vertex and its associated halfedge 
    to calculate energy and area.
    
    The energy due to spontaneous curvature is considered.
    The energy due to area-difference elasticity is considered.

    Version: Xin Bian, 05 June 2018 @CSE Lab, ETH Zurich*/
  
  enum {X, Y, Z};
  int v, t;
  int i, j, k, l;
  int h0, h, n, nn, fnn;
  real a[3], b[3], c[3], d[3], u[3], uu[3], coord[3], ci, cl, cil;
  real *lbx, *lby, *lbz;
  real *curva_mean, *curva_gauss, *normx, *normy, *normz;
  real *energy, *area;
  real theta_a, theta_b, theta_c;
  real len, len2, area0;
  real rxy, phi;
  real area_tot_tri, area_tot_voronoi;

  real kB, kA, H0, C0, D, Delta_A0, Delta_a0, cm_intga, Delta_A;
  real energy_tot_hel, energy_tot_ade, energy_tot;

  
  kB  = 1.0;
  kA  = 2*kB/pi;
  C0  = -1.0;
  H0  = C0/2.0;
  D   = 4.0e-3/3.91;
  Delta_a0 = (1+kB/pi/kA)*D*C0;

  MALLOC(NV, &lbx);
  MALLOC(NV, &lby);
  MALLOC(NV, &lbz);
  MALLOC(NV, &curva_mean);
  MALLOC(NV, &curva_gauss);
  MALLOC(NV, &normx);
  MALLOC(NV, &normy);
  MALLOC(NV, &normz);
  MALLOC(NV, &energy);
  MALLOC(NV, &area);

  for (v = 0; v < NV; v++) {
    
    lbx[v] = 0;
    lby[v] = 0;
    lbz[v] = 0;
    curva_mean[v]  = 0;
    curva_gauss[v] = 0;
    normx[v]  = 0;
    normy[v]  = 0;
    normz[v]  = 0;
    energy[v] = 0;
    area[v]   = 0;
    
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
      
      lbx[j]  += cil*u[X]/2;
      lby[j]  += cil*u[Y]/2;
      lbz[j]  += cil*u[Z]/2;
      
      h = nxt(flp(h));
      
    } while (h != h0);
    
  }

  /*for verification, to be deleted*/
  area_tot_tri = 0;
  
  for (t = 0; t < NT; t++) {
    
    i = T0[t]; j = T1[t]; k = T2[t];
    
    get3(i, j, k, a, b, c);

    theta_a = tri_angle(c, a, b);
    theta_b = tri_angle(a, b, c);
    theta_c = tri_angle(b, c, a);

    curva_gauss[i] -= theta_a;
    curva_gauss[j] -= theta_b;
    curva_gauss[k] -= theta_c;

    tri_normal(a, b, c, u);
    
    normx[i] += theta_a * u[X];
    normy[i] += theta_a * u[Y];
    normz[i] += theta_a * u[Z];

    normx[j] += theta_b * u[X];
    normy[j] += theta_b * u[Y];
    normz[j] += theta_b * u[Z];

    normx[k] += theta_c * u[X];
    normy[k] += theta_c * u[Y];
    normz[k] += theta_c * u[Z];

    area0 = tri_area(a, b, c);    
    area_tot_tri += area0;

  }
  
  area_tot_voronoi = 0;
  energy_tot_hel   = 0;

  cm_intga = 0;

  printf("#1 azimuth angle; 2 axis dist; 3 enegy; 4 energy density; 5 curvature mean; 6 area\n");

  for ( v = 0; v < NV; v++ ) {

    lbx[v] /=area[v];
    lby[v] /=area[v];
    lbz[v] /=area[v];
    
    u[X]  = lbx[v];
    u[Y]  = lby[v];
    u[Z]  = lbz[v];

    uu[X] = normx[v];
    uu[Y] = normy[v];
    uu[Z] = normz[v];

    len = vec_dot(uu, uu);
    len = sqrt(len);
    
    uu[X] = normx[v]/len;
    uu[Y] = normy[v]/len;
    uu[Z] = normz[v]/len;
    
    curva_mean[v] = vec_dot(u, uu);
    curva_mean[v] /= 2;

    curva_gauss[v] = ( curva_gauss[v] + 2 * pi ) / area[v];
    
    energy[v] = 2 * (curva_mean[v]-H0) * (curva_mean[v]-H0) * area[v];

    /*calculate the integral of mean curvature over surface area*/
    cm_intga += curva_mean[v] * area[v];
   
    /*for verification, to be deleted*/
    area_tot_voronoi += area[v];
    energy_tot_hel   += energy[v];

    vec_get(v, XX, YY, ZZ, coord);
    rxy = vec_cylindrical_r(coord);

    phi = TH[v];
    if ( phi > pi / 2) {
      phi = pi - phi;
    }
    
    printf("%g %g %g %g %g %g\n", phi, rxy, energy[v], energy[v]/area[v], curva_mean[v], area[v]);
    
  }

  Delta_A  = 2 * D * cm_intga;
  Delta_A0 = 2 * D * H0 * area_tot_tri;
  energy_tot_ade = (Delta_A - Delta_A0)*(Delta_A - Delta_A0)*pi*kA/2/area_tot_tri/D/D;

  energy_tot = energy_tot_hel + energy_tot_ade;

  printf("###NT, area_tot_tri, area_tot_voronoi, energy_tot_hel, energy_tot_ade, energy_tot, cm integ over area\n");
  printf("##%i %g %g %g %g %g %g\n", NT, area_tot_tri, area_tot_voronoi, energy_tot_hel, energy_tot_ade, energy_tot, cm_intga);
  
  FREE(lbx);
  FREE(lby);
  FREE(lbz);
  FREE(curva_mean);
  FREE(curva_gauss);
  FREE(normx);
  FREE(normy);
  FREE(normz);
  FREE(energy);
  FREE(area);
  
}

int main() {
  ini("/dev/stdin");
  energy_gompper_kroll();
  fin();
  return CO_OK;
}
