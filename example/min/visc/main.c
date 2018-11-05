#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include <real.h>

#include <he/area.h>
#include <he/bending.h>
#include <he/equiangulate.h>
#include <he/err.h>
#include <he/f/area.h>
#include <he/f/garea.h>
#include <he/f/strain.h>
#include <he/he.h>
#include <he/macro.h>
#include <he/memory.h>
#include <he/off.h>
#include <he/punto.h>
#include <he/restore.h>
#include <he/strain.h>
#include <he/util.h>
#include <he/vec.h>
#include <he/volume.h>
#include <he/y.h>

static const char *me = "min/visc";
static const real pi = 3.141592653589793115997964;
static const real EPS = 1e-6;
static Restore *restore;

static He *he;
static int NV, NE, NT;
static HeFArea *f_area;
static HeFGarea *f_garea;
static Bending *f_bending;
static real *XX, *YY, *ZZ;
static real rVolume, Ka, Kga;
static real A0, V0, e0;
static const char **argv;
static char bending[4049], model[4049], off[4049];
static StrainParam strain_param;
static HeFStrain *f_strain;
static real Kb, C0, Kad, DA0D;

static void zero(int n, real *a) {
    int i;
    for (i = 0; i < n; i++) a[i] = 0;
}

#define FMT_IN   XE_REAL_IN


static void usg() {
    fprintf(stderr, "%s %s \\\n"
            "  rVolume Ka Kga {off skalak/linear Ks Ka} {Kb C0 Kad DA0D} < OFF > PUNTO\n", me, bending_list());
    exit(0);
}

static real sph_volume(real area) { return pow(area, 1.5)/(6*pi); }

static real target_volume(real area, real v) { return v*sph_volume(area); }
static real target_area(real volume, real v) { return 4.835975862049408*pow(volume, 2.0/3)/pow(v, 2.0/3); }
static real reduced_volume(real area, real volume) { return (6*sqrt(pi)*volume)/pow(area, 3.0/2); }
static real eq_tri_edg(real area) { return 2*sqrt(area)/pow(3, 0.25); }

static int eq(const char *a, const char *b) { return util_eq(a, b); }
static int scl(/**/ real *p) {
    if (*argv == NULL) ER("not enough args");
    if (sscanf(*argv, FMT_IN, p) != 1)
        ER("not a number '%s'", *argv);
    argv++;
    return HE_OK;
}
static int str(/**/ char *p) {
    if (*argv == NULL) ER("not enough args");
    strncpy(p, *argv, 4048);
    argv++;
    return HE_OK;
}
static void arg() {
    if (*argv != NULL && eq(*argv, "-h")) usg();
    str(bending);
    scl(&rVolume); scl(&Ka); scl(&Kga);
    str(off); str(model); scl(&strain_param.Ks); scl(&strain_param.Ka);
    scl(&Kb); scl(&C0);  scl(&Kad); scl(&DA0D);
}

real Energy(const real *x, const real *y, const real *z) {
    real a, ga, e, b;
    a = he_f_area_energy(f_area, he, x, y, z);
    ga = he_f_garea_energy(f_garea, he, x, y, z);
    e = he_f_strain_energy(f_strain, x, y, z);
    b = bending_energy(f_bending, he, x, y, z);
    MSG("a ga e b: %g %g %g %g", a, ga, e, b);
    return a + ga + e + b;
}

void Force(const real *x, const real *y, const real *z, /**/
           real *fx, real *fy, real *fz) {
    zero(NV, fx); zero(NV, fy); zero(NV, fz);
    he_f_area_force(f_area, he, x, y, z, /**/ fx, fy, fz);
    he_f_garea_force(f_garea, he, x, y, z, /**/ fx, fy, fz);
    he_f_strain_force(f_strain, x, y, z, /**/ fx, fy, fz);
    bending_force(f_bending, he, x, y, z, /**/ fx, fy, fz);
}

static void euler(real dt,
                  const real *fx, const real *fy, const real *fz,
                  real *vx, real *vy, real *vz) {
    int i;
    for (i = 0; i < NV; i++) {
        vx[i] += dt*fx[i];
        vy[i] += dt*fy[i];
        vz[i] += dt*fz[i];
    }
}

static void jigle(real mag, /**/ real *vx, real *vy, real *vz) {
    int nv;
    real r, r0, sx, sy, sz;
    int i;
    nv = NV;
    sx = sy = sz = 0;
    for (i = 0; i < nv; i++) {
        r = rand()/(real)RAND_MAX - 0.5;
        r0 = r * mag;
        vx[i] += r0; vy[i] += r0; vz[i] += r0;
    }
    for (i = 0; i < nv; i++) {
        sx += vx[i]; sy += vy[i]; sz += vz[i];
    }
    sx /= nv; sy /= nv; sz /= nv;
    for (i = 0; i < nv; i++) {
        vx[i] -= sx; vy[i] -= sy; vz[i] -= sz;
    }
}

static int visc_pair(real mu,
                      const real *vx, const real *vy, const real *vz, /*io*/
                      real *fx, real *fy, real *fz) {
    int e, h, n, i, j;
    real a[3], b[3], u[3], u0;
    for (e = 0; e < NE; e++) {
        h = he_hdg_edg(he, e);
        n = he_nxt(he, h);
        
        i = he_ver(he, h);
        j = he_ver(he, n);        
        
        vec_get(i, vx, vy, vz, a);
        vec_get(j, vx, vy, vz, b);
        vec_minus(a, b, u);
        u0 = vec_abs(u);
        vec_scalar_append(u, -mu*u0, i, fx, fy, fz);
        vec_scalar_append(u,  mu*u0, j, fx, fy, fz);
    }
}

static real Kin(real *vx, real *vy, real *vz) {
    int i;
    real s;
    s = 0;
    for (i = 0; i < NV; i++) {
        s += vx[i]*vx[i];
        s += vy[i]*vy[i];
        s += vz[i]*vz[i];
    }
    return s;
}

static real max_vec(real *fx, real *fy, real *fz) {
    int i;
    real c, m;
    m = 0;
    for (i = 0; i < NV; i++) {
        c = sqrt(fx[i]*fx[i] + fy[i]*fy[i] + fz[i]*fz[i]);
        if (c > m)
            m = c;
    }
    return m;
}

static void main0(real *vx, real *vy, real *vz,
                  real *fx, real *fy, real *fz) {
    int cnt, end, i;
    real dt, dt_max, h, mu, rnd;
    real A, V;
    real *queue[] = {XX, YY, ZZ, NULL};
    
    dt_max = 0.001;
    mu = 100.0;
    h = 0.01*e0;
    end = 10000;
    
    zero(NV, vx); zero(NV, vy); zero(NV, vz);
    for (i = 0; i < end ; i++) {
        Force(XX, YY, ZZ, /**/ fx, fy, fz);
        dt = fmin(dt_max,  sqrt(h/max_vec(fx, fy, fz)));
        rnd = 0.01*max_vec(vx, vy, vz);
        jigle(rnd, vx, vy, vz);        
        visc_pair(mu, vx, vy, vz, /**/ fx, fy, fz);
        euler(-dt, vx, vy, vz, /**/ XX, YY, ZZ);
        euler( dt, fx, fy, fz, /**/ vx, vy, vz);

        if (i % 100 == 0) {
	  do {
              he_equiangulate(he, XX, YY, ZZ, /**/ &cnt);
              if (cnt) MSG("cnt : %d", cnt);
          } while (cnt > 0);
          restore_volume(restore, he, /**/ XX, YY, ZZ);
        }

        if (i % 100 == 0) {
            MSG("eng: %g", Energy(XX, YY, ZZ));
            A = he_area_tri(he, XX, YY, ZZ);
            V = he_volume_tri(he, XX, YY, ZZ);
            MSG("area, vol, rVolume: %g %g %g", A/A0, V/V0,
                reduced_volume(A, V));
            he_off_he_xyz_write(he, XX, YY, ZZ, "q.off");
            punto_fwrite(NV, queue, stdout);
            printf("\n");
        }

    }
}



int main(int __UNUSED argc, const char *v[]) {
    real a0, Kv;
    real *fx, *fy, *fz;
    real *vx, *vy, *vz;
    BendingParam bending_param;
    
    argv = v; argv++;
    arg();
    srand(time(NULL));

    y_ini("/dev/stdin", &he, &XX, &YY, &ZZ);
    NV = he_nv(he);
    NE = he_ne(he);
    NT = he_nt(he);
    MSG("N[VET: %d %d %d", NV, NE, NT);

    V0 = he_volume_tri(he, XX, YY, ZZ);    
    A0 = target_area(V0, rVolume);
    MSG("target_area: %g", A0);
    
    a0 = A0/NT;
    e0 = eq_tri_edg(a0);
    
    MSG("v0/volume(): %g", V0/he_volume_tri(he, XX, YY, ZZ));
    MSG("a0/area(): %g", A0/he_area_tri(he, XX, YY, ZZ));
    MSG("area, volume, edg: %g %g", A0, V0);

    restore_ini(V0, he, /**/ &restore);
    he_f_area_ini(a0,  Ka,  he, /**/ &f_area);
    he_f_garea_ini(A0,  Kga, he, /**/ &f_garea);    
    he_f_strain_ini(off, model, strain_param, /**/ &f_strain);

    bending_param.Kb = Kb;
    bending_param.C0 = C0;
    bending_param.Kad = Kad;
    bending_param.DA0D = DA0D;
    bending_ini(bending, bending_param, he, /**/ &f_bending);

    MALLOC(NV, &fx); MALLOC(NV, &fy); MALLOC(NV, &fz);
    MALLOC(NV, &vx); MALLOC(NV, &vy); MALLOC(NV, &vz);

    main0(vx, vy, vz, fx, fy, fz);

    FREE(fx); FREE(fy); FREE(fz);
    FREE(vx); FREE(vy); FREE(vz);

    restore_fin(restore);
    he_f_strain_fin(f_strain);
    bending_fin(f_bending);
    he_f_area_fin(f_area);
    he_f_garea_fin(f_garea);

    y_fin(he, XX, YY, ZZ);

    return 0;
}
