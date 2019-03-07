#include "real.h"
#include "co/err.h"
#include "co/matrix.h"

int matrix_zero(int M, int N, real *a) {
    int i;
    for (i = 0; i < N*M; i++)
        a[i] = 0;
    return CO_OK;
}

int matrix_mult_nn(int M, int N, int K, const real *a, const real *b, /**/ real *c) {
    int m, n, k;
    matrix_zero(M, K, c);
    for (m = 0; m < M; m++)
        for (k = 0; k < K; k++)
            for (n = 0; n < N; n++) ;
    return CO_OK;
}
