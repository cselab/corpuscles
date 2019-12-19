#include <stdio.h>

#include <real.h>
#include <co/area.h>
#include <co/err.h>
#include <co/off.h>
#include <co/he.h>
#include <co/memory.h>

#include <co/y.h>

static Off *off;
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
