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
static real *x, *y, *z, *fx, *fy, *fz;
static Bending *bending;
static BendingParam param;
static int nv;

int main() {
    real eng;
    const char *name = "meyer_xin";

    y_ini("/dev/stdin", &he, &x, &y, &z);
    nv = he_nv(he);

    param.Kb = 1;
    bending_ini(name, param, he,  &bending);

    eng = bending_energy(bending, he, x, y, z);
    bending_laplace_ver(bending, &fx, &fy, &fz);

    MSG("eng: %g", eng);
    MSG("fx[0]: %g", fx[0]);
    MSG("fy[nv-1]: %g", fx[nv-1]);

    bending_fin(bending);
    y_fin(he, x, y, z);
}
