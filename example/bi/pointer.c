#include <stdio.h>
#include <real.h>
#include <co/area.h>
#include <co/bi.h>
#include <co/bi/cortez_zero.h>
#include <co/err.h>
#include <co/he.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/normal.h>
#include <co/punto.h>
#include <co/y.h>

#define FMT   CO_REAL_OUT

int
main(int argc, char **argv)
{
    char *name;
    BI *bi;
    BiCortezZero *cortez;

    int i, n;
    real *x, *y, *z, *ux, *uy, *uz, *nx, *vx, *vy, *vz;
    real alpha;
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
    MALLOC3(n, &ux, &uy, &uz);
    CALLOC3(n, &vx, &vy, &vz);
    for (i = 0; i < n; i++) {
        ux[i] = x[i];
        uy[i] = y[i];
        uz[i] = z[i];
    }
    alpha = 1.0;
    bi_cortez_zero_update(cortez, he, x, y, z);
    bi_cortez_zero_double(cortez, he, alpha, x, y, z, ux, uy, uz, /**/ vx,
                          vy, vz);
    const real *q[] = { x, y, z, vx, vy, vz, NULL };
    puts("x y z vx vy vz");
    punto_fwrite(n, q, stdout);
    y_fin(he, x, y, z);
    FREE3(vx, vy, vz);
    FREE3(ux, uy, uz);
    bi_fin(bi);
}
