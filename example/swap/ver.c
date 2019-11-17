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

const char *me = "swap/ver";
static void
usg()
{
    fprintf(stderr, "%s: -i [int] -j [int] < OFF\n", me);
    exit(2);
}

int
main(int __UNUSED argc, char **argv)
{
    He *he;
    real *x, *y, *z, *ver, *tri;
    int i, j, k, t, nv, nt, status;

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
    status = he_swap_ver(he, i, j);
    if (status != CO_OK)
        ER("he_edg_split failed");
    status = he_invariant(he);
    if (status != CO_OK)
        ER("he_invariant failed");    
    nv = he_nv(he);
    nt = he_nt(he);
    CALLOC(nv, &ver);
    CALLOC(nt, &tri);
    ver[i] = 3;
    ver[j] = 6;
    vec_swap(i, j, x, y, z);
    for (t = 0; t < nt; t++) {
      he_tri_ijk(he, t, &i, &j, &k);
      tri[t] += ver[i]/3;
      tri[t] += ver[j]/3;
      tri[t] += ver[k]/3;
    }
    boff_tri_fwrite(he, x, y, z, tri, stdout);
    FREE(tri);
    FREE(ver);
    y_fin(he, x, y, z);
}
