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
    char path_in[SIZE];
    char path_out[SIZE];
    char command[SIZE];
    const char *cmd;
    FILE *f;
    int i;
    int nv;

    err_set_ignore();
    USED(argc);
    cmd = NULL;
    while (*++argv != NULL && argv[0][0] == '-')
        switch (argv[0][1]) {
        case 'h':
            usg();
            break;
        case 'c':
            argv++;
            if (argv[0] == NULL) {
                fprintf(stderr, "%s: -c needs an argumen'%s'\n", me);
                exit(2);
            }
            cmd = argv[0];
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
    if (cmd == NULL) {
        fprintf(stderr, "%s: option -c is not set\n", me);
        exit(2);
    }

    snprintf(path_in, SIZE, "/tmp/pairwise.in.%ld", getpid());
    snprintf(path_out, SIZE, "/tmp/pairwise.out.%ld", getpid());

    if ((f = fopen(path_in, "w")) == NULL) {
        fprintf(stderr, "%s: fail to open '%s'\n", me, path_in);
        exit(2);
    }
    nv = he_nv(he);
    for (i = 0; i < nv; i++)
        if (fprintf(f, FMT3, x[i], y[i], z[i]) < 0) {
            fprintf(stderr, "%s: fprintf(3) failed for '%s'\n", me,
                    path_in);
            exit(2);
        }

    if (fclose(f) != 0) {
        fprintf(stderr, "%s: fclose(3) failed for '%s'\n", me, path_in);
        exit(2);
    }

    snprintf(command, SIZE, "<%s %s >%s", path_in, cmd, path_out);
    if (system(command) != 0) {
        fprintf(stderr, "%s: command '%s' failed\n", me, command);
        exit(2);
    }

    fprintf(stderr, "%s\n", command);
    y_fin(he, x, y, z);
}
