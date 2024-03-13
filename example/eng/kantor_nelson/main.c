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

void energy_kantor_nelson() {
  /*This routine calculates bending energy
    according to Kantor and Nelson, Phys. Rev. A 1987
    
    It traverses each edge, which has a dihedral angle, to calculate energy.
    Splitting energy of each dihedral to vertices has ambiguity and
    we simply split equaly 1/4 to each vertex involved.

    Version: Xin Bian, 05 June 2018 @CSE Lab, ETH Zurich*/

  enum {X, Y, Z};
  int v, e, h, t;
  int i, j, k, l;
  int h0, n, nn, fnn;
  real a[3], b[3], c[3], d[3], coord[3], u[3];
  real *energy, *area1, *area2, *area3;
  real coef, area0;
  real theta, eng, rxy, phi;
  real area_tot_tri, area_tot_split1, area_tot_split2, area_tot_split3;
  real energy_tot;
  real theta0, len2;
  int  obtuse;
  real ci, cl, cil;
  real cota, cotb, cotc;
  real ab2, bc2, ca2;
  
  coef   = 2.0*sqrt(3.0);
  theta0 = 0.0;
  
  MALLOC(NV, &energy);
  MALLOC(NV, &area1);
  MALLOC(NV, &area2);
  MALLOC(NV, &area3);

  for (v = 0; v < NV; v++) {

    energy[v] = 0;
    area1[v]  = 0;
    area2[v]  = 0;
    area3[v]  = 0;
    
  }
  
  for (e = 0; e < NE; e++) {
    
    h = hdg_edg(e);

    if ( bnd(h) ) continue;
    
    i = D0[e]; j = D1[e]; k = D2[e]; l = D3[e];
    
    get4(i, j, k, l, /**/ a, b, c, d);
    
    theta = tri_dih(a, b, c, d);
    eng = coef*(1 - cos(theta-theta0));
    eng /= 4;
    energy[i] += eng;
    energy[j] += eng;
    energy[k] += eng;
    energy[l] += eng;
  }

  area_tot_tri = 0;

  //use Juelicher's way to calculate area
  for (t = 0; t < NT; t++) {
      
    i = T0[t]; j = T1[t]; k = T2[t];
      
    get3(i, j, k, a, b, c);
    area0 = tri_area(a, b, c);

    area1[i] += area0/3;
    area1[j] += area0/3;
    area1[k] += area0/3;

    area_tot_tri += area0;
    
  }

  //use Gompper and Kroll's way to calculate area  
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
      area2[j] += area0;
            
      h = nxt(flp(h));
      
    } while (h != h0);
    
  }

   //use Meyer et al's way to calculate area
   for ( t = 0; t < NT; t++ ) {
    
    /*At first, assume it is not an obtuse triangle*/
    obtuse = 0;
    
    i = T0[t]; j = T1[t]; k = T2[t];
    
    get3(i, j, k, a, b, c);
    area0 = tri_area(a, b, c);

    cota = tri_cot(c, a, b);
    cotb = tri_cot(a, b, c);
    cotc = tri_cot(b, c, a);

    vec_minus(a, b,  u);
    ab2 = vec_dot(u, u);

    vec_minus(b, c, u);
    bc2 = vec_dot(u, u);

    vec_minus(c, a,  u);
    ca2 = vec_dot(u, u);

    
    /*check if angle a is obtuse*/
    theta0 = tri_angle(c, a, b);
    
    if ( theta0 > pi/2.0 ) {
      
      area3[i] += area0/2;
      area3[j] += area0/4;
      area3[k] += area0/4;
      
      obtuse = 1;
    }
    else {
      /*check if angle b is obtuse*/
      theta0=tri_angle(a, b, c);
      
      if ( theta0 > pi/2.0 ) {
	
	area3[j] += area0/2;
	area3[i] += area0/4;
	area3[k] += area0/4;
	
	obtuse = 1;
      }
      else {
	/*check if angle c is obtuse*/
	theta0=tri_angle(b, c, a);
	
	if ( theta0 > pi/2.0 ) {
	  
	  area3[k] += area0/2;
	  area3[i] += area0/4;
	  area3[j] += area0/4;
	  
	  obtuse = 1;
	}
	
      }
      
    }
   
    if ( obtuse == 0 ) {
      
      area3[i] += ( ab2*cotc + ca2*cotb ) / 8;
      area3[j] += ( bc2*cota + ab2*cotc ) / 8;
      area3[k] += ( ca2*cotb + bc2*cota ) / 8;
      
    }
    
  }
    
  area_tot_split1 = 0;
  area_tot_split2 = 0;
  area_tot_split3 = 0;
  energy_tot      = 0;

  printf("#1 azimuth angle 2 axis dist; 3 enegy; 4 energy density; 5 curvature mean; 6 area(J); 7 area(GK); 8 area(mix)\n");
  
  for (v = 0; v < NV; v++) {

    /*for verification*/
    area_tot_split1 += area1[v];
    area_tot_split2 += area2[v];
    area_tot_split3 += area3[v];
    energy_tot      += energy[v];
    
    vec_get(v, XX, YY, ZZ, coord);
    
    vec_get(v, XX, YY, ZZ, coord);
    rxy = vec_cylindrical_r(coord);
    
    phi = TH[v];
    if ( phi > pi / 2) {
      phi = pi - phi;
    }
    
    printf("%g %g %g %g %g %g %g %g\n", phi, rxy, energy[v], energy[v]/area1[v], 0.0, area1[v], area2[v], area3[v]);
    
  }

  printf("###NT, area_tot_tri, area_tot_split, energy_tot\n");
  printf("##%i %g %g %g %g %g\n", NT, area_tot_tri, area_tot_split1, energy_tot, area_tot_split2, area_tot_split3);
  
  FREE(energy);
  FREE(area1);
  FREE(area2);
  FREE(area3);

}

int main() {
  ini("/dev/stdin");
  energy_kantor_nelson();
  fin();
  return CO_OK;
}
