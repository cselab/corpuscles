#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <real.h>
#include <co/bending.h>
#include <co/memory.h>
#include <co/err.h>
#include <co/he.h>
#include <co/off.h>
#include <co/restore.h>
#include <co/y.h>

static He *he;
static real *x, *y, *z, *fx, *fy, *fz;
static Bending *bending;
static Restore *restore;
static BendingParam param;
static int nv;

int dump(void) {
    static int i;
    char name[4048];
    snprintf(name, sizeof(name) - 1, "%05d.off", i++);
    off_he_xyz_write(he, x, y, z, name);
    return CO_OK;
}

int euler(real dt, const real *fx, const real *fy, const real *fz,
          real *x, real *y, real *z) {
    int i;
    for (i = 0; i < nv; i++) {
        x[i] += dt*fx[i]; y[i] += dt*fy[i]; z[i] += dt*fz[i];
    }
    return CO_OK;
}


int main(void) {
    real eng, dt;
    int i;
    const char *name = "meyer_xin";

    y_inif(stdin, &he, &x, &y, &z);
    nv = he_nv(he);

    dt = 1e-5;
    param.Kb = 1;
    bending_ini(name, param, he,  &bending);
    restore_ini(1000, he, &restore);

    dump();
    for (i = 0; i < 100000; i++) {
        eng = bending_energy(bending, he, x, y, z);
        bending_laplace_ver(bending, &fx, &fy, &fz);
        euler(dt, fx, fy, fz, x, y, z);
        restore_volume(restore, he, x, y, z);
        if (i % 1000 == 0) dump();
    }

    MSG("eng: %g", eng);
    MSG("abs(f[0]): %g",
        sqrt(fx[0]*fx[0] + fy[0]*fy[0] + fz[0]*fz[0]));
    MSG("fx[0]: %g", fx[0]);
    MSG("fy[nv-1]: %g", fx[nv-1]);

    restore_fin(restore);
    bending_fin(bending);
    y_fin(he, x, y, z);
}
