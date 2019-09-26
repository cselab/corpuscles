#include <stdio.h>
#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/memory.h>
#include <co/matrix.h>
#include <co/y.h>
#include <fmm3.h>

#define FMT   CO_REAL_OUT

static int tensor_ini(int n, real **xx, real **xy, real **xz, real **yy, real **yz, real **zz) {
    matrix_ini(n, n, xx);
    matrix_ini(n, n, xy);
    matrix_ini(n, n, xz);
    matrix_ini(n, n, yy);
    matrix_ini(n, n, yz);
    matrix_ini(n, n, zz);
    return CO_OK;
}

static int tensor_fin(real *xx, real *xy, real *xz, real *yy, real *yz, real *zz) {
    matrix_fin(xx);
    matrix_fin(xy);
    matrix_fin(xz);
    matrix_fin(yy);
    matrix_fin(yz);
    matrix_fin(zz);
    return CO_OK;
}

int
main()
{
    int i, n;
    FMM3 *q;
    double *x, *y, *z, *fx, *fy, *fz, *vx, *vy, *vz;

    He *he;
    y_inif(stdin, &he, &x, &y, &z);
    n = he_nv(he);
    MALLOC3(n, &fx, &fy, &fz);
    CALLOC3(n, &vx, &vy, &vz);

    fmm3_ini(n, &q);

    for (i = 0; i < n; i++) {
	fx[i] = x[i]*x[i];
	fy[i] = y[i];
	fz[i] = z[i];
    }

    fmm3_single(q, x, y, z, fx, fy, fz, /**/ vx, vy, vz);

    for (i = 0; i < 2; i++)
	MSG(FMT " " FMT " " FMT, vx[i], vy[i], vz[i]);

    y_fin(he, x, y, z);
    FREE3(fx, fy, fz);
    FREE3(vx, vy, vz);
    fmm3_fin(q);
}
