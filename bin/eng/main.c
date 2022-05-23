#include <stdio.h>
#include <stdlib.h>
#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/macro.h>
#include <co/force.h>
#include <co/util.h>
#include <co/y.h>

#define FMT   CO_REAL_OUT

static const char *me = "co.eng";
static char **argv;

static void
usg(void)
{
    fprintf(stderr, "%s [ARGS..] < IN.off\n", me);
    fprintf(stderr, "%s area 1 10 < 0.off\n", me);
    fprintf(stderr, "%s juelicher_xin 1 0 0 0 < 0.off\n", me);
    exit(2);
}

static int
nxt(const char *b)
{
    return (argv[0] != NULL) && util_eq(argv[0], b);
};

int
main(__UNUSED int c, char **v)
{
    char *name;
    int n, i;
    He *he;
    real *x, *y, *z, e;
    Force *force;

    argv = v;
    argv++;
    if (nxt("-h"))
        usg();
    name = argv[0];
    argv++;
    if (name == NULL || !force_good(name))
        ER("expecting one of the: \n%s", force_list());
    y_inif(stdin, &he, &x, &y, &z);
    force_argv(name, &argv, he, &force);
    e = force_energy(force, he, x, y, z);
    printf(FMT "\n", e);
    force_fin(force);
    y_fin(he, x, y, z);
}
