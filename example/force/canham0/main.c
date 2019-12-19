#include <stdio.h>
#include <math.h>

#include <real.h>
#include <co/err.h>
#include <co/off.h>
#include <co/punto.h>
#include <co/memory.h>
#include <co/f/canham.h>
#include <co/he.h>

typedef struct Param Param;
struct Param { real Kb, C0, Kad, DA0D; };

static Off *read;
static HeFCanham *bending;
static He *he;
static real *x, *y, *z, *r, *eng, e0;
static real *fx, *fy, *fz;
static int  nv, nt, *tri;
static Param param;

static void ini() {
    off_ini("/dev/stdin", &read);
    nv = off_nv(read);
    nt = off_nt(read);
    off_tri(read, &tri);
    MALLOC(nv, &x); MALLOC(nv, &y); MALLOC(nv, &z); MALLOC(nv, &r);
    CALLOC(nv, &fx); CALLOC(nv, &fy); CALLOC(nv, &fz);

    off_xyz(read, x, y, z);
    he_tri_ini(nv, nt, tri, &he);

    param.Kb = 1;
    param.C0 = param.Kad = param.DA0D = 0;
    he_f_canham_ini(param.Kb, param.C0, param.Kad, param.DA0D,
                    he, &bending);
}
static void fin() {
    he_f_canham_fin(bending);
    off_fin(read);
    he_fin(he);
    FREE(x); FREE(y); FREE(z); FREE(r);
    FREE(fx); FREE(fy); FREE(fz);
}

int main() {
    int i;
    real *mean, *gaus;

    ini();

    e0 = he_f_canham_energy(bending, he, x, y, z);
    he_f_canham_force(bending, he, x, y, z, /**/ fx, fy, fz);
    he_f_canham_energy_ver(bending, /**/ &eng);
    MSG("eng[0]: %g", eng[0]);


    he_f_canham_H_ver(bending, &mean);
    he_f_canham_G_ver(bending, &gaus);

    for (i = 0; i < nv; i++)
        r[i] = sqrt(x[i]*x[i] + y[i]*y[i]);

    char *key = "r x y z H G";
    const real *queue[] = {r, x, y, z, mean, gaus, NULL};
    puts(key);
    punto_fwrite(nv, queue, stdout);

    fin();
}
