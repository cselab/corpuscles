#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <real.h>

#include <co/force.h>
#include <co/punto.h>
#include <co/area.h>
#include <co/he.h>
#include <co/memory.h>
#include <co/err.h>
#include <co/util.h>
#include <co/argv.h>
#include <co/macro.h>
#include <co/vec.h>
#include <co/fd.h>
#include <co/y.h>

#define FMT_IN   CO_REAL_IN
#define FMT_OUT  CO_REAL_OUT

static const char **argv;

static char name[1024];

static real *fx, *fy, *fz, *fm, *x, *y, *z, *rr, *area;
static real *gx, *gy, *gz;
static int nv, nt;
static He *he;
static Force *force;
static real delta = 1e-6;

static const char *me = "force/generic";

static void usg(void) {
    const char *list;
    list = force_list();
    fprintf(stderr, "%s %s [args..] < OFF > PUNTO\n", me, list);
    exit(2);
}

static void main0() {
    int i;
    real e, r[3], f[3];

    force_force(force, he, x, y, z, /**/ fx, fy, fz);
    fd(force, he, delta, x, y, z, /**/ gx, gy, gz);
    e = force_energy(force, he, x, y, z);
    he_area_ver(he, x, y, z, /**/ area);

    MSG("name: %s", force_name(force));
    MSG("energy: " FMT_OUT, e);
    MSG("f0: " FMT_OUT " " FMT_OUT " " FMT_OUT, fx[0], fy[0], fz[0]);
    MSG("f0: " FMT_OUT " " FMT_OUT " " FMT_OUT, gx[0], gy[0], gz[0]);
    for (i = 0; i < nv; i++) {
        vec_get(i, x, y, z, /**/ r);
        vec_get(i, fx, fy, fz, /**/ f);
        rr[i] = vec_cylindrical_r(r);
        fm[i] = vec_abs(f);
    }

    char *key = "r x y z fm fx fy fz area gx gy gz";
    const real *queue[] = {rr, x, y, z, fm, fx, fy, fz, area, gx, gy, gz, NULL};
    puts(key);
    punto_fwrite(nv, queue, stdout);
    force_fin(force);
}

int main(int __UNUSED argc, char *argv[]) {
    err_set_ignore();
    argv++;
    if (*argv == NULL)
	ER("needs an argument");
    if (util_eq(*argv, "-h"))
        usg();
    y_inif(stdin, &he, &x, &y, &z);

    argv_str(&argv, name);
    force_argv(name, &argv, he,  &force);
    nv = he_nv(he);
    nt = he_nt(he);

    MALLOC(nv, &rr); MALLOC(nv, &fm); MALLOC(nv, &area);
    CALLOC(nv, &fx); CALLOC(nv, &fy); CALLOC(nv, &fz);
    MALLOC(nv, &gx); MALLOC(nv, &gy); MALLOC(nv, &gz);

    main0();

    FREE(rr); FREE(fm); FREE(area);
    FREE(fx); FREE(fy); FREE(fz);
    FREE(gx); FREE(gy); FREE(gz);

    y_fin(he, x, y, z);
}
