#include <stdio.h>
#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/memory.h>
#include <co/oseen3.h>
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
    int i, j, n;
    FMM3 *q;
    real *x, *y, *z, *fx, *fy, *fz, *vx, *vy, *vz, *wx, *wy, *wz;
    static real *Oxx, *Oxy, *Oxz, *Oyy, *Oyz, *Ozz;
    real reg;
    He *he;
    Oseen3 *oseen;

    reg = 1e42;
    y_inif(stdin, &he, &x, &y, &z);
    n = he_nv(he);
    oseen3_ini(he, reg, &oseen);

    tensor_ini(n, &Oxx, &Oxy, &Oxz, &Oyy, &Oyz, &Ozz);
    MALLOC3(n, &fx, &fy, &fz);
    CALLOC3(n, &vx, &vy, &vz);
    CALLOC3(n, &wx, &wy, &wz);    
    fmm3_ini(n, &q);

    for (i = 0; i < n; i++) {
	fx[i] = x[i]*x[i];
	fy[i] = y[i];
	fz[i] = z[i];
    }

    oseen3_apply(oseen, he, x, y, z, Oxx, Oxy, Oxz, Oyy, Oyz, Ozz);
    oseen3_vector_tensor(n, 1.0, fx, fy, fz, Oxx, Oxy, Oxz, Oyy, Oyz, Ozz, wx, wy, wz);
    fmm3_single(q, x, y, z, fx, fy, fz, /**/ vx, vy, vz);

    for (i = 0; i < n; i++) {
	printf(FMT " " FMT " " FMT, vx[i], vy[i], vz[i]);
	printf(" ");
	printf(FMT " " FMT " " FMT, wx[i], wy[i], wz[i]);
	printf("\n");
    }

    oseen3_fin(oseen);
    y_fin(he, x, y, z);
    FREE3(fx, fy, fz);
    FREE3(vx, vy, vz);
    FREE3(wx, wy, wz);
    tensor_fin(Oxx, Oxy, Oxz, Oyy, Oyz, Ozz);
    fmm3_fin(q);
}
