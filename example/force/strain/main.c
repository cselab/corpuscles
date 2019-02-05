#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <real.h>
#include <he/area.h>
#include <he/memory.h>
#include <he/macro.h>
#include <he/y.h>
#include <he/he.h>
#include <he/err.h>
#include <he/strain.h>
#include <he/vtk.h>
#include <he/f/strain.h>

static char **argv;
static real *x, *y, *z, *fm, *fx, *fy, *fz, *area, *area0;
static real *x0, *y0, *z0;
static int nv, nt;
static HeFStrain *strain;
static He *he, *he0;

static real energy() { return he_f_strain_energy(strain, he, x, y, z); }

int main0() {
    real *eng, e, *al, *be, *al_tri, *be_tri, *eng_tri;
    he_f_strain_argv(&argv, he, &strain);

    he_f_strain_force(strain, he, x, y, z, /**/ fx, fy, fz);
    e = he_f_strain_energy(strain, he, x, y, z);
    he_f_strain_energy_ver(strain, &eng);
    he_f_strain_energy_tri(strain, &eng_tri);

    he_f_strain_invariants(strain, x, y, z, &al, &be);
    he_f_strain_invariants_tri(strain, x, y, z, &al_tri, &be_tri);

    he_area_ver(he, x, y, z, /**/ area);
    he_area_ver(he0, x0, y0, z0, /**/ area0);

    const real *sc[] = {fx, fy, fz, eng, area, area0, al, be, NULL};
    const char *na[] = {"fx", "fy", "fz", "eng", "area", "area0", "al", "be", NULL};
    vtk_write(he, x, y, z, sc, na, "ver.vtk");

    const real *sc_tri[] = {eng_tri, al_tri, be_tri, NULL};
    const char *na_tri[] = {"eng", "al", "be", NULL};
    vtk_tri_write(he, x, y, z, sc_tri, na_tri, "tri.vtk");

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
