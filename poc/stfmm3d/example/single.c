#include <stdio.h>
#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/memory.h>
#include <co/y.h>
#include <fmm3.h>

#define FMT   CO_REAL_OUT

int
main()
{
    int i, n;
    FMM3 *q;
    real *x, *y, *z, *fx, *fy, *fz, *vx, *vy, *vz;

    He *he;
    y_inif(stdin, &he, &x, &y, &z);
    n = he_nv(he);
    MALLOC3(n, &fx, &fy, &fz);
    CALLOC3(n, &vx, &vy, &vz);

    fmm3_ini(n, &q);

    for (i = 0; i < n; i++) {
	fx[i] = 1;
	fy[i] = 1;
	fz[i] = 1;
    }

    fmm3_single(q, x, y, z, fx, fy, fz, /**/ vx, vy, vz);
    MSG(FMT " " FMT " " FMT, vx[0], vy[0], vz[0]);
    MSG(FMT " " FMT " " FMT, vx[n - 1], vy[n - 1], vz[n - 1]);

    y_fin(he, x, y, z);
    FREE3(fx, fy, fz);
    FREE3(vx, vy, vz);
    fmm3_fin(q);
}
