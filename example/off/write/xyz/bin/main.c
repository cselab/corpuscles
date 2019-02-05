#include <stdio.h>

#include <real.h>
#include <he/err.h>
#include <he/off.h>

#include <he/y.h>

static HeOff *off;
static real *x, *y, *z;
static He *he;

int main() {
    y_ini("/dev/stdin", &he, &x, &y, &z);
    boff_fwrite(he, x, y, z, stdout);
    y_fin(he, x, y, z);
    return 0;
}
