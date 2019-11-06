#include <stdio.h>
#include <real.h>
#include <co/area.h>
#include <co/bi.h>
#include <co/bi/cortez_zero.h>
#include <co/err.h>
#include <co/he.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/vec.h>
#include <co/y.h>

#define FMT   CO_REAL_OUT

int
main(int argc, char **argv)
{
    char *name;
    BI *bi;
    BiCortezZero *cortez;

    int i, n;
    real *x, *y, *z, *fx, *fy, *fz, *vx, *vy, *vz, *area;
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
    cortez = bi_pointer(bi);
    n = he_nv(he);
    MALLOC3(n, &fx, &fy, &fz);
    MALLOC(n, &area);
    he_area_ver(he, x, y, z, area);    
    for (i = 0; i < n; i++) {
      fx[i] = area[i];
      fy[i] = 0;
      fz[i] = 0;
    }
    real r[] = {0, 0, -1.01};
    real v[3];
    bi_cortez_zero_velocity(cortez, he, x, y, z, fx, fy, fz, NULL, NULL, NULL, r, v);
    y_fin(he, x, y, z);
    vec_printf(v, FMT);
    FREE3(vx, vy, vz);
    FREE3(fx, fy, fz);
    bi_fin(bi);
}
