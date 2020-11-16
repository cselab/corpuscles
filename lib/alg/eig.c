#include <stdio.h>
#include <gsl/gsl_eigen.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>

#include "real.h"
#include "co/err.h"

#include "alg/eig.h"

static real
get(const gsl_matrix * m, int i, int j)
{
    return gsl_matrix_get(m, i, j);
}

int
alg_eig_values(const real A[6], /**/ real VAL[3])
{
    enum { XX, XY, XZ, YY, YZ, ZZ };
    enum { YX = XY, ZX = XZ, ZY = YZ };
    enum { X, Y, Z };

    double B[3 * 3];
    gsl_matrix_view m;
    gsl_vector *val;
    gsl_matrix *vec;
    gsl_eigen_symmv_workspace *w;
    int i, j, status;
    real det;

    val = gsl_vector_alloc(3);
    vec = gsl_matrix_alloc(3, 3);
    w = gsl_eigen_symmv_alloc(3);

    i = 0;
    B[i++] = A[XX];
    B[i++] = A[XY];
    B[i++] = A[XZ];
    B[i++] = A[YX];
    B[i++] = A[YY];
    B[i++] = A[YZ];
    B[i++] = A[ZX];
    B[i++] = A[ZY];
    B[i++] = A[ZZ];

    m = gsl_matrix_view_array(B, 3, 3);
    status = gsl_eigen_symmv(&m.matrix, val, vec, w);
    if (status != GSL_SUCCESS)
        ERR(CO_NUM, "gsl_eigen_symmv failed");
    gsl_eigen_symmv_sort(val, vec, GSL_EIGEN_SORT_ABS_ASC);

    i = 0;
    VAL[i] = gsl_vector_get(val, i);
    i++;
    VAL[i] = gsl_vector_get(val, i);
    i++;
    VAL[i] = gsl_vector_get(val, i);

    gsl_vector_free(val);
    gsl_matrix_free(vec);
    gsl_eigen_symmv_free(w);
    return CO_OK;
}

int
alg_eig_vectors(const real A[6], /**/ real V[9])
{
    enum { XX, XY, XZ, YY, YZ, ZZ };
    enum { YX = XY, ZX = XZ, ZY = YZ };
    enum { X, Y, Z };

    double B[3 * 3];
    gsl_matrix_view m;
    gsl_vector *val;
    gsl_matrix *vec;
    gsl_eigen_symmv_workspace *w;
    int i, j, status;
    real det;

    val = gsl_vector_alloc(3);
    vec = gsl_matrix_alloc(3, 3);
    w = gsl_eigen_symmv_alloc(3);

    i = 0;
    B[i++] = A[XX];
    B[i++] = A[XY];
    B[i++] = A[XZ];
    B[i++] = A[YX];
    B[i++] = A[YY];
    B[i++] = A[YZ];
    B[i++] = A[ZX];
    B[i++] = A[ZY];
    B[i++] = A[ZZ];

    m = gsl_matrix_view_array(B, 3, 3);
    status = gsl_eigen_symmv(&m.matrix, val, vec, w);
    if (status != GSL_SUCCESS)
        ERR(CO_NUM, "gsl_eigen_symmv failed");
    gsl_eigen_symmv_sort(val, vec, GSL_EIGEN_SORT_ABS_ASC);

    i = 0;
    V[i++] = get(vec, X, X);
    V[i++] = get(vec, X, Y);
    V[i++] = get(vec, X, Z);
    V[i++] = get(vec, Y, X);
    V[i++] = get(vec, Y, Y);
    V[i++] = get(vec, Y, Z);
    V[i++] = get(vec, Z, X);
    V[i++] = get(vec, Z, Y);
    V[i++] = get(vec, Z, Z);
    det =
        V[0] * (V[4] * V[8] - V[5] * V[7]) - V[1] * (V[3] * V[8] -
                                                     V[5] * V[6]) +
        V[2] * (V[3] * V[7] - V[4] * V[6]);

    i = 0;
    if (det < 0) {
        V[i] = -V[i];
        i += 3;
        V[i] = -V[i];
        i += 3;
        V[i] = -V[i];
    }
    gsl_vector_free(val);
    gsl_matrix_free(vec);
    gsl_eigen_symmv_free(w);
    return CO_OK;
}
