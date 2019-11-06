#include <stdio.h>
#include <math.h>

#include <real.h>
#include <co/memory.h>
#include <co/off.h>
#include <co/restore.h>
#include <co/he.h>
#include <co/y.h>

static int nv;
static real *x, *y, *z;
static He *he;
static Restore *restore;

static void main0() {
    const real *queue[] = {x, y, z, NULL};
    restore_volume(restore, he, x, y, z);
    off_he_xyz_fwrite(he, x, y, z, stdout);
}

int main() {
    real V;
    y_inif(stdin, &he, &x, &y, &z);
    nv = he_nv(he);
    V = 5.0;
    restore_ini(V, he, &restore);
    main0();
    restore_fin(restore);

    y_fin(he, x, y, z);
    return 0;
}
