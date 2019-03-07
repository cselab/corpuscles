#include <stdio.h>

#include <real.h>
#include <co/matrix.h>

#define FMT CO_REAL_OUT

int main(void) {
    int M, N, K;
    real a[] = {1, 2,  3, 4};
    real b[] = {10, 20,  30, 40};
    real c[999];
    M = N = K = 2;
    matrix_mult_nn(M, N, K, a, b, c);
    matrix_fwrite(M, K, c, stdout);
}
