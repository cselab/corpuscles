#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <real.h>
#include <co/err.h>
#include <co/memory.h>
#include <co/macro.h>
#include <co/ply.h>
#include <co/f/wlc.h>
#include <co/he.h>
#include <co/sum.h>
#include <co/y.h>

static HeFWlc *force;
static Ply *cell;

static He *he;
static int nv;
static const char **argv;
static char off[4048], ply[4048];

static int str(/**/ char *p) {
    if (*argv == NULL) ER("not enough args");
    strncpy(p, *argv, 4048);
    argv++;
    return CO_OK;
}


static real mean(int n, real *x) {
    return he_sum_array(n, x)/n;
}

static void main0() {
    int i, nm;
    real *x, *y, *z;
    real xc, yc, zc, en;

    nm = ply_nm(cell);
    for (i = 0; i < nm; i++) {
        ply_x(cell, i, &x);
        ply_y(cell, i, &y);
        ply_z(cell, i, &z);

        xc = mean(nv, x);
        yc = mean(nv, y);
        zc = mean(nv, z);
        en = he_f_wlc_energy(force, he, x, y, z);
        
        printf("%g %g %g %g\n", xc, yc, zc, en);
    }
}

int main(int __UNUSED argc, const char *v[]) {
    int nt;
    real K, x0;
    FILE *f;
    real *x, *y, *z;

    argv = v; argv++;
    str(off);
    str(ply);

    y_ini(off, &he, &x, &y, &z);
    if ((f = fopen(ply, "r")) == NULL)
        ER("fail to open '%s'", ply);
    ply_fread(f, &cell);
    fclose(f);

    nv = he_nv(he);
    nt = he_nt(he);

    if (nv != ply_nv(cell))
        ER("off_nv=%d != ply_nv=%d", nv, ply_nv(cell));

    if (nt != ply_nt(cell))
        ER("off_nt=%d != ply_nt=%d", nt, ply_nt(cell));

    K = 1;
    x0 = 0.4545;
    he_f_wlc_ini(x0, K, x, y, z, he, /**/ &force);

    main0();

    he_f_wlc_fin(force);
    y_fin(he, x, y, z);
    return 0;
}
