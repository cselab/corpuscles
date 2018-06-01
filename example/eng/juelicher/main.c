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

void energy_juelicher() {
    enum {X, Y, Z};
    int v, e, h, t;
    int i, j, k, l;
    real r0, ang, a[3], b[3], c[3], d[3], r[3];
    real *curva_mean, *energy, *area;
    real len, len2, are;

      
    MALLOC(NV, &curva_mean);
    MALLOC(NV, &energy);
    MALLOC(NV, &area);
    
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

	ang = tri_dih(a, b, c, d);

	vec_minus(b,c,r);
	len2 = vec_dot(r, r);
	len  = sqrt(len2);

	curva_mean[j] += len*ang/4;
	curva_mean[k] += len*ang/4;
	  
    }

    for (t = 0; t < NT; t++) {

      i = T0[t]; j = T1[t]; k = T2[t];

      get3(i, j, k, a, b, c);
      are = tri_area(a, b, c);

      area[i] += are/3;
      area[j] += are/3;
      area[k] += are/3;
      
    }

    printf("#1 axis dist; 2 curvature mean; 3 enegy; 4 energy density\n");
    for (v = 0; v < NV; v++) {
      
      curva_mean[v] /= area[v];
      energy[v] = 2 * curva_mean[v]*curva_mean[v]*area[v];
      
      vec_get(v, XX, YY, ZZ, r);
      r0 = vec_cylindrical_r(r);
      printf("%g %g %g %g\n", r0, curva_mean[v], energy[v], energy[v]/area[v]);
      
    }

    FREE(curva_mean);
    FREE(energy);
    FREE(area);
}

int main() {
    ini("/dev/stdin");
    energy_juelicher();
    fin();
    return HE_OK;
}
