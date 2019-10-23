#include <tgmath.h>
#include <stdio.h>

#include "real.h"
#include "co/err.h"
#include "co/he.h"
#include "co/vec.h"
#include "co/tri.h"
#include "co/edg.h"
#include "co/memory.h"
#include "co/curv/mean.h"

#define T CurvMean

struct T {
    int nv;
};

int
curv_mean_ini(He * he, T **pq)
{
    T *q;
    int status;
    int nv;

    nv = he_nv(he);
    q->nv = nv;
    *pq = q;
    return CO_OK;
}

int
curv_mean_fin(T * q)
{
    FREE(q);
    return CO_OK;
}

int
curv_mean_apply(T * q, He * he, const real * x, const real * y, const real * z,
	/**/ real * ans)
{
    int nv;
    nv = q->nv;
    if (nv != he_nv(he))
	ERR(CO_NUM, "nv=%d != he_nv(he)=%d", nv, he_nv(he));
    return CO_OK;
}
