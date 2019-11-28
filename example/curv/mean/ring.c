#include <stdlib.h>
#include <stdio.h>

#include <real.h>
#include <co/array.h>
#include <co/curv/mean/ring.h>
#include <co/err.h>
#include <co/he.h>
#include <co/memory.h>
#include <co/off.h>
#include <co/punto.h>
#include <co/y.h>

#define FMT CO_REAL_OUT

int
main(void)
{
    int nv;
    real *mean, *x, *y, *z;
    He *he;
    CurvMeanRing *q;
    int t, nt, i, j, k;
    real *tri;

    y_inif(stdin, &he, &x, &y, &z);
    nv = he_nv(he);
    curv_mean_ring_ini(he, &q);
    MALLOC(nv, &mean);
    curv_mean_ring_apply(q, he, x, y, z, mean);
    const real *queue[] = { x, y, z, mean, NULL };
    puts("x y z mean");
    punto_fwrite(nv, queue, stdout);
    MSG(FMT " " FMT, array_min(nv, mean), array_max(nv, mean));
    curv_mean_ring_fin(q);
    y_fin(he, x, y, z);
    FREE(mean);
}
