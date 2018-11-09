#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <real.h>
#include <he/bending.h>
#include <he/memory.h>
#include <he/err.h>
#include <he/he.h>
#include <he/y.h>

static He *he;
static real *x, *y, *z, *H;
static Bending *bending;
static BendingParam param;
static int nv;

int main() {
    real eng;
    const char *name = "juelicher";

    y_ini("/dev/stdin", &he, &x, &y, &z);
    nv = he_nv(he);

    param.Kb = 1;
    bending_ini(name, param, he,  &bending);

    eng = bending_energy(bending, he, x, y, z);
    bending_curva_mean_ver(bending, &H);

    MSG("eng: %g", eng);
    MSG("H[0]: %g", H[0]);
    MSG("H[nv-1]: %g", H[nv-1]);

    bending_fin(bending);
    y_fin(he, x, y, z);
}
