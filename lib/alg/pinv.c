#include <stdio.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_blas.h>

#include "real.h"

#include "co/err.h"
#include "co/memory.h"

#include "alg/pinv.h"

#define T AlgPinv

typedef struct T T;
struct T {
    int dim;
    gsl_matrix *U, *V, *Sigma, *Tmp;
    gsl_vector *S, *WORK;
};

int
alg_pinv_ini(int dim, T ** pq)
{
    T *q;

    if (dim <= 0)
        ERR(CO_INDEX, "dim=%d <= 0", dim);

    MALLOC(1, &q);
    q->dim = dim;


    if ((q->U = gsl_matrix_alloc(dim, dim)) == NULL)
        ERR(CO_MEMORY, "allocation failed for q->U");

    if ((q->V = gsl_matrix_alloc(dim, dim)) == NULL)
        ERR(CO_MEMORY, "allocation failed for q->V");

    if ((q->Sigma = gsl_matrix_calloc(dim, dim)) == NULL)   /* calloc */
        ERR(CO_MEMORY, "allocation failed for q->Sigma");

    if ((q->Tmp = gsl_matrix_alloc(dim, dim)) == NULL)
        ERR(CO_MEMORY, "allocation failed for q->Tmp");

    if ((q->S = gsl_vector_alloc(dim)) == NULL)
        ERR(CO_MEMORY, "allocation failed for q->S");

    if ((q->WORK = gsl_vector_alloc(dim)) == NULL)
        ERR(CO_MEMORY, "allocation failed for q->WORK");

    *pq = q;
    return CO_OK;
}

int
alg_pinv_fin(T * q)
{
    gsl_matrix_free(q->U);
    gsl_matrix_free(q->V);
    gsl_matrix_free(q->Sigma);
    gsl_matrix_free(q->Tmp);
    gsl_vector_free(q->S);
    gsl_vector_free(q->WORK);
    FREE(q);
    return CO_OK;
}

static int
mult(gsl_matrix * A, gsl_matrix * B, /**/ gsl_matrix * C)
{
    gsl_matrix_set_zero(C);
    gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1, A, B, 0, C);
    return CO_OK;
}

static real
max(real a, real b)
{
    return a > b ? a : b;
}

int
alg_pinv_apply(T * q, real * A, /**/ real * B)
{
    int i, j, m;
    int dim, err;
    gsl_matrix *U, *V, *Sigma, *Tmp;
    gsl_vector *S, *WORK;
    double x, smin, smax, eps;

    dim = q->dim;
    U = q->U;
    V = q->V;
    S = q->S;
    Sigma = q->Sigma;
    Tmp = q->Tmp;
    WORK = q->WORK;

    for (m = i = 0; i < dim; i++)
        for (j = 0; j < dim; j++)
            gsl_matrix_set(U, i, j, A[m++]);
    if ((err = gsl_linalg_SV_decomp(U, V, S, WORK)) != GSL_SUCCESS) {
        MSG("gsl_linalg_SV_decomp: %s", gsl_strerror(err));
        return 1;
    }

    gsl_vector_minmax(S, &smin, &smax);
    eps = dim * max(fabs(smin), fabs(smax)) * GSL_DBL_EPSILON;

    for (i = 0; i < dim; i++) {
        x = gsl_vector_get(S, i);
        x = (fabs(x) > eps) ? 1 / x : 0;
        gsl_matrix_set(Sigma, i, i, x);
    }

    /* V = V . Sigma . UT */
    mult(V, Sigma, Tmp);
    gsl_matrix_transpose(U);
    mult(Tmp, U, V);
     /**/ for (m = i = 0; i < dim; i++)
        for (j = 0; j < dim; j++)
            B[m++] = gsl_matrix_get(V, i, j);
    return CO_OK;
}
