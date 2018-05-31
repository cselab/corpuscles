#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <real.h>
#include <he/err.h>
#include <he/x.h>
#include <he/vec.h>
#include <he/tri.h>
#include <he/memory.h>

static void get3(int i, int j, int k, /**/
          real a[3], real b[3], real c[3]) {
    vec_get(i, XX, YY, ZZ, a);
    vec_get(j, XX, YY, ZZ, b);
    vec_get(k, XX, YY, ZZ, c);
}

static void get_edg(int i, int j, real r[3]) {
    real a[3], b[3];
    vec_get(i, XX, YY, ZZ, a);
    vec_get(j, XX, YY, ZZ, b);
    vec_minus(a, b, r);
}

static void mesh_T(real *cot, /**/ real *H) {
    int h, f;
    real T;
    for (h = 0; h < NH; h++) {
        T  = cot[h];
        if ((f = flp(h)) != -1)
            T += cot[f];
        H[h] = T;
    }
}

static void mesh_cot(real *H) {
    int h, n, nn;
    int i, j, k;
    real a[3], b[3], c[3];
    for (h = 0; h < NH; h++) {
        n = nxt(h); nn = nxt(n);
	i = ver(h); j = ver(n); k = ver(nn);
        get3(i, j, k, /**/ a, b, c);
	H[h] = tri_cot(a, b, c);
    }
}

static void mesh_l2(real *H) {
    int h, n;
    int i, j;
    real r[3];
    for (h = 0; h < NH; h++) {
        n = nxt(h);
	i = ver(h); j = ver(n);
        get_edg(i, j, /**/ r);
	H[h] = vec_dot(r, r);
    }
}

static void mesh_voronoi(real *T, real *l2, /**/ real *H) {
    int h;
    for (h = 0; h < NH; h++)
	H[h] = T[h]*l2[h]/4;
}

static real sum(int n, real *a) {
    int i;
    real s;
    for (i = 0, s = 0; i < n; i++, s += a[i]);
    return s;
}

static void main0() {
    real *cot, *l2, *A, *T;
    RZERO(NH, &cot);
    RZERO(NH, &l2);
    RZERO(NH, &T);
    RZERO(NH, &A);
    
    mesh_cot(cot);
    mesh_T(cot, /**/ T);
    mesh_l2(l2);
    mesh_voronoi(cot, l2, /**/ A);
    MSG("%g", sum(NH, A));
    
    FREE(cot);
    FREE(l2); FREE(A); FREE(T);
}

int main() {
    ini("/dev/stdin");
    main0();
    fin();
    return HE_OK;
}
