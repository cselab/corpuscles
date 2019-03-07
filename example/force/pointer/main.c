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
#include <co/f/juelicher_xin.h>
#include <co/vec.h>
#include <co/y.h>

#define FMT   CO_REAL_OUT

static const char *me = "force/pointer";
static const char **argv;

static char name[1024];
static real *x, *y, *z;
static He *he;
static Force *force;
static HeFJuelicherXin *juelicher_xin;

static void usg(void) {
    const char *list;
    list = force_list();
    fprintf(stderr, "%s %s [args..]\n", me, list);
    exit(2);
}

int main(int __UNUSED argc, char *argv[]) {
    real ad, bend, e;
    
    argv++;
    if (util_eq(*argv, "-h"))
        usg();
    y_inif(stdin, &he, &x, &y, &z);
    argv_str(&argv, name);
    force_argv(name, &argv, he,  &force);
    e = force_energy(force, he, x, y, z);
    juelicher_xin = force_pointer(force);
    ad = he_f_juelicher_xin_energy_ad(juelicher_xin);
    bend = he_f_juelicher_xin_energy_bend(juelicher_xin);
    printf(FMT " " FMT " " FMT "\n", e, ad, bend);
    y_fin(he, x, y, z);
}
