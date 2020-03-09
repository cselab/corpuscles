#include <stdio.h>

#include "real.h"
#include "co/argv.h"
#include "co/memory.h"
#include "co/macro.h"
#include "co/err.h"
#include "co/vec.h"
#include "co/he.h"

#include "co/viscosity/pairwise.h"

#define T ViscosityPairwise

struct T {
    real K;
};

int
viscosity_pairwise_ini(real K, He * he, T ** pq)
{
    T *q;

    USED(he);
    MALLOC(1, &q);
    q->K = K;
    *pq = q;
    return CO_OK;
}

int
viscosity_pairwise_fin(T * q)
{
    FREE(q);
    return CO_OK;
}

int
viscosity_pairwise_force(T * q, He * he, const real * x, const real * y,
                         const real * z, const real * u, const real * v,
                         const real * w, /**/ real * fx, real * fy,
                         real * fz)
{
    int e;
    int i;
    int *ii;
    int j;
    int *jj;
    int n;
    real coef;
    real K;
    real ra[3];
    real rb[3];
    real va[3];
    real vb[3];

    if (he_E(he, &ii, &jj) != CO_OK)
        ERR(CO_NUM, "he_E failed");
    K = q->K;
    n = he_ne(he);
    for (e = 0; e < n; e++) {
        i = ii[e];
        j = jj[e];
        vec_get2(i, j, x, y, z, ra, rb);
        vec_get2(i, j, u, v, w, va, vb);
        vec_sub(rb, /**/ ra);
        vec_sub(vb, /**/ va);
        vec_normalize(ra);
        coef = K * vec_dot(va, ra);
        vec_scalar_append(ra, coef, i, fx, fy, fz);
        vec_scalar_append(ra, -coef, j, fx, fy, fz);
    }

    return CO_OK;
}
