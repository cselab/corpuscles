#include <stdio.h>
#include <stdlib.h>
#include <real.h>
#include <co/argv.h>
#include <co/err.h>
#include <co/he.h>
#include <co/he/invariant.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/off.h>
#include <co/vec.h>
#include <co/y.h>

const char *me = "swap/hdg";
static void
usg()
{
    fprintf(stderr, "%s int int < OFF\n", me);
    exit(2);
}

static int hdg_tri(He *, int, int *, int *);

int
main(int __UNUSED argc, char **argv)
{
    He *he;
    real *x, *y, *z, *tri;
    int i, j, nt, status;

    while (*++argv != NULL && argv[0][0] == '-')
        switch (argv[0][1]) {
        case 'h':
            usg();
            break;
        default:
            fprintf(stderr, "%s: unknown option '%s'\n", me, argv[0]);
            exit(2);
        }
    argv_int(&argv, &i);
    argv_int(&argv, &j);
    y_inif(stdin, &he, &x, &y, &z);
    status = he_swap_hdg(he, i, j);
    if (status != CO_OK)
        ER("he_swap_tri failed");
    status = he_invariant(he);
    if (status != CO_OK)
        ER("he_invariant failed");
    nt = he_nt(he);
    CALLOC(nt, &tri);
    int h, f;

    hdg_tri(he, i, &h, &f);
    tri[h] = tri[f] = 1;

    hdg_tri(he, j, &h, &f);
    tri[h] = tri[f] = 2;
    
    boff_tri_fwrite(he, x, y, z, tri, stdout);
    FREE(tri);
    y_fin(he, x, y, z);
}

static int
hdg_tri(He *he, int h, int *pf, int *ph)
{
    int f;
    f = he_flp(he, h);
    h = he_tri(he, h);
    f = he_tri(he, f);
    *ph = h;
    *pf = f;
    return CO_OK;
}
