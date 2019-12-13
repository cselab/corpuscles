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
usg()
{
    fprintf(stderr, "%s OFF > OFF\n", me);
    exit(2);
}

static int t2edg(He * he, int t, int *, int *, int *);
static int t2ver(He * he, int t, int *, int *, int *);

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

static int
t2edg(He * he, int t, int *i, int *j, int *k)
{
    int h, n, nn;

    h = he_hdg_tri(he, t);
    n = he_nxt(he, h);
    nn = he_nxt(he, n);
    *i = he_edg(he, h);
    *j = he_edg(he, n);
    *k = he_edg(he, nn);

    return CO_OK;
}

static int
t2ver(He * he, int t, int *i, int *j, int *k)
{
    int h, n, nn;

    h = he_hdg_tri(he, t);
    n = he_nxt(he, h);
    nn = he_nxt(he, n);
    *i = he_ver(he, h);
    *j = he_ver(he, n);
    *k = he_ver(he, nn);

    return CO_OK;
}
