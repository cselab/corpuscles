#include <stdio.h>
#include <tgmath.h>
#include <stdlib.h>
#include <string.h>

#include <real.h>
#include <he/memory.h>
#include <he/punto.h>
#include <he/macro.h>
#include <he/y.h>
#include <he/he.h>
#include <he/err.h>
#include <he/strain.h>
#include <he/f/strain.h>

static const real pi = 3.141592653589793115997964;
static char **argv;
static real *xx, *yy, *zz, *fm, *fx, *fy, *fz;
static int nv, nt;
static HeFStrain *strain;
static He *he;
static real h = 1e-6;


static real energy() {
    return he_f_strain_energy(strain, he, xx, yy, zz);
}

int main0() {
    int i;
    real *eng, e, eh, tmp;
    he_f_strain_argv(&argv, he, &strain);
    
    he_f_strain_force(strain, he, xx, yy, zz, /**/ fx, fy, fz);
    e = he_f_strain_energy(strain, he, xx, yy, zz);
    he_f_strain_energy_ver(strain, &eng);
    real *queue[] = {xx, yy, zz, fx, fy, fz, eng, NULL};
    puts("x y z fx fy fz eng");
    punto_fwrite(nv, queue, stdout);
    he_f_strain_fin(strain);

    return HE_OK;
}

int main(int __UNUSED argc, char *v[]) {
    argv = v; argv++;
    
    y_ini("/dev/stdin", /**/ &he, &xx, &yy, &zz);
    nv = he_nv(he);

    CALLOC(nv, &fm); CALLOC(nv, &fx);  CALLOC(nv, &fy); CALLOC(nv, &fz);

    main0();

    FREE(fm); FREE(fx); FREE(fy); FREE(fz);

    y_fin(he, xx, yy, zz);
    return 0;
}
