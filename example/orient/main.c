#include <stdio.h>
#include <tgmath.h>

#include <real.h>
#include <he/memory.h>
#include <he/off.h>
#include <he/orient.h>
#include <he/he.h>
#include <he/y.h>

static int nv;
static real *x, *y, *z;
static He *he;
static Orient *orient;

static void main0() {
    orient_apply(orient, x, y, z);
    off_he_xyz_fwrite(he, x, y, z, stdout);
}

int main() {
    y_ini("/dev/stdin", &he, &x, &y, &z);
    nv = he_nv(he);
    orient_ini(he, &orient);
    main0();
    orient_fin(orient);

    y_fin(he, x, y, z);
    return 0;
}
