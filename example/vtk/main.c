#include <stdio.h>

#include <real.h>
#include <he/err.h>
#include <he/off.h>
#include <he/vtk.h>
#include <he/memory.h>
#include <he/bending.h>
#include <he/he.h>

static HeOff *read;
static Bending *bending;
static He *he;
static real *x, *y, *z, *eng, e0;
static real *fx, *fy, *fz;
static int  nv, nt, *tri;
static BendingParam param;

static void ini() {
    he_off_ini("/dev/stdin", &read);
    nv = he_off_nv(read);
    nt = he_off_nt(read);
    he_off_tri(read, &tri);
    MALLOC(nv, &x); MALLOC(nv, &y); MALLOC(nv, &z);
    CALLOC(nv, &fx); CALLOC(nv, &fy); CALLOC(nv, &fz);

    he_off_xyz(read, x, y, z);
    he_tri_ini(nv, nt, tri, &he);

    param.Kb = 1;
    param.C0 = param.Kad = param.DA0D = 0;
    bending_ini("meyer", param, he, &bending);
}
static void fin() {
    bending_fin(bending);
    he_off_fin(read);
    he_fin(he);
    FREE(x); FREE(y); FREE(z);
    FREE(fx); FREE(fy); FREE(fz);
}

int main() {
    const char *path = "/dev/stdout";

    ini();

    e0 = bending_energy(bending, he, x, y, z);
    bending_force(bending, he, x, y, z, /**/ fx, fy, fz);
    bending_energy_ver(bending, /**/ &eng);
    MSG("eng[0]: %g", eng[0]);
    const real *scalars[] =  {fx, fy, fz, eng, NULL};
    const char *names[] =  {"fx", "fy", "fz", "eng", NULL};
    he_vtk_write(he, x, y, z, scalars, names, path);

    fin();
}
