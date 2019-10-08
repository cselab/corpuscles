#include <stdio.h>
#include <real.h>
#include <co/area.h>
#include <co/err.h>
#include <co/he.h>
#include <co/memory.h>
#include <co/normal.h>
#include <co/y.h>
#include <fm.h>

#define FMT   CO_REAL_OUT

int
main()
{
    int i, n;
    FM *q;
    real alpha, *x, *y, *z, *ux, *uy, *uz, *nx, *ny, *nz, *vx, *vy, *vz;
    real *area;

    He *he;
    y_inif(stdin, &he, &x, &y, &z);
    n = he_nv(he);
    MALLOC3(n, &ux, &uy, &uz);
    MALLOC3(n, &nx, &ny, &nz);
    MALLOC(n, &area);
    CALLOC3(n, &vx, &vy, &vz);

    fm_ini(n, &q);
    normal_mwa(he, x, y, z, nx, ny, nz);
    he_area_ver(he, x, y, z, area);

    for (i = 0; i < n; i++) {
	ux[i] = area[i];
	uy[i] = 0;
	uz[i] = 0;
    }
    alpha = 10.0;
    fm_double(q, alpha, x, y, z, ux, uy, uz, nx, ny, nz, /**/ vx, vy, vz);

    for (i = 0; i < n; i++)
	MSG(FMT " " FMT " " FMT, vx[i], vy[i], vz[i]);

    y_fin(he, x, y, z);
    FREE3(vx, vy, vz);
    FREE3(ux, uy, uz);
    FREE3(nx, ny, nz);
    FREE(area);
    fm_fin(q);
}
