#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <real.h>

#include <he/force.h>
#include <he/vtk.h>
#include <he/area.h>
#include <he/he.h>
#include <he/memory.h>
#include <he/err.h>
#include <he/util.h>
#include <he/argv.h>
#include <he/macro.h>
#include <he/vec.h>
#include <he/y.h>

#define FMT_IN   HE_REAL_IN

static const char **argv;

static char name[1024];

static real *fx, *fy, *fz, *fm, *x, *y, *z, *rr, *area, *area0;
static real *x0, *y0, *z0;
static int nv, nt;
static He *he, *he0;
static Force *force;
static real delta = 1e-6;

static const char *me = "force/generic";

static void usg() {
    const char *list;
    list = force_list();
    fprintf(stderr, "%s %s [args..] < OFF > VTK\n", me, list);
    exit(2);
}

static void main0() {
    int i;
    real e, r[3], f[3];

    force_force(force, he, x, y, z, /**/ fx, fy, fz);
    e = force_energy(force, he, x, y, z);
    he_area_ver(he, x, y, z, /**/ area);
    he_area_ver(he0, x0, y0, z0, /**/ area0);

    MSG("name: %s", force_name(force));
    MSG("energy: %g", e);
    MSG("f0: %g %g %g", fx[0], fy[0], fz[0]);

    for (i = 0; i < nv; i++) {
        vec_get(i, x, y, z, /**/ r);
        vec_get(i, fx, fy, fz, /**/ f);
        rr[i] = vec_cylindrical_r(r);
        fm[i] = vec_abs(f);
    }

    const char *na[] = {"fm", "fx", "fy", "fz", "area", "area0", NULL};
    const real *sc[] = {fm, fx, fy, fz, area, area0, NULL};
    vtk_fwrite(he, x, y, z, sc, na, stdout);
    force_fin(force);
}

int main(int __UNUSED argc, char *argv[]) {
    argv++;
    if (util_eq(*argv, "-h"))
        usg();
    y_ini("/dev/stdin", &he, &x, &y, &z);
    y_ini("/dev/stdin", &he0, &x0, &y0, &z0);

    argv_str(&argv, name);
    force_argv(name, &argv, he,  &force);
    nv = he_nv(he);
    nt = he_nt(he);

    MALLOC(nv, &rr); MALLOC(nv, &fm); MALLOC(nv, &area); MALLOC(nv, &area0);
    CALLOC(nv, &fx); CALLOC(nv, &fy); CALLOC(nv, &fz);

    main0();

    FREE(rr); FREE(fm); FREE(area); FREE(area0);
    FREE(fx); FREE(fy); FREE(fz);

    y_fin(he, x, y, z);
    y_fin(he0, x0, y0, z0);    
}