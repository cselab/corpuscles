#include <stdio.h>

#include <real.h>
#include <he/err.h>
#include <he/x.h>
#include <he/vec.h>
#include <he/tri.h>

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
    printf("%g\n", A);
    
}

int main() {
    ini("/dev/stdin");
    main0();
    fin();
    return HE_OK;
}
