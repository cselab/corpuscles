#include <stdio.h>
#include <stdlib.h>
#include <real.h>
#include <co/err.h>
#include <co/memory.h>
#include <co/macro.h>
#include <co/matrix.h>
#include <alg/linsolve.h>

#define FMT CO_REAL_OUT

int
main(void)
{
    real *A, *b, *x;
    int n, m, n0, i;
    LinSolve *l;

    matrix_fread(stdin, &n, &m, &A);
    if (n != m)
        ER("n=%d != m=%d", n, m);

    matrix_fread(stdin, &n0, &m, &b);
    if (n0 != n)
        ER("n0=%d != n=%d", n0, n);
    if (m != 1)
        ER("m=%d != 1", m);

    MALLOC(n, &x);

    lin_solve_ini(n, &l);
    lin_solve_apply(l, A, b, x);

    MSG("n %d", n);
    MSG("m %d", m);
    for (i = 0; i < n; i++)
        printf(FMT "\n", x[i]);

    matrix_fin(A);
    matrix_fin(b);
    FREE(x);
    lin_solve_fin(l);
}

/*
m lint
cat data/A data/b | ./main
*/
