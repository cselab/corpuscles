#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <real.h>
#include <co/err.h>
#include <co/green/3wall.h>
#include <co/he.h>
#include <co/macro.h>
#include <co/matrix.h>
#include <co/memory.h>
#include <co/ten.h>
#include <co/tensor3x3.h>
#include <co/y.h>

#define FMT CO_REAL_OUT
static char me[] = "green3wall/velocity";
static void
usg(void)
{
    fprintf(stderr, "%s [-w wall] < OFF\n", me);
    exit(1);
}

int
main(int argc, const char **argv)
{
    He *he;
    Green3Wall *green;
    real *x;
    real *y;
    real *z;
    real w;
    struct Tensor3x3 tensor;
    int n;
    int i;
    int j;
    int k;
    USED(argc);
    w = -2;
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
    err_set_ignore();
    y_inif(stdin, &he, &x, &y, &z);
    n = he_nv(he);
    tensor3x3_ini(n, &tensor);
    green3_wall_ini(he, w, &green);
    green3_wall_apply(green, he, x, y, z, &tensor);

    Ten t;
    for (i = 0; i < n; i++) {
	for (j = 0; j < n; j++) {
	    for (k = 0; k < 3*3; k++)
		printf(FMT " ", matrix_get(n, n, i, j, tensor.d[k]));
	    printf("\n");
	}
    }

    green3_wall_fin(green);
    tensor3x3_fin(&tensor);
    y_fin(he, x, y, z);
}
