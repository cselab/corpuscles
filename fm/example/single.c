#include <stdio.h>
#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/memory.h>
#include <co/y.h>
#include <fm.h>

#define FMT   CO_REAL_OUT

int
main()
{
    int i, n;
    FM *q;
    real *x, *y, *z, *fx, *fy, *fz, *vx, *vy, *vz;

    He *he;
    y_inif(stdin, &he, &x, &y, &z);
    n = he_nv(he);
    MALLOC3(n, &fx, &fy, &fz);
    CALLOC3(n, &vx, &vy, &vz);

    fm_ini(n, &q);

    for (i = 0; i < n; i++) {
	fx[i] = 1;
	fy[i] = 1;
	fz[i] = 1;
    }

    fm_single(q, x, y, z, fx, fy, fz, /**/ vx, vy, vz);

    for (i = 0; i < 2; i++)
	MSG(FMT " " FMT " " FMT, vx[i], vy[i], vz[i]);

    y_fin(he, x, y, z);
    FREE3(fx, fy, fz);
    FREE3(vx, vy, vz);
    fm_fin(q);
}
