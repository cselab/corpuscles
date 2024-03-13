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

void energy_meyer_et_al1() {
  /*This routine calculates bending energy
    according to Meyer et al, 2003
    
    It traverses each triangle to calculate energy and area.

    Version: Xin Bian, 05 June 2018 @CSE Lab, ETH Zurich*/

  enum {X, Y, Z};
  int v, t;
  int i, j, k;
  real a[3], b[3], c[3], u[3], coord[3];
  real *lbx, *lby, *lbz, *curva_mean, *energy, *area;
  real len, area0;
  real rxy, phi;
  int  obtuse;
  real theta0;
  real cota,cotb,cotc;
  real ab2, bc2, ca2;
  real area_tot_tri, area_tot_mix;
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

    vec_minus(a, b,  u);
    
    lbx[i] += cotc*u[0]/2;
    lby[i] += cotc*u[1]/2;
    lbz[i] += cotc*u[2]/2;

    lbx[j] -= cotc*u[0]/2;
    lby[j] -= cotc*u[1]/2;
    lbz[j] -= cotc*u[2]/2;
    
    ab2 = vec_dot(u, u);

    vec_minus(b, c, u);
    
    lbx[j] += cota*u[0]/2;
    lby[j] += cota*u[1]/2;
    lbz[j] += cota*u[2]/2;

    lbx[k] -= cota*u[0]/2;
    lby[k] -= cota*u[1]/2;
    lbz[k] -= cota*u[2]/2;

    bc2 = vec_dot(u, u);

    vec_minus(c, a,  u);
    
    lbx[k] += cotb*u[0]/2;
    lby[k] += cotb*u[1]/2;
    lbz[k] += cotb*u[2]/2;

    lbx[i] -= cotb*u[0]/2;
    lby[i] -= cotb*u[1]/2;
    lbz[i] -= cotb*u[2]/2;    
    
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

  area_tot_mix = 0;
  energy_tot   = 0;
  
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
    
    energy[v] = 2 * (curva_mean[v] - H0) * (curva_mean[v] - H0) * area[v];

    /*for verification*/
    area_tot_mix += area[v];
    energy_tot   += energy[v];
    
    vec_get(v, XX, YY, ZZ, coord);
    rxy = vec_cylindrical_r(coord);

    phi = TH[v];
    if ( phi > pi / 2) {
      phi = pi - phi;
    }
    
    printf("%g %g %g %g %g %g\n", phi, rxy, energy[v], energy[v]/area[v], curva_mean[v], area[v]);
    
  }

  printf("###NT, area_tot_tri, area_tot_mix, energy_tot\n");
  printf("##%i %g %g %g\n", NT, area_tot_tri, area_tot_mix, energy_tot);

  FREE(lbx);
  FREE(lby);
  FREE(lbz);
  FREE(curva_mean);
  FREE(energy);
  FREE(area);
  
}

void energy_meyer_et_al2() {
  /*This routine calculates bending energy
    according to Meyer et al, 2003
    
    It traverses each edge/dihedral to calculate energy and
    it trasverses each triangle to calculate area.

    Version: Xin Bian, 05 June 2018 @CSE Lab, ETH Zurich*/

  int v, e, t;
  int i, j, k, l;
  real a[3], b[3], c[3], d[3], u[3], coord[3], coti, cotl, cotil;
  real *lbx, *lby, *lbz, *curva_mean, *energy, *area;
  real len, len2, area0;
  real rxy, phi;
  int  obtuse;
  real theta0;
  real cota,cotb,cotc;
  real ab2, bc2, ca2;
  real area_tot_tri, area_tot_mix;
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
    
    coti = tri_cot(c, a, b);
    cotl = tri_cot(b, d, c);
    cotil = coti + cotl;
    
    vec_minus(b, c,  u);
    len2 = vec_dot(u, u);
    
    /*
      area0    = cil * len2 / 8;
      area[j] += area0;
      area[k] += area0;
    */
    
    lbx[j]  += cotil*u[0]/2;
    lby[j]  += cotil*u[1]/2;
    lbz[j]  += cotil*u[2]/2;
    
    lbx[k]  -= cotil*u[0]/2;
    lby[k]  -= cotil*u[1]/2;
    lbz[k]  -= cotil*u[2]/2;

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
    
  }

  area_tot_mix = 0;
  energy_tot   = 0;
  
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
    
    energy[v] = 2 * (curva_mean[v]-H0) * (curva_mean[v]-H0) * area[v];

    /*for verification*/
    area_tot_mix += area[v];
    energy_tot   += energy[v];
    
    vec_get(v, XX, YY, ZZ, coord);
    rxy = vec_cylindrical_r(coord);

    phi = TH[v];
    if ( phi > pi / 2) {
      phi = pi - phi;
    }
    
    printf("%g %g %g %g %g %g\n", phi, rxy, energy[v], energy[v]/area[v], curva_mean[v], area[v]);
    
  }

  printf("#NT, area_tot_tri, area_tot_mix, energy_tot\n");
  printf("#%i %g %g %g\n", NT, area_tot_tri, area_tot_mix, energy_tot);

  FREE(lbx);
  FREE(lby);
  FREE(lbz);
  FREE(curva_mean);
  FREE(energy);
  FREE(area); 
  
}


int main() {
  ini("/dev/stdin");
  energy_meyer_et_al1();
  energy_meyer_et_al2();
  fin();
  return CO_OK;
}
