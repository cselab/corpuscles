#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include <real.h>
#include <he/memory.h>
#include <he/punto.h>
#include <he/macro.h>
#include <he/off.h>
#include <he/he.h>
#include <he/err.h>
#include <he/strain.h>
#include <he/f/strain.h>

static const real pi = 3.141592653589793115997964;
static const char **argv;
static real *xx, *yy, *zz, *gx, *gy, *gz, *fm, *fx, *fy, *fz;
static int nv, nt;
static HeFStrain *strain;
static He *he;
static real h = 1e-6;

static real energy() {
    return he_f_strain_energy(strain, he, xx, yy, zz);
}

static real fd0(real *p) {
    real e, ep, em, v, t;
    v = *p;
    *p += h; ep  = energy(); *p = v;
    *p -= h; em  = energy(); *p = v;
    return (ep - em)/(2*h);
}

static int fd(int i, real *gx, real *gy, real *gz) {
    enum {X, Y, Z};
    gx[i] = fd0(&xx[i]);
    gy[i] = fd0(&yy[i]);
    gz[i] = fd0(&zz[i]);
    return HE_OK;
}

int main0() {
    int i;
    real *eng, e, eh, tmp;
    StrainParam param;
    param.Ka = 0;
    param.Ks = 1;
    he_f_strain_ini("skalak", param, xx, yy, zz, he, /**/ &strain);
    for (i = 0; i < nv; i++) {
        xx[i] += 0.01*xx[i]*yy[i];
        yy[i] += 0.01*xx[i]*xx[i];
    }
    he_f_strain_force(strain, he, xx, yy, zz, /**/ fx, fy, fz);
    e = he_f_strain_energy(strain, he, xx, yy, zz);
    he_f_strain_energy_ver(strain, &eng);
    MSG("eng: %g", e);
    for (i = 0; i < nv; i++)
        fd(i, gx, gy, gz);
    real *queue[] = {xx, yy, zz, fx, fy, fz, gx, gy, gz, eng, NULL};
    puts("x y z fx fy fz gx gy gz eng");
    punto_fwrite(nv, queue, stdout);
    he_f_strain_fin(strain);
    
    return HE_OK;
}

int main(int __UNUSED argc, const char *v[]) {
    int *tri;
    const char path[] = "/dev/stdin";
    static HeOff *off;
    argv = v; argv++;
    he_off_ini(path, &off);

    nv = he_off_nv(off);
    nt = he_off_nt(off);
    he_off_tri(off, &tri);
    he_tri_ini(nv, nt, tri, &he);

    MALLOC(nv, &xx); MALLOC(nv, &yy); MALLOC(nv, &zz);
    CALLOC(nv, &fm); CALLOC(nv, &fx);  CALLOC(nv, &fy); CALLOC(nv, &fz);
    CALLOC(nv, &gx);  CALLOC(nv, &gy); CALLOC(nv, &gz);

    he_off_xyz(off, xx, yy, zz);

    main0();
    
    FREE(xx); FREE(yy); FREE(zz);
    FREE(fm); FREE(fx); FREE(fy); FREE(fz);
    FREE(gx); FREE(gy); FREE(gz);    

    he_off_fin(off);
    he_fin(he);

    return 0;
}
