#include <stdio.h>
#include <real.h>
#include <co/he.h>
#include <co/memory.h>
#include <co/y.h>
#include <fmm3.h>

int
main()
{
    int n;
    FMM3 *q;
    double *x, *y, *z, *fx, *fy, *fz, *vx, *vy, *vz;

    He *he;
    y_inif(stdin, &he, &x, &y, &z);
    n = he_nv(he);
    CALLOC3(n, &fx, &fy, &fz);
    CALLOC3(n, &vx, &vy, &vz);

    fmm3_ini(n, &q);
    fmm3_single(q, x, y, z, fx, fy, fz, vx, vy, vz);

    y_fin(he, x, y, z);
    FREE3(fx, fy, fz);
    FREE3(vx, vy, vz);
    fmm3_fin(q);
}
