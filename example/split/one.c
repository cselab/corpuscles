#include <stdio.h>
#include <stdlib.h>
#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/he/invariant.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/off.h>
#include <co/y.h>

const char *me = "rotate/one";
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
    const char *arg;
    int e, i, j, h, f, ne, nt, Eflag, status;

    Eflag = 0;
    while (*++argv != NULL && argv[0][0] == '-')
        switch (argv[0][1]) {
        case 'h':
            usg();
            break;
        case 'e':
            argv++;
            if ((arg = *argv) == NULL) {
                fprintf(stderr, "%s: -t needs an argument\n", me);
                exit(2);
            }
            Eflag = 1;
            e = atoi(*argv);
            break;
        default:
            fprintf(stderr, "%s: unknown option '%s'\n", me, argv[0]);
            exit(2);
        }
    y_inif(stdin, &he, &x, &y, &z);
    ne = he_ne(he);
    nt = he_nt(he);
    if (!Eflag) {
        fprintf(stderr, "%s: edge (-e) is not set\n", me);
        exit(2);
    }
    if (e >= ne) {
        fprintf(stderr, "%s: e=%d >= ne=%d\n", me, e, ne);
        exit(2);
    }
    status = he_edg_split(he, e);
    if (status != CO_OK)
        ER("he_edg_split failed");

    status = he_invariant_nxt(he);
    if (status != CO_OK)
	ER("he_invariant_nxt failed");
    
    CALLOC(nt, &color);
    h = he_hdg_edg(he, e);
    f = he_flp(he, h);
    i = he_tri(he, h);
    j = he_tri(he, f);
    color[i] = color[j] = 1;
    boff_tri_fwrite(he, x, y, z, color, stdout);
    FREE(color);
    y_fin(he, x, y, z);
}
