#include <stdio.h>
#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/memory.h>
#include <co/normal.h>
#include <co/y.h>
#include <fmm3.h>

#define FMT   CO_REAL_OUT

int
main()
{
    int i, n;
    FMM3 *q;
    real *x, *y, *z, *ux, *uy, *uz, *nx, *ny, *nz, *vx, *vy, *vz;

    He *he;
    y_inif(stdin, &he, &x, &y, &z);
    n = he_nv(he);
    MALLOC3(n, &ux, &uy, &uz);
    MALLOC3(n, &nx, &ny, &nz);
    CALLOC3(n, &vx, &vy, &vz);

    fmm3_ini(n, &q);
    //normal_mwa(he, x, y, z, nx, ny, nz);

    for (i = 0; i < n; i++) {
	ux[i] = 1;
	uy[i] = 0;
	uz[i] = 0;
    }
    for (i = 0; i < n; i++) {
	nx[i] = 1;
	ny[i] = 0;
	nz[i] = 0;
    }
    fmm3_double(q, x, y, z, ux, uy, uz, nx, ny, nz, /**/ vx, vy, vz);

    for (i = 0; i < n; i++)
	MSG(FMT " " FMT " " FMT, vx[i], vy[i], vz[i]);

    y_fin(he, x, y, z);
    FREE3(vx, vy, vz);
    FREE3(ux, uy, uz);
    FREE3(nx, ny, nz);    
    fmm3_fin(q);
}
