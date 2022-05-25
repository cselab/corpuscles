#include <stdio.h>
#include <stdlib.h>
#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/off.h>
#include <co/y.h>

static const char *me = "rotate/one";
static void
usg(void)
{
    fprintf(stderr, "%s -e int [-c] < OFF\n", me);
    exit(2);
}

int
main(int __UNUSED argc, const char **argv)
{
    const char *arg;
    He *he;
    int Cflag;
    int e;
    int Eflag;
    int f;
    int h;
    int i;
    int j;
    int ne;
    int nt;
    real *color;
    real *x;
    real *y;
    real *z;

    Cflag = 0;
    Eflag = 0;
    while (*++argv != NULL && argv[0][0] == '-')
        switch (argv[0][1]) {
        case 'h':
            usg();
            break;
        case 'c':
            Cflag = 1;
            break;
        case 'e':
            argv++;
            if ((arg = *argv) == NULL) {
                fprintf(stderr, "%s: -e needs an argument\n", me);
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
    if (he_edg_rotate(he, e) != CO_OK)
        ER("he_edg_rotate failed");

    if (Cflag) {
        CALLOC(nt, &color);
        h = he_hdg_edg(he, e);
        f = he_flp(he, h);
        i = he_tri(he, h);
        j = he_tri(he, f);
        color[i] = color[j] = 1;
        boff_tri_fwrite(he, x, y, z, color, stdout);
        FREE(color);
    } else
        boff_fwrite(he, x, y, z, stdout);
    y_fin(he, x, y, z);
}
