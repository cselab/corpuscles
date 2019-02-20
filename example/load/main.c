#include <stdio.h>

#include <real.h>

#include <he/array.h>
#include <he/err.h>
#include <he/he.h>
#include <he/macro.h>
#include <he/memory.h>
#include <he/off.h>
#include <he/stretch.h>
#include <he/force.h>
#include <he/argv.h>
#include <he/y.h>

static int nv;
static He *he;
static real *x, *y, *z;
static real *fx, *fy, *fz;

static Stretch *stretch;

static char name[1024];
static Force *force;

int main0(void) {
    int i, n;
    n = 10;
    for (i = 0; i < n; i++) {
        stretch_force(stretch, x, y, z, fx, fy, fz);
        force_force(force, he, x, y, z, fx, fy, fz);
        array_zero(nv, fx);
        array_zero(nv, fy);
        array_zero(nv, fz);
    }
}

int main(__UNUSED int c, char **v) {
    v++;

    y_inif(stdin, &he, &x, &y, &z);
    nv = he_nv(he);
    CALLOC(nv, &fx); CALLOC(nv, &fy); CALLOC(nv, &fz);

    argv_str(&v, name);
    force_argv(name, &v, he,  &force);
    stretch_argv(&v, he, x, y, z, &stretch);

    main0();
    boff_point_fwrite(he, x, y, z, fx, stdout);

    force_fin(force);
    stretch_fin(stretch);

    FREE(fx); FREE(fy); FREE(fz);
    y_fin(he, x, y, z);
    return 0;
}
