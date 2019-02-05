#include <stdio.h>

#include <real.h>
#include <he/err.h>
#include <he/off.h>

#include <he/y.h>

static HeOff *off;
static real *x, *y, *z;
static He *he;
static real lo, hi, *area;

int main() {
    lo = 0; hi = 0.1;
    y_ini("/dev/stdin", &he, &x, &y, &z);
    boff_lh_tri_fwrite(he, x, y, z, lo, hi, area, stdout);
    y_fin(he, x, y, z);
    return 0;
}
