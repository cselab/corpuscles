#include <stdio.h>

#include <real.h>
#include <co/err.h>
#include <co/matrix.h>

#define FMT CO_REAL_OUT

int main(void) {
    int M, N, N0, K;
    real *a, *b, *c;

    matrix_fread(stdin, &M,  &N, &a);
    matrix_fread(stdin, &N0, &K, &b);
    if (N0 != N) ER("N0=%d != N=%d", N0, N);
    matrix_ini(M, K, &c);
    matrix_mult_nn(M, N, K, a, b, c);
    matrix_fwrite(M, K, c, stdout);
    matrix_free(a); matrix_free(b); matrix_free(c);

}
