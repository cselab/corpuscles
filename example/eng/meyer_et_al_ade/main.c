#include <stdio.h>
#include <tgmath.h>

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

void energy_meyer_et_al() {
  /*This routine calculates bending energy
    according to Meyer et al, 2003
    
    It traverses each triangle to calculate energy and area.

    The energy due to spontaneous curvature is considered.
    The energy due to area-difference elasticity is considered.
    
    Version: Xin Bian, 05 June 2018 @CSE Lab, ETH Zurich*/

  enum {X, Y, Z};
  int v, t;
  int i, j, k;
  real a[3], b[3], c[3], u[3], uu[3], coord[3];
  real *lbx, *lby, *lbz;
  real *curva_mean, *curva_gauss, *normx, *normy, *normz;
  real *energy, *area;
  real theta_a, theta_b, theta_c;
  real len, area0;
  real rxy, phi;
  int  obtuse;
  real theta0;
  real cota,cotb,cotc;
  real ab2, bc2, ca2;
  real area_tot_tri, area_tot_mix;
  
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

  area_tot_tri = 0;
  
  for ( t = 0; t < NT; t++ ) {
    
    /*At first, assume it is not an obtuse triangle*/
    obtuse = 0;
    
    i = T0[t]; j = T1[t]; k = T2[t];
    
    get3(i, j, k, a, b, c);
    area0 = tri_area(a, b, c);

    area_tot_tri += area0;
    
    cota = tri_cot(c, a, b);
    cotb = tri_cot(a, b, c);
    cotc = tri_cot(b, c, a);

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

    vec_minus(a, b,  u);
    
    lbx[i] += cotc*u[X]/2;
    lby[i] += cotc*u[Y]/2;
    lbz[i] += cotc*u[Z]/2;

    lbx[j] -= cotc*u[X]/2;
    lby[j] -= cotc*u[Y]/2;
    lbz[j] -= cotc*u[Z]/2;
    
    ab2 = vec_dot(u, u);

    vec_minus(b, c, u);
    
    lbx[j] += cota*u[X]/2;
    lby[j] += cota*u[Y]/2;
    lbz[j] += cota*u[Z]/2;

    lbx[k] -= cota*u[X]/2;
    lby[k] -= cota*u[Y]/2;
    lbz[k] -= cota*u[Z]/2;

    bc2 = vec_dot(u, u);

    vec_minus(c, a,  u);
    
    lbx[k] += cotb*u[X]/2;
    lby[k] += cotb*u[Y]/2;
    lbz[k] += cotb*u[Z]/2;

    lbx[i] -= cotb*u[X]/2;
    lby[i] -= cotb*u[Y]/2;
    lbz[i] -= cotb*u[Z]/2;    
    
    ca2 = vec_dot(u, u);
    
    /*check if angle a is obtuse*/
    theta0 = tri_angle(c, a, b);
    
    if ( theta0 > pi/2.0 ) {
      
      area[i] += area0/2;
      area[j] += area0/4;
      area[k] += area0/4;
      
      obtuse = 1;
    }
    else {
      /*check if angle b is obtuse*/
      theta0=tri_angle(a, b, c);
      
      if ( theta0 > pi/2.0 ) {
	
	area[j] += area0/2;
	area[i] += area0/4;
	area[k] += area0/4;
	
	obtuse = 1;
      }
      else {
	/*check if angle c is obtuse*/
	theta0=tri_angle(b, c, a);
	
	if ( theta0 > pi/2.0 ) {
	  
	  area[k] += area0/2;
	  area[i] += area0/4;
	  area[j] += area0/4;
	  
	  obtuse = 1;
	}
	
      }
      
    }
    
    /*self verificaton*/
    /*if ( obtuse == 1 ) {
      printf("a[]=%g %g %g\n", a[0], a[1], a[2]);
      printf("b[]=%g %g %g\n", b[0], b[1], b[2]);
      printf("c[]=%g %g %g\n", c[0], c[1], c[2]);
      }*/
    
    /*if it is not obtuse triangle, 
      we calculte its area split due to voronoi tesselation*/
    if ( obtuse == 0 ) {
      
      area[i] += ( ab2*cotc + ca2*cotb ) / 8;
      area[j] += ( bc2*cota + ab2*cotc ) / 8;
      area[k] += ( ca2*cotb + bc2*cota ) / 8;
      
    }
    
  }

  area_tot_mix   = 0;
  energy_tot_hel = 0;

  cm_intga = 0;

  printf("#1 azimuth angle; 2 axis dist; 3 enegy; 4 energy density; 5 curvature mean; 6 area; 7 curvature gauss\n");
  for ( v = 0; v < NV; v++ ) {
    
    lbx[v] /=area[v];
    lby[v] /=area[v];
    lbz[v] /=area[v];

    u[X] = lbx[v];
    u[Y] = lby[v];
    u[Z] = lbz[v];

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

    /*calculat the integral of mean curvature over surface area*/
    cm_intga += curva_mean[v] * area[v];

    /*for verification*/
    area_tot_mix   += area[v];
    energy_tot_hel += energy[v];
    
    vec_get(v, XX, YY, ZZ, coord);
    rxy = vec_cylindrical_r(coord);

    phi = TH[v];
    if ( phi > pi / 2) {
      phi = pi - phi;
    }
    
    printf("%g %g %g %g %g %g %g\n", phi, rxy, energy[v], energy[v]/area[v], curva_mean[v], area[v], curva_gauss[v]);
    
  }

  Delta_A  = 2 * D * cm_intga;
  Delta_A0 = 2 * D * H0 * area_tot_tri;
  energy_tot_ade = (Delta_A - Delta_A0)*(Delta_A - Delta_A0)*pi*kA/2/area_tot_tri/D/D;
  
  energy_tot = energy_tot_hel + energy_tot_ade;

  printf("###NT, area_tot_tri, area_tot_mix, energy_tot_hel, energy_tot_ade, energy_tot, cm integ over area\n");
  printf("##%i %g %g %g %g %g %g\n", NT, area_tot_tri, area_tot_mix, energy_tot_hel, energy_tot_ade, energy_tot, cm_intga);

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
  energy_meyer_et_al();
  fin();
  return HE_OK;
}
