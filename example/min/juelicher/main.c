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

static real *lentheta, *AREA;
static real *curva_mean, *ENERGY;

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
  real a[3], b[3], c[3], d[3], u[3], coord[3];
  real cur, len, len2, area0;
  real theta, phi;
  real area_tot_tri, area_tot_split;
  real energy_tot;
  real C0, H0;

  C0=0;
  H0=C0/2.0;
  zero(NV, curva_mean); zero(NV, ENERGY); zero(NV, AREA);

  for (e = 0; e < NE; e++) {

    h = hdg_edg(e);

    if ( bnd(h) ) continue;

    i = D0[e]; j = D1[e]; k = D2[e]; l = D3[e];

    get4(xx, yy, zz, i, j, k, l, /**/ a, b, c, d);

    theta = tri_dih(a, b, c, d);

    vec_minus(b, c, u);
    len2 = vec_dot(u, u);
    len  = sqrt(len2);

    cur = len*theta/4;
    curva_mean[j] += cur;
    curva_mean[k] += cur;

  }

  area_tot_tri = 0;

  for (t = 0; t < NT; t++) {

    i = T0[t]; j = T1[t]; k = T2[t];

    get3(xx, yy, zz, i, j, k, a, b, c);
    area0 = tri_area(a, b, c);

    AREA[i] += area0/3;
    AREA[j] += area0/3;
    AREA[k] += area0/3;

    area_tot_tri += area0;

  }

  area_tot_split = 0;
  energy_tot     = 0;

  for (v = 0; v < NV; v++) {

    curva_mean[v] /= AREA[v];
    ENERGY[v] = 2 * (curva_mean[v]-H0)*(curva_mean[v]-H0)*AREA[v];
    /*for verification*/
    area_tot_split += AREA[v];
    energy_tot     += ENERGY[v];
    vec_get(v, XX, YY, ZZ, coord);

    phi = TH[v];
    if ( phi > pi / 2) {
      phi = pi - phi;
    }
  }
  return Kb*energy_tot;
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
  real coef;

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
    area0 = tri_area(a, b, c);
    AREA[i] += area0/3;
    AREA[j] += area0/3;
    AREA[k] += area0/3;
  }


  for (e = 0; e < NE; e++) {
    i = D0[e]; j = D1[e]; k = D2[e]; l = D3[e];
    get4(xx, yy, zz, i, j, k, l, /**/ a, b, c, d);
    theta0 = tri_dih(a, b, c, d);

    dedg_abs(c, b, /**/ dc, db);

    coef = -(lentheta[j]/AREA[j]/4.0) * theta0 * Kb;
    vec_scalar_append(db, coef, j, fx, fy, fz);
    vec_scalar_append(dc, coef, k, fx, fy, fz);

    coef = -(lentheta[k]/AREA[k]/4.0) * theta0;
    vec_scalar_append(db, coef, j, fx, fy, fz);
    vec_scalar_append(dc, coef, k, fx, fy, fz);
  }


  for (e = 0; e < NE; e++) {
    i = D0[e]; j = D1[e]; k = D2[e]; l = D3[e];
    get4(xx, yy, zz, i, j, k, l, /**/ a, b, c, d);
    ddih_angle(a, b, c, d, /**/ da, db, dc, dd);
    vec_minus(c, b, u);
    len0 = vec_dot(u, u);
    len0 = sqrt(len0);

    coef =
        -(lentheta[j]/AREA[j]/4.0) * len0 * Kb
        -(lentheta[k]/AREA[k]/4.0) * len0 * Kb;

    vec_scalar_append(da, coef, i, fx, fy, fz);
    vec_scalar_append(db, coef, j, fx, fy, fz);
    vec_scalar_append(dc, coef, k, fx, fy, fz);
    vec_scalar_append(dd, coef, l, fx, fy, fz);
  }

  for (t = 0; t < NT; t++) {
    i = T0[t]; j = T1[t]; k = T2[t];
    get3(xx, yy, zz, i, j, k, a, b, c);
    dtri_area(a, b, c, /**/ da, db, dc);
    coef = (lentheta[i]*lentheta[i]/8.0/AREA[i]/AREA[i]);
    vec_scalar_append(da, Kb*coef/3.0, i, fx, fy, fz);
    coef = (lentheta[j]*lentheta[j]/8.0/AREA[j]/AREA[j]);
    vec_scalar_append(db, Kb*coef/3.0, j, fx, fy, fz);
    coef = (lentheta[k]*lentheta[k]/8.0/AREA[k]/AREA[k]);
    vec_scalar_append(dc, Kb*coef/3.0, k, fx, fy, fz);
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
    MALLOC(NV, &ENERGY);
}

static void energy_fin() {
    FREE(curva_mean);
    FREE(ENERGY);
}

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
    real A0, v0, a0, e0;
    real *fx, *fy, *fz;
    int i;

    argv = v; argv++;
    arg();
    ini("/dev/stdin");

    A0 = area();
    a0 = A0/NT;   v0 = area2volume(A0); e0 = eq_tri_edg(a0);
    MSG("v0/volume(): %g", v0/volume());
    MSG("area, volume, edg: %g %g %g", a0, v0, e0);

    force_ini();
    energy_ini();

    MALLOC(NV, &fx); MALLOC(NV, &fy); MALLOC(NV, &fz);
    zero(NV, fx); zero(NV, fy); zero(NV, fz);

    f_volume_ini(v0, Kv);
    f_area_ini(a0, Ka);
    f_harmonic_ini(e0, Ke);
    min_ini(VECTOR_BFGS2);
    real *queue[] = {XX, YY, ZZ, NULL};

    for (i = 0; i < 0; i++) {
        min_position(/**/ XX, YY, ZZ);
        if (i % 100 == 0) {
            punto_fwrite(NV, queue, stdout);
            printf("\n");
            MSG("eng: %g", min_energy());
            off_write(XX, YY, ZZ, "q.off");
            MSG("dump: q.off");
        }
        min_iterate();
    }

    force(XX, YY, ZZ, fx, fy, fz);
    write(/*i*/ fx, fy, fz);
    printf("%g\n", energy(XX, YY, ZZ));

    FREE(fx); FREE(fy); FREE(fz);

    min_fin();
    f_harmonic_fin();
    f_volume_fin();
    f_area_fin();

    energy_fin();
    force_fin();
    fin();
    return HE_OK;
}
