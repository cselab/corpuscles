#include <stdio.h>
#include <tgmath.h>
#include <real.h>
#include <co/area.h>
#include <co/err.h>
#include <co/he.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/off.h>
#include <co/y.h>
#include <co/bi.h>

#define FMT CO_REAL_OUT

int
main(int argc, char **argv)
{
    int i, n, nt;
    char *name;
    BI *bi;
    real alpha, *x, *y, *z, *ux, *uy, *uz, *vx, *vy, *vz, *area, *tri;
    He *he;
    USED(argc);

    argv++;
    if (argv[0] == NULL)
	ER("needs an argument");
    if (!bi_good(argv[0])) {
	MSG("not a bi algorithm '%s'", argv[0]);
	ER("possible values are '%s'", bi_list());
    }
    y_inif(stdin, &he, &x, &y, &z);
    name = argv[0];
    argv++;
    bi_argv(name, &argv, he, &bi);

    n = he_nv(he);
    nt = he_nt(he);
    MALLOC3(n, &ux, &uy, &uz);
    MALLOC(n, &area);
    CALLOC(nt, &tri);
    CALLOC3(n, &vx, &vy, &vz);
    he_area_ver(he, x, y, z, area);

    for (i = 0; i < n; i++) {
	ux[i] = 1;
	uy[i] = 0;
	uz[i] = 0;
    }
    alpha = -2.0;
    bi_update(bi, he, x, y, z);
    bi_double(bi, he, alpha, x, y, z, ux, uy, uz, /**/ vx, vy, vz);

    int t, j, k;
    real coeff;
    for (t = 0; t < nt; t++) {
	he_tri_ijk(he, t, &i, &j, &k);
	tri[t] += fabs(vx[i] - 1)/3;
	tri[t] += fabs(vx[j] - 1)/3;
	tri[t] += fabs(vx[k] - 1)/3;
    }
    boff_tri_fwrite(he, x, y, z, tri, stdout);

    y_fin(he, x, y, z);
    bi_fin(bi);
    FREE3(ux, uy, uz);
    FREE3(vx, vy, vz);
    FREE(area);
    FREE(tri);
}
