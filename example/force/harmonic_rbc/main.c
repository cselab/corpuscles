#include <stdio.h>
#include <tgmath.h>
#include <stdlib.h>

#include <real.h>
#include <he/memory.h>
#include <he/punto.h>
#include <he/f/harmonic_ref.h>
#include <he/he.h>
#include <he/y.h>

static real *x, *y, *z;
static HeFHarmonicRef *force;
static He *he;
static int nv;

static void main0() {
    fprintf(stderr, "eng: %g\n", he_f_harmonic_ref_energy(force, he, x, y, z));
}

int main() {
    real K;
    y_ini("/dev/stdin", &he, &x, &y, &z);
    nv = he_nv(he);
    K = 1;
    he_f_harmonic_ref_ini(K, x, y, z, he, /**/ &force);

    main0();

    he_f_harmonic_ref_fin(force);
    y_fin(he, x, y, z);
    return 0;
}
