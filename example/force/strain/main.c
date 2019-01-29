#include <stdio.h>
#include <tgmath.h>
#include <stdlib.h>
#include <string.h>

#include <real.h>
#include <he/area.h>
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
static real *x, *y, *z, *fm, *fx, *fy, *fz, *area, *area0;
static real *x0, *y0, *z0;
static int nv, nt;
static HeFStrain *strain;
static He *he, *he0;
static real h = 1e-6;


static real energy() { return he_f_strain_energy(strain, he, x, y, z); }

int main0() {
    int i;
    real *eng, e, eh, tmp;
    he_f_strain_argv(&argv, he, &strain);

    he_f_strain_force(strain, he, x, y, z, /**/ fx, fy, fz);
    e = he_f_strain_energy(strain, he, x, y, z);
    he_f_strain_energy_ver(strain, &eng);

    he_area_ver(he, x, y, z, /**/ area);
    he_area_ver(he0, x0, y0, z0, /**/ area0);

    real *queue[] = {x, y, z, fx, fy, fz, eng, area, area0, NULL};
    puts("x y z fx fy fz eng area area0");
    punto_fwrite(nv, queue, stdout);
    he_f_strain_fin(strain);

    return HE_OK;
}

int main(int __UNUSED argc, char *v[]) {
    argv = v; argv++;

    y_ini("/dev/stdin", /**/ &he, &x, &y, &z);
    y_ini(*argv, /**/ &he0, &x0, &y0, &z0);

    nv = he_nv(he);

    CALLOC(nv, &fm); CALLOC(nv, &fx);  CALLOC(nv, &fy); CALLOC(nv, &fz);
    MALLOC(nv, &area); MALLOC(nv, &area0);

    main0();

    FREE(fm); FREE(fx); FREE(fy); FREE(fz);
    FREE(area); FREE(area0);

    y_fin(he, x, y, z);
    y_fin(he0, x0, y0, z0);

    return 0;
}
