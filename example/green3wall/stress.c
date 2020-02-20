#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <real.h>
#include <co/array.h>
#include <co/err.h>
#include <co/green/3wall.h>
#include <co/he.h>
#include <co/macro.h>
#include <co/ten.h>
#include <co/vec.h>
#include <co/y.h>

#define FMT CO_REAL_OUT
static real pi = 3.141592653589793115997964;

static char me[] = "green3wall/stress";
static void
usg(void)
{
    fprintf(stderr, "%s [-w w] x0 y0 z0 nx ny nz gx gy gz x y z < OFF\n", me);
    exit(1);
}

int
main(int argc, const char **argv)
{
    He *he;
    Green3Wall *green;
    real *x, *y, *z, a[3], b[3], g[3], n[3];
    real u[3];
    real coeff;
    real w;
    Ten ten;
    USED(argc);
    err_set_ignore();
    w = 0;
    while (*++argv != NULL && argv[0][0] == '-')
        switch (argv[0][1]) {
        case 'h':
            usg();
            break;
	case 'w':
	    argv++;
	    if (argv[0] == NULL) {
		fprintf(stderr, "%s: -w needs an argument\n", me);
		exit(1);
	    }
	    w = atof(argv[0]);
	    break;
        default:
            fprintf(stderr, "%s: unknown option '%s'\n", me, argv[0]);
            exit(1);
        }
    y_inif(stdin, &he, &x, &y, &z);
    if (vec_argv(&argv, a) != CO_OK) {
        fprintf(stderr, "%s: fail to read vector\n", me);
        exit(1);
    }
    if (vec_argv(&argv, n) != CO_OK) {
        fprintf(stderr, "%s: fail to read vector\n", me);
        exit(1);
    }
    if (vec_argv(&argv, g) != CO_OK) {
        fprintf(stderr, "%s: fail to read vector\n", me);
        exit(1);
    }
    if (vec_argv(&argv, b) != CO_OK) {
        fprintf(stderr, "%s: fail to read vector\n", me);
        exit(1);
    }
    if (*argv != NULL) {
        fprintf(stderr, "%s: too many arguments\n", me);
        exit(1);
    }
    green3_wall_ini(he, w, &green);
    green3_wall_t(green, a, n, b, &ten);
    ten_vec(&ten, g, u);
    coeff = 3/(4*pi);
    vec_scale(coeff, u);
    vec_printf(u, FMT);
    green3_wall_fin(green);
    y_fin(he, x, y, z);
}
