#include <stdio.h>

#include <real.h>
#include <co/err.h>
#include <co/matrix.h>
#include <co/macro.h>
#include <co/util.h>

#define FMT CO_REAL_OUT
static const char **argv;

static int eq(const char *a, const char *b) { return util_eq(a, b); }
int main(__UNUSED int argc, const char **v0) {
    const char *op;
    int M, N, N0, K;
    real *a, *b, c[999];

    argv = v0;
    argv++;
    if (*argv == NULL)
        ER("mssing OP");
    op = *argv++;
    if (eq(op, "nn")) {
        matrix_fread(stdin, &M,  &N, &a);
        matrix_fread(stdin, &N0, &K, &b);
        if (N0 != N) ER("N0=%d != N=%d", N0, N);
        matrix_mult_nn(M, N, K, a, b, c);
    } else if (eq(op, "nt")) {
        matrix_fread(stdin, &M,  &N, &a);
        matrix_fread(stdin, &K, &N0, &b);
        if (N0 != N) ER("N0=%d != N=%d", N0, N);
        matrix_mult_nt(M, N, K, a, b, c);
    } else if (eq(op, "tn")) {
        matrix_fread(stdin, &N,  &M, &a);
        matrix_fread(stdin, &N0, &K, &b);
        if (N0 != N) ER("N0=%d != N=%d", N0, N);
        matrix_mult_tn(M, N, K, a, b, c);
    } else if (eq(op, "tt")) {
        matrix_fread(stdin, &N,  &M, &a);
        matrix_fread(stdin, &K, &N0, &b);
        if (N0 != N) ER("N0=%d != N=%d", N0, N);
        matrix_mult_nn(M, N, K, a, b, c);
    } else
        ER("unknown operation '%s'", op);
    matrix_fwrite(M, K, c, stdout);
    matrix_free(a); matrix_free(b);
}
