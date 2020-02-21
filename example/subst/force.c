#include <stdio.h>
#include <stdlib.h>
#include <real.h>
#include <co/area.h>
#include <co/array.h>
#include <co/bi.h>
#include <co/err.h>
#include <co/force.h>
#include <co/he.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/punto.h>
#include <co/subst.h>
#include <co/y.h>

#define FMT CO_REAL_OUT
static char me[] = "subst/force";

static void
usg(void)
{
    fprintf(stderr, "%s -b BI -f force [-l lambda] [-n iter_max] [-t tol]\n", me);
    exit(1);
}

int
main(int argc, char **argv)
{
    BI *bi;
    char *name;
    Force *force;
    He *he;
    int i;
    int iter_max;
    int n;
    real alpha;
    real coef;
    real *fx;
    real *fy;
    real *fz;
    real lambda;
    real tol;
    real *ux;
    real *uy;
    real *uz;
    real *vx;
    real *vy;
    real *vz;
    real *x;
    real *y;
    real *z;
    Subst *subst;

    USED(argc);
    if (y_inif(stdin, &he, &x, &y, &z) != CO_OK) {
        fprintf(stderr, "%s: fail to read off file\n", me);
        exit(2);
    }

    argv++;
    bi = NULL;
    force = NULL;
    lambda = 5;
    iter_max = 100;
    tol = 0.01;
    while (*argv != NULL && argv[0][0] == '-')
        switch (argv[0][1]) {
        case 'h':
            usg();
            break;
        case 'n':
            argv++;
            if ((name = *argv++) == NULL) {
                fprintf(stderr, "%s: -n needs an argument\n", me);
                exit(2);
            }
            iter_max = atoi(name);
            break;
        case 'l':
            argv++;
            if ((name = *argv++) == NULL) {
                fprintf(stderr, "%s: -l needs an argument\n", me);
                exit(2);
            }
            lambda = atof(name);
            break;
        case 't':
            argv++;
            if ((name = *argv++) == NULL) {
                fprintf(stderr, "%s: -t needs an argument\n", me);
                exit(2);
            }
            tol = atof(name);
            break;
        case 'b':
            argv++;
            if ((name = *argv++) == NULL) {
                fprintf(stderr, "%s: -b needs an argument\n", me);
                exit(2);
            }
            if (!bi_good(name)) {
                fprintf(stderr, "%s: not a bi algorithm '%s'\n", me,
                        argv[0]);
                fprintf(stderr, "possible values are %s\n", bi_list());
                exit(1);
            }
            bi_argv(name, &argv, he, &bi);
            break;
        case 'f':
            argv++;
            if ((name = *argv++) == NULL) {
                fprintf(stderr, "%s: -f needs an argument\n", me);
                exit(2);
            }
            if (!force_good(name)) {
                fprintf(stderr, "%s: not a force '%s'\n", me, argv[0]);
                fprintf(stderr, "possible values are %s\n", force_list());
                exit(1);
            }
            force_argv(name, &argv, he, &force);
            break;
        default:
            fprintf(stderr, "%s: unknown option '%s'\n", me, *argv);
            exit(1);
            break;
        }
    if (*argv != NULL) {
        fprintf(stderr, "%s unknown argument '%s'\n", me, *argv);
        exit(2);
    }
    if (bi == NULL) {
        fprintf(stderr, "%s: -b is not set\n", me);
        exit(1);
    }
    if (force == NULL) {
        fprintf(stderr, "%s: -f is not set\n", me);
        exit(1);
    }
    n = he_nv(he);
    MALLOC3(n, &ux, &uy, &uz);
    MALLOC3(n, &fx, &fy, &fz);
    CALLOC3(n, &vx, &vy, &vz);

    coef = 2 / (1 + lambda);
    alpha = 2 * (1 - lambda) / (1 + lambda);
    subst_ini(n, alpha, tol, iter_max, &subst);
    array_zero3(n, ux, uy, uz);
    bi_update(bi, he, x, y, z);
    array_zero3(n, fx, fy, fz);
    force_force(force, he, x, y, z, fx, fy, fz);
    bi_single(bi, he, -coef, x, y, z, fx, fy, fz, ux, uy, uz);
    subst_apply(subst, he, bi, x, y, z, ux, uy, uz, /**/ vx, vy, vz);
    MSG("iter: %d", subst_niter(subst));
    const real *q[] = { x, y, z, vx, vy, vz, NULL };
    puts("x y z vx vy vz");
    punto_fwrite(n, q, stdout);

    subst_fin(subst);
    y_fin(he, x, y, z);
    bi_fin(bi);
    FREE3(ux, uy, uz);
    FREE3(vx, vy, vz);
}
