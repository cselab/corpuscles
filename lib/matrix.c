#include <stdio.h>

#include "real.h"
#include "co/array.h"
#include "co/err.h"
#include "co/memory.h"
#include "co/matrix.h"

#define OUT CO_REAL_OUT
#define IN  CO_REAL_IN

#define BEGIN \
    for (m = 0; m < M; m++) \
        for (k = 0; k < K; k++) \
            for (n = 0; n < N; n++) {
#define END }

#define ABEGIN \
    for (m = 0; m < M; m++) \
            for (n = 0; n < N; n++) {
#define AEND }

int
matrix_zero(int M, int N, real * a)
{
    int i;

    for (i = 0; i < N * M; i++)
        a[i] = 0;
    return CO_OK;
}

int
matrix_one(int M, int N, real * a)
{
    int i;

    for (i = 0; i < N * M; i++)
        a[i] = 1;
    return CO_OK;
}

int
matrix_scale(int M, int N, real s, real * a)
{
    int i;

    for (i = 0; i < N * M; i++)
        a[i] *= s;
    return CO_OK;
}

int
matrix_set(int M, int N, int m, int n, real s, real * a)
{
    if (m >= M)
        ERR(CO_INDEX, "m=%d >= M=%d", m, N);
    if (n >= N)
        ERR(CO_INDEX, "n=%d >= N=%d", n, N);
    a[N * m + n] = s;
    return CO_OK;
}

int
matrix_add(int M, int N, int m, int n, real s, real * a)
{
    if (m >= M)
        ERR(CO_INDEX, "m=%d >= M=%d", m, N);
    if (n >= N)
        ERR(CO_INDEX, "n=%d >= N=%d", n, N);
    a[N * m + n] += s;
    return CO_OK;
}

real
matrix_get(int M, int N, int m, int n, const real * a)
{
    if (m >= M)
        ERR(CO_INDEX, "m=%d >= M=%d", m, N);
    if (n >= N)
        ERR(CO_INDEX, "n=%d >= N=%d", n, N);
    return a[N * m + n];
}

int
matrix_mult_nn(int M, int N, int K, const real * a, const real * b,
               /**/ real * c)
{
    int m, n, k;

    matrix_zero(M, K, c);
    BEGIN {
        c[K * m + k] += a[N * m + n] * b[M * n + k];
    } END;
    return CO_OK;
}

int
matrix_mult_tn(int M, int N, int K, const real * a, const real * b,
               /**/ real * c)
{
    int m, n, k;

    matrix_zero(M, K, c);
    BEGIN {
        c[K * m + k] += a[M * n + m] * b[M * n + k];
    } END;
    return CO_OK;
}

int
matrix_mult_nt(int M, int N, int K, const real * a, const real * b,
               /**/ real * c)
{
    int m, n, k;

    matrix_zero(M, K, c);
    BEGIN {
        c[K * m + k] += a[N * m + n] * b[N * k + n];
    } END;
    return CO_OK;
}

int
matrix_mult_tt(int M, int N, int K, const real * a, const real * b,
               /**/ real * c)
{
    int m, n, k;

    matrix_zero(M, K, c);
    BEGIN {
        c[K * m + k] += a[M * n + m] * b[N * k + n];
    } END;
    return CO_OK;
}

int
matrix_array_n(int M, int N, const real * a, const real * b, /**/ real * c)
{
    int m, n;

    array_zero(M, c);
    ABEGIN {
        c[m] += a[N * m + n] * b[n];
    } AEND;
    return CO_OK;
}

int
matrix_array_t(int M, int N, const real * a, const real * b, /**/ real * c)
{
    int m, n;

    array_zero(M, c);
    ABEGIN {
        c[m] += a[M * n + m] * b[n];
    } AEND;
    return CO_OK;
}

int
matrix_array_append_n(int M, int N, const real * a, const real * b, /*io */
                      real * c)
{
    int m, n;

    ABEGIN {
        c[m] += a[N * m + n] * b[n];
    } AEND;
    return CO_OK;
}

int
matrix_array_append_t(int M, int N, const real * a, const real * b, /*io */
                      real * c)
{
    int m, n;

    ABEGIN {
        c[m] += a[M * n + m] * b[n];
    } AEND;
    return CO_OK;
}

int
matrix_array_substr_n(int M, int N, const real * a, const real * b, /*io */
                      real * c)
{
    int m, n;

    ABEGIN {
        c[m] -= a[N * m + n] * b[n];
    } AEND;
    return CO_OK;
}

int
matrix_array_substr_t(int M, int N, const real * a, const real * b, /*io */
                      real * c)
{
    int m, n;

    ABEGIN {
        c[m] -= a[M * n + m] * b[n];
    } AEND;
    return CO_OK;
}

int
matrix_transpose(int M, int N, real * a)
{
    int s, n, i, K;
    real tmp;

    K = N * M;
    for (s = 0; s < K; ++s) {
        n = s;
        i = 0;
        do {
            ++i;
            n = (n % M) * N + n / M;
        } while (n > s);
        if (n >= s && i != 1) {
            tmp = a[s];
            n = s;
            do {
                i = (n % M) * N + n / M;
                a[n] = (i == s) ? tmp : a[i];
                n = i;
            } while (n > s);
        }
    }
    return CO_OK;
}

int
matrix_fwrite(int M, int N, const real * a, FILE * f)
{
    int m, n, s;

    if (fprintf(f, "%d %d\n", M, N) < 0)
        ERR(CO_IO, "fail to write");
    for (m = s = 0; m < M; m++) {
        for (n = 0; n < N; n++, s++) {
            if (n > 0)
                if (EOF == fputc(' ', f))
                    ERR(CO_IO, "fail to write");
            if (fprintf(f, OUT, a[s]) < 0)
                ERR(CO_IO, "fail to write");
        }
        if (EOF == fputc('\n', f))
            ERR(CO_IO, "fail to write");
    }
    return CO_OK;
}

int
matrix_fread(FILE * f, int *pM, int *pN, real ** pa)
{
    int M, N, n, m, s;
    real *a;

    if (EOF == fscanf(f, "%d %d\n", &M, &N))
        ERR(CO_IO, "fail to read");
    MALLOC((N * M), &a);
    for (m = s = 0; m < M; m++)
        for (n = 0; n < N; n++, s++) {
            if (EOF == fscanf(f, IN, &a[s]))
                ERR(CO_IO, "fail to read matrix: N=%d M=%d", N, M);
        }
    *pM = M;
    *pN = N;
    *pa = a;
    return CO_OK;
}

int
matrix_fin(real * a)
{
    FREE(a);
    return CO_OK;
}

int
matrix_ini(int M, int N, real ** p)
{
    if (M < 0)
        ERR(CO_INDEX, "M=%d < 0", M);
    if (N < 0)
        ERR(CO_INDEX, "N=%d < 0", N);
    MALLOC(M * N, p);
    return CO_OK;
}
