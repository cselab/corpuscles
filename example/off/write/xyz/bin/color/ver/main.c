#include <stdio.h>

#include <real.h>
#include <he/err.h>
#include <he/off.h>
#include <he/he.h>
#include <he/memory.h>

#include <he/y.h>

static HeOff *off;
static real *x, *y, *z;
static real lo, hi;
static He *he;

int main() {
    lo = -5; hi = 5;
    y_ini("/dev/stdin", &he, &x, &y, &z);
    boff_lh_ver_fwrite(he, x, y, z, lo, hi, x, stdout);
    y_fin(he, x, y, z);
    return 0;
}
