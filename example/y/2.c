#include <stdio.h>
#include <stdlib.h>
#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/he/multi.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/off.h>
#include <co/y.h>

static const char *me = "y/2";
static void
usg()
{
    fprintf(stderr, "%s OFF OFF > OFF\n", me);
    exit(2);
}

int
main(int argc, char **argv)
{
    He *he;
    real *x, *y, *z, *color;
    char *a, *b;
    int *label, nt, nc, i;
    FILE *fa, *fb;

    USED(argc);
    while (*++argv != NULL && argv[0][0] == '-')
        switch (argv[0][1]) {
        case 'h':
            usg();
            break;
        default:
            fprintf(stderr, "%s: unknown option '%s'\n", me, argv[0]);
            exit(2);
        }
    if ((a = *argv++) == NULL) {
        fprintf(stderr, "%s: needs OFF\n", me);
        exit(2);
    }
    if ((b = *argv++) == NULL) {
        fprintf(stderr, "%s: needs OFF\n", me);
        exit(2);
    }

    if ((fa = fopen(a, "r")) == NULL) {
        fprintf(stderr, "%s: fail to open '%s'\n", me, a);
        exit(2);
    }
    if ((fb = fopen(b, "r")) == NULL) {
        fprintf(stderr, "%s: fail to open '%s'\n", me, b);
        exit(2);
    }
    y_inif2(fa, fb, &he, &x, &y, &z);
    nt = he_nt(he);
    MALLOC(nt, &label);
    MALLOC(nt, &color);
    he_multi_label(he, &nc, label);
    MSG("nc: %d", nc);
    for (i = 0; i < nt; i++)
      color[i] = label[i];
    boff_tri_fwrite(he, x, y, z, color, stdout);
    FREE(label);
    FREE(color);
    y_fin(he, x, y, z);
    fclose(fa);
    fclose(fb);
}
