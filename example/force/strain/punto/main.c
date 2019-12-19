#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <real.h>
#include <co/area.h>
#include <co/memory.h>
#include <co/punto.h>
#include <co/macro.h>
#include <co/y.h>
#include <co/he.h>
#include <co/err.h>
#include <co/strain.h>
#include <co/f/strain.h>

static const real pi = 3.141592653589793115997964;
static char **argv;
static real *x, *y, *z, *fm, *fx, *fy, *fz, *area, *area0;
static real *x0, *y0, *z0;
static int nv, nt;
static HeFStrain *strain;
static He *he, *he0;

#define FMT   CO_REAL_OUT
static real
energy()
{
    return he_f_strain_energy(strain, he, x, y, z);
}

int
main0()
{
    real *eng, e, *al, *be, ea, eb;

    he_f_strain_argv(&argv, he, &strain);

    he_f_strain_force(strain, he, x, y, z, /**/ fx, fy, fz);
    e = he_f_strain_energy(strain, he, x, y, z);
    he_f_strain_energies(strain, &ea, &eb);
    he_f_strain_energy_ver(strain, &eng);

    he_f_strain_invariants(strain, x, y, z, &al, &be);

    he_area_ver(he, x, y, z, /**/ area);
    he_area_ver(he0, x0, y0, z0, /**/ area0);

    const real *sc[] =
        { x, y, z, fx, fy, fz, eng, area, area0, al, be, NULL };
    const char *na[] =
        { "x", "y", "z", "fx", "fy", "fz", "eng", "area", "area0", "al",
"be", NULL };

    puts("x y z fx fy fz eng area area0 al be");
    punto_fwrite(nv, sc, stdout);

    MSG(FMT " " FMT " " FMT, e, ea, eb);

    he_f_strain_fin(strain);

    return CO_OK;
}

int
main(int __UNUSED argc, char *v[])
{
    argv = v;
    argv++;

    y_inif(stdin, /**/ &he, &x, &y, &z);
    y_ini(*argv, /**/ &he0, &x0, &y0, &z0);

    nv = he_nv(he);

    CALLOC(nv, &fm);
    CALLOC(nv, &fx);
    CALLOC(nv, &fy);
    CALLOC(nv, &fz);
    MALLOC(nv, &area);
    MALLOC(nv, &area0);

    main0();

    FREE(fm);
    FREE(fx);
    FREE(fy);
    FREE(fz);
    FREE(area);
    FREE(area0);

    y_fin(he, x, y, z);
    y_fin(he0, x0, y0, z0);

    return 0;
}
