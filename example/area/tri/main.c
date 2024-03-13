#include <stdio.h>

#include <real.h>
#include <co/err.h>
/* #include <co/x.h> */
#include <co/vec.h>
#include <co/tri.h>

void get3(int i, int j, int k, /**/ real a[3], real b[3], real c[3]) {
    vec_get(i, XX, YY, ZZ, a);
    vec_get(j, XX, YY, ZZ, b);
    vec_get(k, XX, YY, ZZ, c);
}

void main0() {
    int m, i, j, k;
    real A, a[3], b[3], c[3];
    A = 0;
    for (m = 0; m < NT; m++) {
        i = T0[m]; j = T1[m]; k = T2[m];
        get3(i, j, k, a, b, c);
        A += tri_area(a, b, c);
    }
    printf("NT = %i %g\n", NT, A);
}

int main() {
    ini("/dev/stdin");
    main0();
    fin();
    return CO_OK;
}
