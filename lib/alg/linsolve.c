#include <stdio.h>
#include <gsl/gsl_linalg.h>
#include <real.h>
#include "co/err.h"
#include "co/macro.h"
#include "co/matrix.h"
#include "co/memory.h"
#include "co/real.h"
#include "alg/linsolve.h"

#define T LinSolve
struct T {
    int n;
    gsl_permutation *p;
    gsl_matrix *A;
    gsl_vector *b, *x;
};

int
lin_solve_ini(int n, T ** pq)
{
    T *q;
    gsl_permutation *p;
    gsl_matrix *A;
    gsl_vector *b, *x;

    if (n < 0)
        ERR(CO_INDEX, "n=%d < 0", n);
    MALLOC(1, &q);
    p = gsl_permutation_alloc(n);
    A = gsl_matrix_alloc(n, n);
    b = gsl_vector_alloc(n);
    x = gsl_vector_alloc(n);
    if (x == NULL)
        ERR(CO_MEMORY, "fail to allocate x");
    q->n = n;
    q->p = p;
    q->A = A;
    q->b = b;
    q->x = x;
    *pq = q;
    return CO_OK;
}

int
lin_solve_fin(T * q)
{
    gsl_permutation_free(q->p);
    gsl_matrix_free(q->A);
    gsl_vector_free(q->b);
    gsl_vector_free(q->x);
    FREE(q);
    return CO_OK;
}

static int
solve0(T * q, const real * b, real * x)
{
    int n, s, i;
    real t;

    n = q->n;
    for (i = 0; i < n; i++) {
        t = b[i];
        gsl_vector_set(q->b, i, t);
    }
    gsl_linalg_LU_decomp(q->A, q->p, &s);
    gsl_linalg_LU_solve(q->A, q->p, q->b, q->x);
    for (i = 0; i < n; i++) {
        t = gsl_vector_get(q->x, i);
        x[i] = t;
    }
    return CO_OK;
}

int
lin_solve_apply(T * q, const real * A, const real * b, real * x)
{
    int n, s, i, j;
    real t;

    n = q->n;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++) {
            t = matrix_get(n, n, i, j, A);
            gsl_matrix_set(q->A, i, j, t);
        }
    return solve0(q, b, x);
}

int
lin_solve_sym(T * q, const real * A, const real * b, real * x)
{
    int n, s, i, j, k;
    real t;

    n = q->n;
    for (k = i = 0; i < n; i++)
        for (j = i; j < n; j++) {
            t = A[k++];
            gsl_matrix_set(q->A, i, j, t);
            gsl_matrix_set(q->A, j, i, t);
        }
    return solve0(q, b, x);
}
