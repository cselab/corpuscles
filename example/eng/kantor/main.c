#include <stdio.h>
#include <math.h>

#include <real.h>
#include <co/err.h>
/* #include <co/x.h> */
#include <co/vec.h>
#include <co/tri.h>
#include <co/memory.h>

void get4(int i, int j, int k, int l, /**/
          real a[3], real b[3], real c[3], real d[3]) {
    vec_get(i, XX, YY, ZZ, a);
    vec_get(j, XX, YY, ZZ, b);
    vec_get(k, XX, YY, ZZ, c);
    vec_get(l, XX, YY, ZZ, d);    
}

void main0() {
    enum {X, Y, Z};
    int v, e;
    int i, j, k, l;
    real r0, ang, eng, a[3], b[3], c[3], d[3], r[3];
    real *E;
    MALLOC(NV, &E);
    for (v = 0; v < NV; v++) E[v] = 0;
    for (e = 0; e < NE; e++) {
        i = D0[e]; j = D1[e]; k = D2[e]; l = D3[e];
        get4(i, j, k, l, /**/ a, b, c, d);
        ang = tri_dih(a, b, c, d);
        eng = 2*(1 - cos(ang));
        eng /= 4;
        E[i] += eng; E[j] += eng; E[k] += eng; E[l] += eng;
    }
    for (v = 0; v < NV; v++) {
        vec_get(v, XX, YY, ZZ, r);
        r0 = vec_cylindrical_r(r);
        printf("%g %g\n", r0, E[v]);
    }
    FREE(E);
}

int main() {
    ini("/dev/stdin");
    main0();
    fin();
    return CO_OK;
}
