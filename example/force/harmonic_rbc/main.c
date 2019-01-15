#include <stdio.h>
#include <tgmath.h>
#include <stdlib.h>
#include <string.h>

#include <real.h>
#include <he/err.h>
#include <he/memory.h>
#include <he/macro.h>
#include <he/ply.h>
#include <he/f/harmonic_ref.h>
#include <he/he.h>
#include <he/y.h>

static HeFHarmonicRef *force;
static Ply *cell;

static He *he;
static int nv;
static const char **argv;
static char off[4048], ply[4048];

static int str(/**/ char *p) {
    if (*argv == NULL) ER("not enough args");
    strncpy(p, *argv, 4048);
    argv++;
    return HE_OK;
}

static void main0() {
    int i, nm;
    real *x, *y, *z;

    nm = ply_nm(cell);
    for (i = 0; i < nm; i++) {
        ply_x(cell, i, &x);
        ply_y(cell, i, &y);
        ply_z(cell, i, &z);
        printf("%g\n", he_f_harmonic_ref_energy(force, he, x, y, z));
    }
}

int main(int __UNUSED argc, const char *v[]) {
    int nt;
    real K;
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
    he_f_harmonic_ref_ini(K, x, y, z, he, /**/ &force);

    main0();

    he_f_harmonic_ref_fin(force);
    y_fin(he, x, y, z);
    return 0;
}
