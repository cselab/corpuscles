#include <stdio.h>

#include <real.h>

#include <he/err.h>
#include <he/he.h>
#include <he/macro.h>
#include <he/memory.h>
#include <he/off.h>
#include <he/stretch.h>
#include <he/y.h>

static int nv;
static He *he;
static real *x, *y, *z;
static real *fx, *fy, *fz;
static Stretch *stretch;

int main(__UNUSED int c, char **v) {
    v++;

    y_inif(stdin, &he, &x, &y, &z);
    nv = he_nv(he);
    CALLOC(nv, &fx); CALLOC(nv, &fy); CALLOC(nv, &fz);

    stretch_argv(&v, he, x, y, z, &stretch);
    stretch_force(stretch, x, y, z, fx, fy, fz);

    MSG("n = %d", stretch_n(stretch));

    //boff_ver_fwrite(he, x, y, z, fx, stdout);
    boff_point_fwrite(he, x, y, z, fx, stdout);

    stretch_fin(stretch);

    FREE(fx); FREE(fy); FREE(fz);
    y_fin(he, x, y, z);
    return 0;
}
