#include <stdio.h>

#include <real.h>
#include <he/err.h>
#include <he/off.h>
#include <he/punto.h>
#include <he/memory.h>
#include <he/f/canham.h>
#include <he/he.h>

typedef struct Param Param;
struct Param { real Kb, C0, Kad, DA0D; };

static HeOff *read;
static HeFCanham *bending;
static He *he;
static real *x, *y, *z, *eng, e0;
static real *fx, *fy, *fz;
static int  nv, nt, *tri;
static Param param;

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
    he_f_canham_ini(param.Kb, param.C0, param.Kad, param.DA0D,
                    he, &bending);
}
static void fin() {
    he_f_canham_fin(bending);
    he_off_fin(read);
    he_fin(he);
    FREE(x); FREE(y); FREE(z);
    FREE(fx); FREE(fy); FREE(fz);
}

int main() {
    ini();

    e0 = he_f_canham_energy(bending, he, x, y, z);
    he_f_canham_force(bending, he, x, y, z, /**/ fx, fy, fz);
    he_f_canham_energy_ver(bending, /**/ &eng);
    MSG("eng[0]: %g", eng[0]);


    char *key = "x y z fm fx fy fz eng";
    real *queue[] = {x, y, z, fx, fy, fz, eng, NULL};
    puts(key);
    punto_fwrite(nv, queue, stdout);

    fin();
}
