#include <stdio.h>
#include <tgmath.h>
#include <stdlib.h>
#include <string.h>

#include <real.h>
#include <he/area.h>
#include <he/memory.h>
#include <he/macro.h>
#include <he/y.h>
#include <he/he.h>
#include <he/err.h>
#include <he/vtk.h>

static const real pi = 3.141592653589793115997964;
static real *x, *y, *z, *area;
static int nv, nt;
static He *he;
char **argv;

int main0() {
    he_area_ver(he, x, y, z, /**/ area);

    const real *sc[] = {area, NULL};
    const char *na[] = {"area", NULL};

    vtk_fwrite(he, x, y, z, sc, na, stdout);

    return HE_OK;
}

int main(int __UNUSED argc, char *v[]) {
    argv = v; argv++;

    y_ini("/dev/stdin", /**/ &he, &x, &y, &z);

    nv = he_nv(he);

    MALLOC(nv, &area);

    main0();

    FREE(area);
    y_fin(he, x, y, z);
    return 0;
}
