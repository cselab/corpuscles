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

static const char *me = "loop";
static void
usg(void)
{
    fprintf(stderr, "%s OFF > OFF\n", me);
    exit(2);
}

int
main(int argc, char **argv)
{
    USED(argc);
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
            ER("unknown option '%s'", argv[0]);
        }
    y_inif(stdin, &he, &x, &y, &z);
    remesh_loop(he, &x, &y, &z, &he0);
    if (off_he_xyz_fwrite(he0, x, y, z, stdout) != CO_OK)
        ER("off_he_xyz_fwrite failed");
    he_fin(he);
    y_fin(he0, x, y, z);
}
