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

void energy_gompper_kroll1() {
  /*This applies halfedge to traverse*/
  enum {X, Y, Z};
  int v;
  int i, j, k, l;
  int h0, h, n, nn, fnn;
  real a[3], b[3], c[3], d[3], u[3], u2, ci, cl;
  real A, A0;
  
  A = 0;
  for (v = 0; v < NV; v++) {
    h0 = h = hdg_ver(v);
        do {
	  if (bnd(h)) break;
	  n = nxt(h); nn = nxt(nxt(h)); fnn = nxt(nxt(flp(h)));
	  j = ver(h); k = ver(n); i = ver(nn); l = ver(fnn);
	  get4(i, j, k, l, /**/ a, b, c, d);
	  ci = tri_cot(c, a, b);
	  cl = tri_cot(b, d, c);
	  vec_minus(b, c,  u);
	  u2 = vec_dot(u, u);
	  A0 = (ci + cl) * u2;
	  A += A0;
	  h = nxt(flp(h));
        } while (h != h0);
  }
  A /= 8;
  
  printf("NT=%i, traverse halfedge A = %g\n", NT, A);
}

void energy_gompper_kroll2() {
  /*This utilize edge/diheral to traverse*/
  int v, e;
  int i, j, k, l;
  real a[3], b[3], c[3], d[3], u[3], coord[3], ci, cl, cil;
  real *lbx, *lby, *lbz, *curva_mean, *energy, *area;
  real len, len2, area0;
  real rxy, phi;
  
  MALLOC(NV, &lbx);
  MALLOC(NV, &lby);
  MALLOC(NV, &lbz);
  MALLOC(NV, &curva_mean);
  MALLOC(NV, &energy);
  MALLOC(NV, &area);
  
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

  printf("#1azimuth angle; 2 axis dist; 3 enegy; 4 energy density; 5 curvature mean\n");
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
    
    energy[v] = 2 * curva_mean[v]* curva_mean[v] * area[v];

    vec_get(v, XX, YY, ZZ, coord);
    rxy = vec_cylindrical_r(coord);

    phi = TH[v];
    if ( phi > pi / 2) {
      phi = pi - phi;
    }
    
    printf("%g %g %g %g %g\n", phi, rxy, energy[v], energy[v]/area[v], curva_mean[v]);
    
  }

  FREE(lbx);
  FREE(lby);
  FREE(lbz);
  FREE(curva_mean);
  FREE(energy);
  FREE(area); 
  
}


int main() {
  ini("/dev/stdin");
  energy_gompper_kroll2();
  fin();
  return HE_OK;
}
