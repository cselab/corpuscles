#include <tgmath.h>
#include <stdio.h>

#include "real.h"
#include "co/array.h"
#include "co/curv/mean/juelicher.h"
#include "co/dih.h"
#include "co/edg.h"
#include "co/err.h"
#include "co/he.h"
#include "co/memory.h"
#include "co/tri.h"
#include "co/vec.h"

#define T CurvMeanJuelicher

struct T {
    int nv;
    real *area;
};

int
curv_mean_juelicher_ini(He * he, T ** pq)
{
    T *q;
    int status;
    int nv;

    nv = he_nv(he);
    MALLOC(1, &q);
    MALLOC(nv, &q->area);
    q->nv = nv;
    *pq = q;
    return CO_OK;
}

int
curv_mean_juelicher_fin(T * q)
{
    FREE(q->area);
    FREE(q);
    return CO_OK;
}

int
curv_mean_juelicher_apply(T * q, He * he, const real * x, const real * y,
                          const real * z, /**/ real * ans)
{
    real a[3], b[3], c[3], d[3], A, *area;
    int i, j, k, l, t, nt, ne, nv;
    int e;
    real len, theta, coeff;

    nv = q->nv;
    if (nv != he_nv(he))
        ERR(CO_NUM, "nv=%d != he_nv(he)=%d", nv, he_nv(he));
    nt = he_nt(he);
    ne = he_ne(he);
    area = q->area;

    array_zero(nv, area);
    array_zero(nv, ans);

    for (t = 0; t < nt; t++) {
        he_tri_ijk(he, t, &i, &j, &k);
        vec_get3(i, j, k, x, y, z, a, b, c);
        A = tri_area(a, b, c) / 3;
        area[i] += A;
        area[j] += A;
        area[k] += A;
    }

    for (e = 0; e < ne; e++) {
        he_dih_ijkl(he, e, &i, &j, &k, &l);
        vec_get4(i, j, k, l, x, y, z, a, b, c, d);
        len = edg_abs(c, b);
        theta = dih_angle_sup(a, b, c, d);
        coeff = len * theta;
        ans[j] += coeff;
        ans[k] += coeff;
    }
    for (i = 0; i < nv; i++)
        ans[i] /= 4 * area[i];
    return CO_OK;
}

int
curv_mean_juelicher_area(T * q, /**/ real ** ans)
{
    *ans = q->area;
    return CO_OK;
}
