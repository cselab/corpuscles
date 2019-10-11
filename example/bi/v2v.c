#include <stdio.h>
#include <real.h>
#include <co/area.h>
#include <co/err.h>
#include <co/he.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/punto.h>
#include <co/y.h>
#include <co/bi.h>

#define FMT CO_REAL_OUT

int
main(int argc, char **argv)
{
    int i, n;
    char *name;
    BI *bi;
    real alpha, *x, *y, *z, *fx, *fy, *fz, *vx, *vy, *vz, *area;
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
    MALLOC3(n, &fx, &fy, &fz);
    MALLOC(n, &area);
    CALLOC3(n, &vx, &vy, &vz);
    he_area_ver(he, x, y, z, area);
    
    for (i = 0; i < n; i++) {
	fx[i] = 1;
	fy[i] = 2;
	fz[i] = 3;
    }
    alpha = -2.0;
    bi_update(bi, he, x, y, z);

    bi_double(bi, he, alpha, x, y, z, fx, fy, fz, /**/ vx, vy, vz);

    const real *q[] = {x, y, z, vx, vy, vz, NULL};
    puts("x y z vx vy vz");
    punto_fwrite(n, q, stdout);

    y_fin(he, x, y, z);
    bi_fin(bi);
    FREE3(fx, fy, fz);
    FREE3(vx, vy, vz);
    FREE(area);
}
