#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/remesh.h>
#include <co/off.h>
#include <co/vec.h>
#include <co/edg.h>
#include <co/tri.h>
#include <co/y.h>

static const char *me = "upsample";
static void
usg(void)
{
    fprintf(stderr, "%s -0 A.off -1 B.off > OFF\n", me);
    exit(2);
}

struct {
    int n;
    int i;
    int j;
    int *q;
} Queue;
static int queue_ini(int);
static int queue_fin();
static int queue_enqueue(int);
static int queue_dequeue(int *);
static int queue_empty(int *);

int
main(int argc, char **argv)
{
    USED(argc);
    He *he0;
    He *he1;
    real *x0;
    real *x1;
    real *y0;
    real *y1;
    real *z0;
    real *z1;
    int i;
    int n0;
    int n1;

    he0 = he1 = NULL;
    while (*++argv != NULL && argv[0][0] == '-')
        switch (argv[0][1]) {
        case 'h':
            usg();
            break;
        case 'l':
            argv++;
            break;
        case '0':
            argv++;
            if (*argv == NULL) {
                fprintf(stderr, "%s: -0 needs an agument\n", me);
                exit(2);
            }
            y_ini(*argv, &he0, &x0, &y0, &z0);
            break;
        case '1':
            argv++;
            if (*argv == NULL) {
                fprintf(stderr, "%s: -1 needs an agument\n", me);
                exit(2);
            }
            y_ini(*argv, &he1, &x1, &y1, &z1);
            break;
        default:
            fprintf(stderr, "%s: unknown option '%s'\n", me, argv[0]);
            exit(2);
            break;
        }
    if (he0 == NULL) {
        fprintf(stderr, "%s: -0 is not given\n", me);
        exit(2);
    }
    if (he1 == NULL) {
        fprintf(stderr, "%s: -1 is not given\n", me);
        exit(2);
    }

    n0 = he_nv(he0);
    n1 = he_nv(he0);
    if (n1 < n0) {
        fprintf(stderr, "%s: n1=%d < n0=%d\n", me, n1, n0);
        exit(2);
    }
    for (i = 0; i < n0; i++) {
        x0[i] = x1[i];
        y0[i] = y1[i];
        z0[i] = z1[i];
    }
    if (off_he_xyz_fwrite(he0, x0, y0, z0, stdout) != CO_OK) {
        fprintf(stderr, "%s: off_he_xyz_fwrite failed\n", me);
        exit(2);

    }
    y_fin(he0, x0, y0, z0);
    y_fin(he1, x1, y1, z1);
}
