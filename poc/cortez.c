#include <stdio.h>

#include "real.h"
#include "co/memory.h"
#include "co/err.h"
#include "co/he.h"

#include "co/bi/cortez.h"

#define T BiCortez

struct T {
};

int
bi_cortez_ini(real eps, He *he, /**/ T **pq)
{
    T *q;
    MALLOC(1, &q);
    *pq = q;
    return CO_OK;
}

int
bi_cortez_argv(char ***p, He *he, /**/ T **pq)
{
    return CO_OK;
}

int
bi_cortez_fin(T *q)
{
    FREE(q);
    return CO_OK;
}

int
bi_cortez_single(T *q, He *he, real alpha,
		     const real *x, const real *y, const real *z,
		     const real *fx, const real *fy, const real *fz,
		     /*io*/ real *ux, real *uy, real *uz)
{
    return CO_OK;
}

int
bi_cortez_double(T *q, He *he, real alpha,
		 const real *x, const real *y, const real *z,
		 const real *ux, const real *uy, const real *uz,
		 /*io*/real *fx, real *fy, real *fz)
{
    return CO_OK;
}

