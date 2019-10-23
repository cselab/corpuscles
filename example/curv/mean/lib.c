#include <stdlib.h>
#include <stdio.h>

#include <real.h>
#include <co/array.h>
#include <co/curv/mean.h>
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
    real *mean, *area, *x, *y, *z;
    He *he;
    CurvMean *q;
    int t, nt, i, j, k;
    real *tri;
    
    y_inif(stdin, &he, &x, &y, &z);
    nv = he_nv(he);
    curv_mean_ini(he, &q);
    MALLOC(nv, &mean);
    curv_mean_apply(q, he, x, y, z, mean);
    curv_mean_area(q, &area);
    const real *queue[] = {x, y, z, area, mean, NULL};
    
    puts("x y z area mean");
    punto_fwrite(nv, queue, stdout);
    
    /* nt = he_nt(he);
    CALLOC(nt, &tri);
    for (t = 0; t < nt; t++)
    {
	he_tri_ijk(he, t, &i, &j, &k);
	tri[t] = (mean[i] + mean[j] + mean[k])/3;
    }
    boff_tri_fwrite(he, x, y, z, tri, stdout); */
    
    MSG(FMT " " FMT, array_min(nv, mean), array_max(nv, mean));
    
    curv_mean_fin(q);
    y_fin(he, x, y, z);
    FREE(mean);
}
