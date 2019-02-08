#include <stdio.h>

#include <real.h>
#include <he/area.h>
#include <he/err.h>
#include <he/off.h>
#include <he/he.h>
#include <he/memory.h>

#include <he/y.h>

static HeOff *off;
static int nt;
static real *x, *y, *z;
static He *he;
static real lo, hi, *a;

int main() {
    lo = 0; hi = 0.005;
    y_inif(stdin, &he, &x, &y, &z);

    nt = he_nt(he);
    CALLOC(nt, &a);

    he_area_tri(he, x, y, z, a);
    //boff_lh_tri_fwrite(he, x, y, z, lo, hi, a, stdout);
    boff_tri_fwrite(he, x, y, z, a, stdout);
    y_fin(he, x, y, z);

    FREE(a);
    return 0;
}
