#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <real.h>
#include <he/err.h>
#include <he/util.h>
#include <he/x.h>
#include <he/vec.h>
#include <he/ddih.h>
#include <he/tri.h>
#include <he/dtri.h>
#include <he/memory.h>
#include <he/macro.h>
#include <he/punto.h>
#include <he/dedg.h>

#include <alg/x.h>
#include <alg/min.h>

#define FMT_IN   (XE_REAL_IN)

static real *lentheta, *AREA, *curva_mean;
static real Ka, Kv, Kb, Ke;
static const char **argv;
static const char *me = "min/gompper";

static int eq(const char *a, const char *b) { return util_eq(a, b); }
static int scl(/**/ real *p) {
    if (*argv == NULL) ER("not enough args");
    if (sscanf(*argv, FMT_IN, p) != 1)
        ER("not a number '%s'", *argv);
    argv++;
    return HE_OK;
}
static void usg() {
    fprintf(stderr, "%s Ka Kv Kb Ke < OFF\n", me);
    exit(0);
}

static void zero(int n, real *a) {
    int i;
    for (i = 0; i < n; i++) a[i] = 0;
}

static void get3(const real *xx, const real *yy, const real *zz,
                 int i, int j, int k,
                 /**/ real a[3], real b[3], real c[3]) {

    vec_get(i, xx, yy, zz, a);
    vec_get(j, xx, yy, zz, b);
    vec_get(k, xx, yy, zz, c);
}

static void get4(const real *xx, const real *yy, const real *zz,
                 int i, int j, int k, int l, /**/
                 real a[3], real b[3], real c[3], real d[3]) {
    vec_get(i, xx, yy, zz, a);
    vec_get(j, xx, yy, zz, b);
    vec_get(k, xx, yy, zz, c);
    vec_get(l, xx, yy, zz, d);
}

static real energy(const real *xx, const real *yy, const real *zz) {
    enum {X, Y, Z};
    int v, e, h, t;
    int i, j, k, l;
    real a[3], b[3], c[3], d[3], u[3];
    real cur, len, area0;
    real theta;
    real en, e0;
    real C0, H0;

    C0=0;
    H0=C0/2.0;
    zero(NV, curva_mean); zero(NV, AREA);

    for (e = 0; e < NE; e++) {
        h = hdg_edg(e);
        if ( bnd(h) ) continue;
        i = D0[e]; j = D1[e]; k = D2[e]; l = D3[e];
        get4(xx, yy, zz, i, j, k, l, /**/ a, b, c, d);
        theta = tri_dih(a, b, c, d);
        vec_minus(b, c, u);
        len = vec_abs(u);
        cur = len*theta/4;
        curva_mean[j] += cur;
        curva_mean[k] += cur;

    }

    for (t = 0; t < NT; t++) {
        i = T0[t]; j = T1[t]; k = T2[t];
        get3(xx, yy, zz, i, j, k, a, b, c);
        area0 = tri_area(a, b, c);
        AREA[i] += area0/3;
        AREA[j] += area0/3;
        AREA[k] += area0/3;

    }

    en  = 0;
    for (v = 0; v < NV; v++) {
        curva_mean[v] /= AREA[v];
        e0 = 2 * (curva_mean[v]-H0)*(curva_mean[v]-H0)*AREA[v];
        en  += e0;
    }
    return Kb*en;
}

static void force(const real *xx, const real *yy, const real *zz,
                  /**/ real *fx, real *fy, real *fz) {
    enum {X, Y, Z};

    int e, t;
    int i, j, k, l;
    real a[3], b[3], c[3], d[3];
    real da[3], db[3], dc[3], dd[3];
    real u[3];

    real lentheta0, area0, theta0, len0;
    real coef, c0, c1;

    zero(NV, lentheta); zero(NV, AREA);

    for (e = 0; e < NE; e++) {
        i = D0[e]; j = D1[e]; k = D2[e]; l = D3[e];
        get4(xx, yy, zz, i, j, k, l, /**/ a, b, c, d);
        vec_minus(c, b, u);
        lentheta0    = vec_abs(u) * tri_dih(a, b, c, d);
        lentheta[j] += lentheta0;
        lentheta[k] += lentheta0;
    }

    for (t = 0; t < NT; t++) {
        i = T0[t]; j = T1[t]; k = T2[t];
        get3(xx, yy, zz, i, j, k, a, b, c);
        area0 = tri_area(a, b, c)/3;
        AREA[i] += area0; AREA[j] += area0; AREA[k] += area0;
    }

    for (e = 0; e < NE; e++) {
        i = D0[e]; j = D1[e]; k = D2[e]; l = D3[e];
        get4(xx, yy, zz, i, j, k, l, /**/ a, b, c, d);
        theta0 = tri_dih(a, b, c, d);
        dedg_abs(c, b, /**/ dc, db);
        coef = -(lentheta[j]/AREA[j] + lentheta[k]/AREA[k])/4.0*theta0*Kb;
        vec_scalar_append(db, coef, j, fx, fy, fz);
        vec_scalar_append(dc, coef, k, fx, fy, fz);
    }

    for (e = 0; e < NE; e++) {
        i = D0[e]; j = D1[e]; k = D2[e]; l = D3[e];
        get4(xx, yy, zz, i, j, k, l, /**/ a, b, c, d);
        ddih_angle(a, b, c, d, /**/ da, db, dc, dd);
        vec_minus(c, b, u);
        len0 = vec_abs(u);
        coef = -(lentheta[j]/AREA[j] + lentheta[k]/AREA[k])/4.0*len0*Kb;

        vec_scalar_append(da, coef, i, fx, fy, fz);
        vec_scalar_append(db, coef, j, fx, fy, fz);
        vec_scalar_append(dc, coef, k, fx, fy, fz);
        vec_scalar_append(dd, coef, l, fx, fy, fz);
    }

    for (t = 0; t < NT; t++) {
        i = T0[t]; j = T1[t]; k = T2[t];
        get3(xx, yy, zz, i, j, k, a, b, c);
        dtri_area(a, b, c, /**/ da, db, dc);
        c0 = Kb/24;
        c1 = lentheta[i]*lentheta[i]/AREA[i]/AREA[i] * c0;
        vec_scalar_append(da, c1, i, fx, fy, fz);
    
        c1 = lentheta[j]*lentheta[j]/AREA[j]/AREA[j] * c0;
        vec_scalar_append(db, c1, j, fx, fy, fz);
    
        c1 = lentheta[k]*lentheta[k]/AREA[k]/AREA[k] * c0;
        vec_scalar_append(dc, c1, k, fx, fy, fz);
    }
}

real Energy(const real *x, const real *y, const real *z) {
    real a, v, e, b;
    a = f_area_energy(x, y, z);
    v = f_volume_energy(x, y, z);
    e = f_harmonic_energy(x, y, z);
    b = energy(x, y, z);
    return a + v + e + b;
}

void Force(const real *x, const real *y, const real *z,
           /**/ real *fx, real *fy, real *fz) {
    zero(NV, fx); zero(NV, fy); zero(NV, fz);
    f_area_force(x, y, z, /**/ fx, fy, fz);
    f_volume_force(x, y, z, /**/ fx, fy, fz);
    f_harmonic_force(x, y, z, /**/ fx, fy, fz);
    force(x, y, z, /**/ fx, fy, fz);
    MSG("%g %g %g", fx[0], fy[0], fz[0]);
    exit(0);
}

static void write(real *fx, real *fy, real *fz) {
    real *queue[] = {XX, YY, ZZ, fx, fy, fz, NULL};
    punto_fwrite(NV, queue, stdout);
}

static void force_ini() {
    MALLOC(NV, &lentheta);
    MALLOC(NV, &AREA);
}

static void force_fin() {
    FREE(lentheta);
    FREE(AREA);
}

static void energy_ini() {
    MALLOC(NV, &curva_mean);
}

static void energy_fin() { FREE(curva_mean); }

static void arg() {
    if (*argv != NULL && eq(*argv, "-h")) usg();
    scl(&Ka); scl(&Kv); scl(&Kb); scl(&Ke);
}

static real eq_tri_edg(real area) {
    /* area = sqrt(3)/4 * edg^2 */
    return 2*sqrt(area)/pow(3, 0.25);
}
static real area2volume(real area) { return 0.06064602170131934*pow(area, 1.5); }

int main(int __UNUSED argc, const char *v[]) {
    real A0, v0, vt, a0, e0;
    real *fx, *fy, *fz;
    int i;

    argv = v; argv++;
    arg();
    ini("/dev/stdin");

    A0 = area(); v0 = volume();
    a0 = A0/NT;   vt = area2volume(A0); e0 = eq_tri_edg(a0);

    force_ini();
    energy_ini();

    MALLOC(NV, &fx); MALLOC(NV, &fy); MALLOC(NV, &fz);
    zero(NV, fx); zero(NV, fy); zero(NV, fz);
    f_area_ini(a0, Ka);
    f_harmonic_ini(e0, Ke);
    real *queue[] = {XX, YY, ZZ, NULL};

    for (;;) {
        f_volume_ini(v0, Kv);
        min_ini(VECTOR_BFGS2);
        MSG("eng: %g", min_energy());
        for (i = 0; i < 400; i++) {
            min_position(/**/ XX, YY, ZZ);
            min_iterate();
        }
        MSG("a/a0 = %g v/v0 = %g", area()/a0/NT, volume()/v0);
        punto_fwrite(NV, queue, stdout);
        off_write(XX, YY, ZZ, "q.off");
        f_volume_fin();
        min_fin();
        v0 -= vt/10;
        if (v0 < vt) break;
    }

    force(XX, YY, ZZ, fx, fy, fz);
    write(fx, fy, fz);
    printf("%g\n", energy(XX, YY, ZZ));

    FREE(fx); FREE(fy); FREE(fz);

    f_harmonic_fin();
    f_area_fin();

    energy_fin();
    force_fin();
    fin();
    return HE_OK;
}
