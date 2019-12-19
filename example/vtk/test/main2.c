#include <stdio.h>

#include <real.h>
#include <co/err.h>
#include <co/off.h>
#include <co/vtk.h>
#include <co/memory.h>
#include <co/bending.h>
#include <co/he.h>

static Off *read;
static Bending *bending1;
static Bending *bending2;
static He *he;
static real *x, *y, *z;
static real *eng1, *eng2, *deng, e1, e2;
static real *fx, *fy, *fz;
static int  nv, nt, *tri;
static BendingParam param;

static void ini() {
    he_off_ini("/dev/stdin", &read);
    nv = he_off_nv(read);
    nt = he_off_nt(read);
    he_off_tri(read, &tri);
    MALLOC(nv, &x); MALLOC(nv, &y); MALLOC(nv, &z);
    MALLOC(nv, &deng);
    CALLOC(nv, &fx); CALLOC(nv, &fy); CALLOC(nv, &fz);

    he_off_xyz(read, x, y, z);
    he_tri_ini(nv, nt, tri, &he);

    param.Kb = 1;
    param.C0 = param.Kad = param.DA0D = 0;
    bending_ini("juelicher_xin", param, he, &bending1);
    bending_ini("gompper", param, he, &bending2);
}
static void fin() {
    bending_fin(bending1);
    bending_fin(bending2);
    he_off_fin(read);
    he_fin(he);
    FREE(x); FREE(y); FREE(z);
    FREE(deng);
    FREE(fx); FREE(fy); FREE(fz);
}

int main() {
    const char *path = "/dev/stdout";

    ini();

    e1 = bending_energy(bending1, he, x, y, z);
    e2 = bending_energy(bending2, he, x, y, z);
    bending_force(bending1, he, x, y, z, /**/ fx, fy, fz);
    bending_energy_ver(bending1, /**/ &eng1);
    bending_energy_ver(bending2, /**/ &eng2);
    MSG("eng1[0], eng2[0]: %g%g", eng1[0], eng2[0]);
    
    for (int i=0;i<nv;i++){
      deng[i] = (eng1[i] - eng2[i])/eng1[i];
    }
      
    const real *scalars[] =  {fx, fy, fz, eng1, eng2,  deng, NULL};
    const char *names[] =  {"fx", "fy", "fz", "eng1", "eng2", "deng", NULL};

    vtk_write(he, x, y, z, scalars, names, path);

    fin();
}
