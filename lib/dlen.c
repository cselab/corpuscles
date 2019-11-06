#include <math.h>
#include <stdio.h>
#include "real.h"
#include "co/dedg2.h"
#include "co/dlen.h"
#include "co/err.h"
#include "co/matrix.h"
#include "co/skel.h"
#include "co/vec2.h"

int
dlen_ver(Skel * skel, const real * x, const real * y, real * u, real * v)
{
#define SET(i, j, a) matrix_set(nv, nv, i, j, a[X], u), matrix_set(nv, nv, i, j, a[Y], v)
    enum { X, Y };
    int e, i, j, ne, nv;
    real a[2], b[2], da[2], db[2];

    ne = skel_ne(skel);
    nv = skel_nv(skel);
    matrix_zero(nv, nv, u);
    matrix_zero(nv, nv, v);
    for (e = 0; e < ne; e++) {
        skel_edg_ij(skel, e, &i, &j);
        vec2_get(i, x, y, a);
        vec2_get(j, x, y, b);
        dedg2_abs(a, b, da, db);
        vec2_scale(0.5, da);
        vec2_scale(0.5, db);
        SET(i, i, da);
        SET(j, j, db);
        SET(i, j, db);
        SET(j, i, da);
    }
    return CO_OK;
}
