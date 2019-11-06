#include <stdio.h>
#include <math.h>

#include <real.h>
#include <co/memory.h>
#include <co/punto.h>
#include <co/f/dvolume.h>
#include <co/he.h>
#include <co/y.h>

static int nv;
static real *fx, *fy, *fz;
static real *x, *y, *z;
static He *he;
static HeFDvolume *dvolume;

static void main0() {
    const real *queue[] = {z, y, z, fx, fy, fz, NULL};
    fprintf(stderr, "eng: %g\n",
            he_f_dvolume_energy(dvolume, he, x, y, z));
    he_f_dvolume_force(dvolume, he, x, y, z,   fx, fy, fz);

    puts("x y z fx fy fz");
    punto_fwrite(nv, queue, stdout);
}

int main() {
    y_inif(stdin, &he, &x, &y, &z);
    nv = he_nv(he);

    he_f_dvolume_ini(he, &dvolume);

    CALLOC(nv, &fx); CALLOC(nv, &fy); CALLOC(nv, &fz);
    main0();

    FREE(fx); FREE(fy); FREE(fz);
    he_f_dvolume_fin(dvolume);

    y_fin(he, x, y, z);
    return 0;
}
