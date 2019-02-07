#include <stdio.h>
#include <tgmath.h>
#include <stdlib.h>
#include <string.h>

#include <real.h>
#include <he/area.h>
#include <he/array.h>
#include <he/memory.h>
#include <he/macro.h>
#include <he/y.h>
#include <he/he.h>
#include <he/err.h>
#include <he/vtk.h>

#define FMT  HE_REAL_OUT

static real *x, *y, *z, *area;
static int nv;
static He *he;
char **argv;

int main0() {
    he_area_ver(he, x, y, z, /**/ area);

    const real *sc[] = {area, NULL};
    const char *na[] = {"area", NULL};

    MSG("min: " FMT, array_min(nv, area));
    MSG("max: " FMT, array_max(nv, area));

    vtk_fwrite(he, x, y, z, sc, na, stdout);

    return HE_OK;
}

int main(int __UNUSED argc, char *v[]) {
    argv = v; argv++;

    y_inif(stdin, /**/ &he, &x, &y, &z);

    nv = he_nv(he);
    MALLOC(nv, &area);

    main0();

    FREE(area);
    y_fin(he, x, y, z);
    return 0;
}
