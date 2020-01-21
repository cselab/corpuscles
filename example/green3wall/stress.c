#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <real.h>
#include <co/array.h>
#include <co/err.h>
#include <co/he.h>
#include <co/green/3wall.h>
#include <co/vec.h>
#include <co/y.h>

#define FMT CO_REAL_OUT
static real pi = 3.141592653589793115997964;

static char me[] = "green3wall/stress";
static void
usg()
{
    fprintf(stderr, "%s x0 y0 z0 nx ny nz gx gy gz x y z < OFF\n", me);
    exit(1);
}

int
main(int argc, const char **argv)
{
    He *he;
    Green3Wall *green;
    real *x, *y, *z, a[3], b[3], g[3], n[3];
    real xx, xy, xz, yy, yz, zz;
    real u, v, w, coeff;
    enum {X, Y, Z};

    err_set_ignore();
    while (*++argv != NULL && argv[0][0] == '-')
        switch (argv[0][1]) {
        case 'h':
            usg();
            break;
        default:
            fprintf(stderr, "%s: unknown option '%s'\n", me, argv[0]);
            exit(1);
        }
    y_inif(stdin, &he, &x, &y, &z);

    if (vec_argv(&argv, a) != CO_OK) {
        fprintf(stdin, "%s: fail to read vector\n", me);
        exit(1);
    }
    if (vec_argv(&argv, n) != CO_OK) {
        fprintf(stdin, "%s: fail to read vector\n", me);
        exit(1);
    }
    if (vec_argv(&argv, b) != CO_OK) {
        fprintf(stdin, "%s: fail to read vector\n", me);
        exit(1);
    }
    if (vec_argv(&argv, g) != CO_OK) {
        fprintf(stdin, "%s: fail to read vector\n", me);
        exit(1);
    }
    if (*argv != NULL) {
        fprintf(stdin, "%s: too many arguments\n", me);
        exit(1);
    }
    green3_wall_ini(he, &green);
    green3_wall_t(green, a, n, b, &xx, &xy, &xz, &yy, &yz, &zz);

    v = g[X]*xx + g[Y]*xy + g[Z]*xz;
    u = g[X]*xy + g[Y]*yy + g[Z]*yz;
    w = g[X]*xz + g[Y]*yz + g[Z]*zz;
    coeff = 3/(4*pi);
    printf(FMT " " FMT " " FMT "\n", coeff*v, coeff*u, coeff*w);
    
    green3_wall_fin(green);
    y_fin(he, x, y, z);
}