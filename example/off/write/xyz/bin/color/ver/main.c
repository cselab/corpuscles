#include <stdio.h>

#include <real.h>
#include <co/err.h>
#include <co/off.h>
#include <co/he.h>
#include <co/memory.h>

#include <co/y.h>

static HeOff *off;
static real *x, *y, *z;
static real lo, hi;
static He *he;

int main() {
    lo = -2; hi = 2;
    y_inif(stdin, &he, &x, &y, &z);
    //boff_lh_ver_fwrite(he, x, y, z, lo, hi, x, stdout);
    boff_ver_fwrite(he, x, y, z, z, stdout);
    y_fin(he, x, y, z);
    return 0;
}
