#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include <real.h>

#include <he/err.h>
#include <he/punto.h>
#include <he/vec.h>
#include <he/macro.h>
#include <he/util.h>
#include <he/memory.h>
#include <he/tri.h>
#include <he/dedg.h>
#include <he/ddih.h>
#include <he/dtri.h>
#include <he/x.h>
#include <alg/x.h>
#include <alg/min.h>
#include <stdlib.h>

static real Kb;
static void zero(int n, real *a) {
    int i;
    for (i = 0; i < n; i++) a[i] = 0;
}


#define FMT_IN   XE_REAL_IN

static real rVolume, Ka, Kga, Kv, Ke;
static real A0, V0;
static const char **argv;
static char bending[4048];
static const char *me = "min/visc";

enum {KANTOR, GOMPPER, JUELICHER, MEYER};
static int btype;
static int f_bending_ini(const char *bending, real K) {
    real Da0, H0, Kad;
    Kad = 1e-6;
    Da0 = 0;

    if (util_eq(bending, "kantor")) {
        btype = KANTOR;
        f_kantor_ini(K);
    }
    else if (util_eq(bending, "gompper")) {
        btype = GOMPPER;
        f_gompper_ini(K);
    } else if (util_eq(bending, "juelicher")) {
        btype = JUELICHER;
        f_juelicher_ini(K, Kad, Da0);
    } else if (util_eq(bending, "meyer")) {
        btype = MEYER;
        f_meyer_ini(K, Kad, Da0);
    } else
        ER("unknown bending type: %s", bending);
    return HE_OK;
}

static int f_bending_fin() {
    switch (btype) {
    case KANTOR: return f_kantor_fin();
    case GOMPPER: return f_gompper_fin();
    case JUELICHER: return f_juelicher_fin();
    case MEYER: return f_meyer_fin();
    }
    ER("unknown btype: %d", btype);
}

static real f_bending_energy(const real *x, const real *y, const real *z) {
    switch (btype) {
    case KANTOR: return f_kantor_energy(x, y, z);
    case GOMPPER: return f_gompper_energy(x, y, z);
    case JUELICHER: return f_juelicher_energy(x, y, z);
    case MEYER: return f_meyer_energy(x, y, z);
    }
    ER("unknown btype: %d", btype);
}

static real f_bending_force(const real *x, const real *y, const real *z,
                            /**/ real *fx, real *fy, real *fz) {
    switch (btype) {
    case KANTOR: return f_kantor_force(x, y, z, /**/ fx, fy, fz);
    case GOMPPER: return f_gompper_force(x, y, z, /**/ fx, fy, fz);
    case JUELICHER: return f_juelicher_force(x, y, z, /**/ fx, fy, fz);
    case MEYER: return f_meyer_force(x, y, z, /**/ fx, fy, fz);
    }
    ER("unknown btype: %d", btype);
}

static void usg() {
    fprintf(stderr, "%s kantor/gompper/juelicher/meyer rVolume Ka Kga Kv Kb Ke < OFF > PUNTO\n", me);
    exit(0);
}

int eq(const char *a, const char *b) { return util_eq(a, b); }
int scl(/**/ real *p) {
    if (*argv == NULL) ER("not enough args");
    if (sscanf(*argv, FMT_IN, p) != 1)
        ER("not a number '%s'", *argv);
    argv++;
    return HE_OK;
}
int str(/**/ char *p) {
    if (*argv == NULL) ER("not enough args");
    strncpy(p, *argv, 4048);
    argv++;
    return HE_OK;
}
static void arg() {
    if (*argv != NULL && eq(*argv, "-h")) usg();
    str(bending);
    scl(&rVolume); scl(&Ka); scl(&Kga); scl(&Kv); scl(&Kb); scl(&Ke);
}

real Energy(const real *x, const real *y, const real *z) {
    real a, ga, v, e, b;
    a = f_area_energy(x, y, z);
    ga = f_garea_energy(x, y, z);
    v = f_volume_energy(x, y, z);
    e = f_edg_sq_energy(x, y, z);
    b = f_bending_energy(x, y, z);
    return a + ga + v + e + b;
}

void Force(const real *x, const real *y, const real *z, /**/
           real *fx, real *fy, real *fz) {
    zero(NV, fx); zero(NV, fy); zero(NV, fz);
    f_area_force(x, y, z, /**/ fx, fy, fz);
    f_garea_force(x, y, z, /**/ fx, fy, fz);
    f_volume_force(x, y, z, /**/ fx, fy, fz);
    f_edg_sq_force(x, y, z, /**/ fx, fy, fz);
    f_bending_force(x, y, z, /**/ fx, fy, fz);
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

static void visc_lang(real mu,
                      const real *vx, const real *vy, const real *vz, /*io*/
                      real *fx, real *fy, real *fz) {
    int i;
    for (i = 0; i < NV; i++) {
        fx[i] -= mu*vx[i];
        fy[i] -= mu*vy[i];
        fz[i] -= mu*vz[i];
    }
}

static void visc_pair(real mu,
                      const real *vx, const real *vy, const real *vz, /*io*/
                      real *fx, real *fy, real *fz) {
    int e, i, j;
    real a[3], b[3], u[3], u0;
    for (e = 0; e < NE; e++) {
        i = D1[e]; j = D2[e];
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

static void main0(real *vx, real *vy, real *vz,
                  real *fx, real *fy, real *fz) {
    int cnt, i;
    real dt, mu, rnd;
    real *queue[] = {XX, YY, ZZ, NULL};
    i = 0;
    dt = 1e-3;
    mu = 20.0;
    rnd = 0.01;

    zero(NV, vx); zero(NV, vy); zero(NV, vz);
    for (;;) {
        i++;
        Force(XX, YY, ZZ, /**/ fx, fy, fz);
//        visc_lang(mu, vx, vy, vz, /**/
//                  fx, fy, fz);
        visc_pair(mu, vx, vy, vz, /**/ fx, fy, fz);
        jigle(rnd, vx, vy, vz);
        euler(-dt, vx, vy, vz, /**/ XX, YY, ZZ);
        euler( dt, fx, fy, fz, /**/ vx, vy, vz);
        if (i % 1500 == 0) {
            do {
                //equiangulate(&cnt);
                cnt = 0;
                MSG("cnt : %d", cnt);
            } while (cnt > 0);
            punto_fwrite(NV, queue, stdout);
            printf("\n");
            MSG("eng: %g %g", Energy(XX, YY, ZZ), Kin(vx, vy, vz));
            MSG("area/vol: %g %g", area()/A0, volume()/V0);
            off_write(XX, YY, ZZ, "q.off");
        }
    }
}

static real sph_volume(real area) { return 0.09403159725795977*pow(area, 1.5); }
static real target_volume(real area, real v) { return v*sph_volume(area); }

int main(int __UNUSED argc, const char *v[]) {
    real a0;
    real *fx, *fy, *fz;
    real *vx, *vy, *vz;
    argv = v; argv++;
    arg();
    srand(time(NULL));

    ini("/dev/stdin");
    A0 = area();
    a0 = A0/NT;
    V0 = target_volume(A0, rVolume);
    MSG("v0/volume(): %g", V0/volume());
    MSG("area, volume, edg: %g %g %g", A0, V0);

    f_area_ini(a0,  Ka);
    f_garea_ini(A0, Kga);
    f_volume_ini(V0, Kv);
    f_edg_sq_ini(Ke);
    f_bending_ini(bending, Kb);

    MALLOC(NV, &fx); MALLOC(NV, &fy); MALLOC(NV, &fz);
    MALLOC(NV, &vx); MALLOC(NV, &vy); MALLOC(NV, &vz);

    main0(vx, vy, vz, fx, fy, fz);

    FREE(fx); FREE(fy); FREE(fz);
    FREE(vx); FREE(vy); FREE(vz);

    f_bending_fin();
    f_edg_sq_fin();
    f_volume_fin();
    f_area_fin();
    f_garea_fin();
    fin();

    return 0;
}
