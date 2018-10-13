#include <stdio.h>

#include <real.h>
#include <he/err.h>
#include <he/off.h>
#include <he/vtk.h>
#include <he/memory.h>
#include <he/he.h>

static HeOff *read;
static He *he;
static real *x, *y, *z;
static int  nv, nt, *tri;

static void ini() {
    he_off_ini("/dev/stdin", &read);
    nv = he_off_nv(read);
    nt = he_off_nt(read);
    he_off_tri(read, &tri);

    MALLOC(nv, &x); MALLOC(nv, &y); MALLOC(nv, &z);

    he_off_xyz(read, x, y, z);
    he_tri_ini(nv, nt, tri, &he);
}
static void fin() {
    he_off_fin(read);
    he_fin(he);
}

int main() {
    enum {X, Y, Z};
    ini();

    const real *scalars[] =  {x, y, z, NULL};
    const char *names[] =  {"fx", "fy", "fz", NULL,};
    const char *path = "/dev/stdout";

    he_vtk_write(he, x, y, z, scalars, names, path);
    fin();
}
