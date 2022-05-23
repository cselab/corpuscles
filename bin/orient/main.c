#include <stdio.h>
#include <stdlib.h>

#include <real.h>
#include <co/memory.h>
#include <co/off.h>
#include <co/orient.h>
#include <co/he.h>
#include <co/err.h>
#include <co/util.h>
#include <co/macro.h>
#include <co/y.h>

static int nv;
static real *x, *y, *z;
static He *he;
static Orient *orient;

static const char *me = "co.orient";

static void
usg(void)
{
    fprintf(stderr, "%s < IN.off > OUT.off\n", me);
    fprintf(stderr, "%s   IN.off > OUT.off\n", me);
    fputs("put mesh in a center of mass frame, orient axis\n", stderr);
    exit(2);
}

static int
eq(const char **a, const char *b)
{
    return (*a != NULL) && util_eq(*a, b);
};

static void
main0()
{
    real *queue[] = { x, y, z, NULL };
    orient_apply(orient, x, y, z);
    off_he_xyz_fwrite(he, x, y, z, stdout);
}

int
main(__UNUSED int c, const char **v)
{
    const char *f;
    const char *stream = "/dev/stdin";

    v++;
    if (eq(v, "-h"))
        usg();
    f = (*v == NULL) ? stream : *v;

    if (y_ini(f, &he, &x, &y, &z) != CO_OK) {
        fprintf(stderr, "%s: fail to open '%s'", me, f);
        exit(1);
    }

    nv = he_nv(he);
    orient_ini(he, &orient);
    main0();
    orient_fin(orient);

    y_fin(he, x, y, z);
    return 0;
}
