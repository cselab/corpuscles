#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/off.h>
#include <co/remesh.h>
#include <co/vec.h>
#include <co/y.h>

static const char *me = "co.sqrt3";
static void
usg(void)
{
    fprintf(stderr, "%s < OFF > OFF\n", me);
    exit(2);
}

int
main(int argc, char **argv)
{
    He *he;
    He *he0;
    real *x;
    real *y;
    real *z;

    while (*++argv != NULL && argv[0][0] == '-')
        switch (argv[0][1]) {
        case 'h':
            usg();
            break;
        default:
            fprintf(stderr, "%s: unknown option '%s'\n", me, argv[0]);
            exit(1);
        }
    err_set_ignore();
    if (y_inif(stdin, &he, &x, &y, &z) != CO_OK) {
        fprintf(stderr, "%s: fail to read OFF file\n", me);
        exit(1);
    }
    if (remesh_sqrt3(he, &x, &y, &z, &he0) != CO_OK) {
        fprintf(stderr, "%s: remesh_sqrt3 failed\n", me);
        exit(1);
    }
    if (off_he_xyz_fwrite(he0, x, y, z, stdout) != CO_OK) {
        fprintf(stderr, "%s: off_he_xyz_fwrite failed\n", me);
        exit(1);
    }
    he_fin(he0);
    y_fin(he, x, y, z);
}
