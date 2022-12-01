#include <stdio.h>
#include <stdlib.h>

#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/he/multi.h>
#include <co/memory.h>
#include <co/off.h>
#include <co/ply.h>
#include <co/y.h>

static const char *me = "co.components";
static void
usg(void)
{
    fprintf(stderr, "color connected components\n"
	    "%s < OFF > OFF\n", me);
    exit(1);
}

int
main(int argc, const char **argv)
{
    real *x;
    real *y;
    real *z;
    int *label;
    real *color;
    He *he;
    int nt;
    int nc;
    int i;

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
    nt = he_nt(he);    
    MALLOC(nt, &label);
    MALLOC(nt, &color);
    if (he_multi_label(he, &nc, label) != CO_OK) {
        fprintf(stderr, "%s: he_multi_label failed\n", me);
        exit(1);
    }
    for (i = 0; i < nt; i++)
      color[i] = label[i];
    if (boff_tri_fwrite(he, x, y, z, color, stdout) != CO_OK) {
      fprintf(stderr, "%s: fail to write OFF file\n", me);
      exit(1);
    }
    fprintf(stderr, "%d\n", nc);
    y_fin(he, x, y, z);
    FREE(label);
    FREE(color);
}
