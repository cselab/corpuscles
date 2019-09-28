#include <stdio.h>
#include <real.h>
#include <co/array.h>
#include <co/err.h>
#include <co/he.h>
#include <co/memory.h>
#include <co/oseen3.h>
#include <co/matrix.h>
#include <co/normal.h>
#include <co/area.h>
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
    real *x, *y, *z, *ux, *uy, *uz, *nx, *ny, *nz, *vx, *vy, *vz, *wx, *wy, *wz;
    real *area;
    Oseen3 *oseen;
    real *Oxx, *Oxy, *Oxz, *Oyy, *Oyz, *Ozz;
    real reg;
    He *he;

    reg = 1e42;
    y_inif(stdin, &he, &x, &y, &z);
    n = he_nv(he);
    oseen3_ini(he, reg, &oseen);
    MALLOC3(n, &ux, &uy, &uz);
    MALLOC3(n, &nx, &ny, &nz);
    CALLOC3(n, &vx, &vy, &vz);
    CALLOC3(n, &wx, &wy, &wz);
    MALLOC(n, &area);
    tensor_ini(n, &Oxx, &Oxy, &Oxz, &Oyy, &Oyz, &Ozz);

    fmm3_ini(n, &q);
    normal_mwa(he, x, y, z, nx, ny, nz);
    he_area_ver(he, x, y, z, area);
    for (i = 0; i < n; i++) {
	ux[i] = 1;
	uy[i] = 0;
	uz[i] = 0;
    }
    oseen3_stresslet(oseen, he, x, y, z, Oxx, Oxy, Oxz, Oyy, Oyz, Ozz);
    oseen3_vector_tensor(n, 1.0, ux, uy, uz, Oxx, Oxy, Oxz, Oyy, Oyz, Ozz, wx, wy, wz);
    array_multiply3(n, area, ux, uy, uz);
    fmm3_double(q, x, y, z, ux, uy, uz, nx, ny, nz, /**/ vx, vy, vz);

    for (i = 0; i < n; i++) {
	printf(FMT " " FMT " " FMT, vx[i], vy[i], vz[i]);
	printf(" ");
	printf(FMT " " FMT " " FMT, wx[i], wy[i], wz[i]);
	printf("\n");
    }
    MSG(FMT " " FMT " " FMT, vx[0], vy[0], vz[0]);

    y_fin(he, x, y, z);
    oseen3_fin(oseen);
    FREE3(wx, wy, wz);
    FREE3(vx, vy, vz);
    FREE3(ux, uy, uz);
    FREE3(nx, ny, nz);
    FREE(area);
    tensor_fin(Oxx, Oxy, Oxz, Oyy, Oyz, Ozz);    
    fmm3_fin(q);
}
