#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>
#include <string.h>

#include <real.h>

#include <he/force.h>
#include <he/punto.h>
#include <he/area.h>
#include <he/he.h>
#include <he/off.h>
#include <he/memory.h>
#include <he/err.h>
#include <he/util.h>
#include <he/macro.h>
#include <he/vec.h>
#include <he/y.h>

#define FMT_IN   XE_REAL_IN

static const char **argv;
static char name[4048];

static real *fx, *fy, *fz, *fm, *x, *y, *z, *rr, *area;
static int nv, nt;
static He *he;
static Force *force;
static real param[99];

static const char *me = "bending";

static void usg() {
    const char *list;
    list = force_list();
    fprintf(stderr, "%s %s Kb C0 Kad DA0D < OFF > PUNTO\n", me, list);
}

static int eq(const char *a, const char *b) { return util_eq(a, b); }
static int scl(/**/ real *p) {
    if (*argv == NULL) {
        usg();
        ER("not enough args");
    }
    if (sscanf(*argv, FMT_IN, p) != 1)
        ER("not a number '%s'", *argv);
    argv++;
    return HE_OK;
}
static int str(/**/ char *p) {
    if (*argv == NULL) {
        usg();
        ER("not enough args");
    }
    strncpy(p, *argv, 4047);
    argv++;
    return HE_OK;
}
static void arg() {
    if (*argv != NULL && eq(*argv, "-h")) {
        usg();
        exit(0);
    }
    str(name);
    scl(&param[0]); scl(&param[1]);
}

static void main0() {
    int i;
    real e, r[3], f[3];

    force_ini(name, param, he,  &force);
    force_force(force, he, x, y, z, /**/ fx, fy, fz);
    e = force_energy(force, he, x, y, z);
    he_area_ver(he, x, y, z, /**/ area);

    MSG("energy: %g", e);
    MSG("f0: %g %g %g", fx[0], fy[0], fz[0]);
    MSG("f0: %g %g %g", fx[nv - 1], fy[nv  - 1], fz[nv - 1]);

    for (i = 0; i < nv; i++) {
        vec_get(i, x, y, z, /**/ r);
        vec_get(i, fx, fy, fz, /**/ f);
        rr[i] = vec_cylindrical_r(r);
        fm[i] = vec_abs(f);
    }

    char *key = "r x y z fm fx fy fz area";
    real *queue[] = {rr, x, y, z, fm, fx, fy, fz, area, NULL};
    puts(key);
    punto_fwrite(nv, queue, stdout);
    force_fin(force);
}

int main(int __UNUSED argc, const char *v[]) {
    argv = v; argv++;
    arg();

    y_ini("/dev/stdin", &he, &x, &y, &z);
    nv = he_nv(he);
    nt = he_nt(he);

    MALLOC(nv, &rr); MALLOC(nv, &fm); MALLOC(nv, &area);
    CALLOC(nv, &fx); CALLOC(nv, &fy); CALLOC(nv, &fz);

    main0();

    FREE(rr); FREE(fm);
    FREE(fx); FREE(fy); FREE(fz);

    y_fin(he, x, y, z);
}
