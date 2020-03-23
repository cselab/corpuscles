#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/y.h>

#define FMT  CO_REAL_OUT
#define FMT3 FMT " " FMT " " FMT "\n"
enum { SIZE = 9999 };

static const char *me = "viscosity/pairwise";
static void
usg(void)
{
    fprintf(stderr, "%s < off\n", me);
    exit(2);
}

int
main(int argc, char **argv)
{
    He *he;
    real *x;
    real *y;
    real *z;
    int fd;
    long pid;
    char path[SIZE];
    FILE *f;
    int i;
    int nv;

    err_set_ignore();
    USED(argc);
    while (*++argv != NULL && argv[0][0] == '-')
        switch (argv[0][1]) {
        case 'h':
            usg();
            break;
        default:
            fprintf(stderr, "%s: unknown option '%s'\n", me, argv[0]);
            exit(2);
            break;
        }
    if (argv[0] != NULL) {
        fprintf(stderr, "%s: unknown argument '%s'\n", me, argv[0]);
        exit(2);
    }
    if (y_inif(stdin, &he, &x, &y, &z) != 0) {
        fprintf(stderr, "%s: y_inif failed\n", me);
        exit(2);
    }
    snprintf(path, SIZE, "/tmp/pairwise.%ld", getpid());
    if ((f = fopen(path, "w")) == NULL) {
        fprintf(stderr, "%s: fail to open '%s'\n", me, path);
        exit(2);
    }
    nv = he_nv(he);
    for (i = 0; i < nv; i++)
        fprintf(f, FMT3, x[i], y[i], z[i]);
    fclose(f);
    fprintf(stderr, "%s\n", path);
    y_fin(he, x, y, z);
}
