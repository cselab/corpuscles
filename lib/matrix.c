#include <stdio.h>

#include "real.h"
#include "co/err.h"
#include "co/matrix.h"

#define FMT CO_REAL_OUT

#define BEGIN                                   \
    for (m = 0; m < M; m++)                     \
        for (k = 0; k < K; k++)                 \
            for (n = 0; n < N; n++) {
#define END }

int matrix_zero(int M, int N, real *a) {
    int i;
    for (i = 0; i < N*M; i++)
        a[i] = 0;
    return CO_OK;
}

int matrix_mult_nn(int M, int N, int K, const real *a, const real *b, /**/ real *c) {
    int m, n, k;
    BEGIN {
        c[K*m + k] += a[N*m + n] * b[M*n + k];
    } END;
    return CO_OK;
}

int matrix_mult_tn(int M, int N, int K, const real *a, const real *b, /**/ real *c) {
    int m, n, k;
    matrix_zero(M, K, c);
    BEGIN {
        c[K*m + k] += a[M*n + m] * b[M*n + k];
    } END;
    return CO_OK;
}

int matrix_mult_nt(int M, int N, int K, const real *a, const real *b, /**/ real *c) {
    int m, n, k;
    BEGIN {
        c[K*m + k] += a[N*m + n] * b[N*k + n];
    } END;
    return CO_OK;
}

int matrix_mult_tt(int M, int N, int K, const real *a, const real *b, /**/ real *c) {
    int m, n, k;
    BEGIN {
        c[K*m + k] += a[M*n + m] * b[N*k + n];
    } END;
    return CO_OK;
}

int matrix_fwrite(int M, int N, const real *a, FILE *f) {
    int m, n, s;
    for (m = s = 0; m < M; m++) {
        for (n = 0; n < N; n++, s++) {
            if (n > 0)
                if (EOF == fputc(' ', f))
                    ERR(CO_IO, "fail to write");
            if (fprintf(f, FMT, a[s]) < 0)
                ERR(CO_IO, "fail to write");
        }
        if (EOF == fputc('\n', f))
            ERR(CO_IO, "fail to write");
    }
    return CO_OK;
}
