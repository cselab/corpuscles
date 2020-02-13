#include <stdio.h>
#include <real.h>
#include <co/array.h>
#include <co/area.h>
#include <co/bi.h>
#include <co/err.h>
#include <co/he.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/punto.h>
#include <co/subst.h>
#include <co/y.h>

#define FMT CO_REAL_OUT

int
main(int argc, char **argv)
{
    int i;
    int n;
    char *name;
    BI *bi;
    real *x;
    real *y;
    real *z;
    real *ux;
    real *uy;
    real *uz;
    real *vx;
    real *vy;
    real *vz;
    real *area;
    He *he;
    static Subst *subst;

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
    MALLOC3(n, &ux, &uy, &uz);
    MALLOC(n, &area);
    CALLOC3(n, &vx, &vy, &vz);
    he_area_ver(he, x, y, z, area);

    real alpha;
    real lambda;
    real tol;
    real coef;
    int iter_max;
    lambda = 0.2;
    tol = 0.01;
    coef =  2 / (1 + lambda);
    alpha = 2 * (1 - lambda) / (1 + lambda);
    iter_max = 100;    
    subst_ini(n, alpha, tol, iter_max, &subst);
    for (i = 0; i < n; i++) {
        ux[i] = 1;
        uy[i] = 2;
        uz[i] = 3;
    }
    array_scale3(n, coef, ux, uy, uz);
    
    bi_update(bi, he, x, y, z);
    subst_apply(subst, he, bi, x, y, z, ux, uy, uz, /**/ vx, vy, vz);
    const real *q[] = { x, y, z, vx, vy, vz, NULL };
    puts("x y z vx vy vz");
    punto_fwrite(n, q, stdout);

    subst_fin(subst);
    y_fin(he, x, y, z);
    bi_fin(bi);
    FREE3(ux, uy, uz);
    FREE3(vx, vy, vz);
    FREE(area);
}
