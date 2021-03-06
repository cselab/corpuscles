#include <stdio.h>
#include <stdlib.h>
#include <real.h>
#include <co/argv.h>
#include <co/array.h>
#include <co/ten.h>
#include <co/err.h>
#include <co/he.h>
#include <co/green/3wall2.h>
#include <co/vec.h>
#include <co/macro.h>
#include <co/y.h>

#define FMT CO_REAL_OUT
static real pi = 3.141592653589793115997964;

static char me[] = "green3wall/velocity2";
static void
usg(void)
{
    fprintf(stderr, "%s [-w wall] x0 y0 z0 gx gy gz x y z < OFF\n", me);
    exit(1);
}

int
main(int argc, const char **argv)
{
    He *he;
    Green3Wall2 *green;
    real *x, *y, *z, a[3], b[3], g[3];
    real u[3];
    real w;
    Ten ten;
    enum {X, Y, Z};
    USED(argc);

    w = 0;
    err_set_ignore();
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
    if (vec_argv(&argv, b) != CO_OK) {
        fprintf(stderr, "%s: fail to read vector\n", me);
        exit(1);
    }
    if (vec_argv(&argv, g) != CO_OK) {
        fprintf(stderr, "%s: fail to read vector\n", me);
        exit(1);
    }
    if (vec_argv(&argv, a) != CO_OK) {
        fprintf(stderr, "%s: fail to read vector\n", me);
        exit(1);
    }
    if (*argv != NULL) {
        fprintf(stderr, "%s: too many arguments\n", me);
        exit(1);
    }
    green3_wall2_ini(he, w, &green);
    green3_wall2_s(green, a, b, &ten);
    ten_vec(&ten, g, u);
    vec_scale(1/(8*pi), u);
    vec_printf(u, FMT);
    green3_wall2_fin(green);
    y_fin(he, x, y, z);
}
