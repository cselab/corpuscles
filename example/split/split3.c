#include <stdio.h>
#include <stdlib.h>
#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/he/invariant.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/off.h>
#include <co/vec.h>
#include <co/tri.h>
#include <co/y.h>

static const char *me = "split/split3";
static void
usg()
{
    fprintf(stderr, "%s: [tirangle number] < OFF\n", me);
    exit(2);
}

int
main(int __UNUSED argc, const char **argv)
{
    He *he;
    real *x, *y, *z, *color;
    real a[3], b[3], c[3], d[3];
    const char *arg;
    int t, i, j, k, nt, nv, Tflag, status;

    Tflag = 0;
    while (*++argv != NULL && argv[0][0] == '-')
        switch (argv[0][1]) {
        case 'h':
            usg();
            break;
        case 't':
            argv++;
            if ((arg = *argv) == NULL) {
                fprintf(stderr, "%s: -t needs an argument\n", me);
                exit(2);
            }
            Tflag = 1;
            t = atoi(*argv);
            break;
        default:
            fprintf(stderr, "%s: unknown option '%s'\n", me, argv[0]);
            exit(2);
        }
    y_inif(stdin, &he, &x, &y, &z);
    nt = he_nt(he);
    if (!Tflag) {
        fprintf(stderr, "%s: triangle (-t) is not set\n", me);
        exit(2);
    }
    if (t >= nt) {
        fprintf(stderr, "%s: t=%d >= nt=%d\n", me, t, nt);
        exit(2);
    }
    MSG("he_nv: %d", he_nv(he));
    MSG("he_nt: %d", he_nt(he));
    he_tri_ijk(he, t, &i, &j, &k);

    he_tri_ijk(he, t, &i, &j, &k);
    vec_get3(i, j, k, x, y, z, a, b, c);
    tri_center(a, b, c, d);
    status = he_tri_split3(he, t);
    if (status != CO_OK)
        ER("he_tri_split failed");
    status = he_invariant(he);
    if (status != CO_OK)
        ER("he_invariant failed");
    nv = he_nv(he);
    nt = he_nt(he);
    REALLOC(nv, &x);
    REALLOC(nv, &y);
    REALLOC(nv, &z);
    CALLOC(nt, &color);
    for (i = nt - 2; i < nt; i++)
        color[i] = 1;
    color[t] = 1;
    vec_set(d, nv - 1, x, y, z);
    MSG("nv: %d", nv);
    MSG("nt: %d", nt);
    boff_tri_fwrite(he, x, y, z, color, stdout);
    FREE(color);
    y_fin(he, x, y, z);
}
