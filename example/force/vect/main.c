#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <real.h>

#include <he/area.h>
#include <he/argv.h>
#include <he/err.h>
#include <he/force.h>
#include <he/he.h>
#include <he/macro.h>
#include <he/memory.h>
#include <he/off.h>
#include <he/util.h>
#include <he/vec.h>
#include <he/y.h>

#define FMT_IN   HE_REAL_IN

static const char **argv;

static char name[1024];

static real *fx, *fy, *fz, *fm, *x, *y, *z, *rr, *area;
static int nv, nt;
static He *he, *he0;
static Force *force;

static const char *me = "force/vect";

static void usg(void) {
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

    MSG("name: %s", force_name(force));
    MSG("energy: %g", e);
    MSG("f0: %g %g %g", fx[0], fy[0], fz[0]);

    //boff_vect_fwrite(he, x, y, z, fx, fy, fz, stdout);
    boff_point_fwrite(he, x, y, z, fx, stdout);

    force_fin(force);
}

int main(int __UNUSED argc, char *argv[]) {
    argv++;
    if (util_eq(*argv, "-h"))
        usg();
    y_inif(stdin, &he, &x, &y, &z);

    argv_str(&argv, name);
    force_argv(name, &argv, he,  &force);
    nv = he_nv(he);
    nt = he_nt(he);

    MALLOC(nv, &rr); MALLOC(nv, &fm); MALLOC(nv, &area);
    CALLOC(nv, &fx); CALLOC(nv, &fy); CALLOC(nv, &fz);

    main0();

    FREE(rr); FREE(fm); FREE(area);
    FREE(fx); FREE(fy); FREE(fz);

    y_fin(he, x, y, z);
}
