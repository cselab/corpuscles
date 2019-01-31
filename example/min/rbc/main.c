#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <real.h>
#include <he/argv.h>
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
#include <he/bending.h>
#include <he/x.h>
#include <he/force.h>

#define FMT_IN   HE_REAL_IN

static const char *me = "min/rbc";
static const real pi = 3.141592653589793115997964;
static const real tolerA = 1.0e-3;

static Force *force;
static He *he;
static char fname[1024];

static real rVolume, Ka, Kga, Kv, Ke;
static real Kb, C0, Kad, DA0D;
static real nu, dt, kBT;
static int  end;
static int  freq;
static real A0, V0, e0;
static real eb, ea, ega, ev, ee, ebl, ebn, es;
static char **argv;
static char bscheme[1024];
static char dir[4049], fpath[4049];

static void zero(int n, real *a) {
    int i;
    for (i = 0; i < n; i++) a[i] = 0;
}

static void usg() {
    fprintf(stderr, "%s juelicher_xin rVolume Ka Kga Kv Ke Kb C0 Kad DA0D nu dt kBT", me);
    fprintf(stderr, "end: number of iterations\n");
    fprintf(stderr, "freq: frequency of output off files\n");
    fprintf(stderr, "< init.off > msg\n");
    exit(0);
}

static int mkdir0(const char *path) {
    int rc;
    char cmd[4048];
    sprintf(cmd, "mkdir -p \"%s\"", path);
    rc = system(cmd);
    if (rc != 0)
        ER("fail to create directory '%s'\n", path);
    return HE_OK;
}

static char *fullpath(const char *path) {
  strncpy(fpath, dir, 4048);
  strncat(fpath, "/", 4048);
  strncat(fpath, path, 4048);
  return fpath;
}

static real reduced_volume(real area, real volume) { return (6*sqrt(pi)*volume)/pow(area, 3.0/2); }

static int eq(const char *a, const char *b) { return util_eq(a, b); }
static int num(/**/ int *p) {
  if (*argv == NULL) {
    usg();
    ER("not enough args");
  }
  if (sscanf(*argv, "%d", p) != 1)
    ER("not a number '%s'", *argv);
  argv++;
  return HE_OK;
}
static int scl(/**/ real *p) {
  return argv_real(&argv, p);
}
static int str(/**/ char *p) {
  return argv_str(&argv, p);
}
static void arg() {
  
  if (*argv != NULL && eq(*argv, "-h")) usg();
  str(bscheme);
  scl(&rVolume);
  scl(&Ka);
  scl(&Kga);
  scl(&Kv);
  scl(&Ke);
  scl(&Kb);
  scl(&C0);
  scl(&Kad);
  scl(&DA0D);
  scl(&nu);
  scl(&dt);
  scl(&kBT);
  num(&end);
  num(&freq);
  str(dir);

}

real Energy0(const real *x, const real *y, const real *z) {
  
  real et;
  
  ea  = f_area_energy(x, y, z);
  ega = f_garea_energy(x, y, z);
  ev  = f_volume_energy(x, y, z);
  ee  = f_edg_sq_energy(x, y, z);
  eb  = f_bending_energy(x, y, z);
  
  ebl = f_bending_energy_bend();
  ebn = f_bending_energy_ad();
  
  es  = force_energy(force, he, x, y, z);

  et  = ea + ega + ev + ee + eb + es;
  
  return et;
}

void Force0(const real *x, const real *y, const real *z, /**/
	    real *fx, real *fy, real *fz) {
  
  zero(NV, fx); zero(NV, fy); zero(NV, fz);
  f_area_force(x, y, z, /**/ fx, fy, fz);
  f_garea_force(x, y, z, /**/ fx, fy, fz);
  f_volume_force(x, y, z, /**/ fx, fy, fz);
  f_edg_sq_force(x, y, z, /**/ fx, fy, fz);
  f_bending_force(x, y, z, /**/ fx, fy, fz);
  //force_force(force, he, x, y, z, /**/ fx, fy, fz);
  
}

void ForceArea(const real *x, const real *y, const real *z, /**/
	       real *fx, real *fy, real *fz) {
  
  zero(NV, fx); zero(NV, fy); zero(NV, fz);
  f_garea_force(x, y, z, /**/ fx, fy, fz);
  
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

static void visc_lang(real nu,
                      const real *vx, const real *vy, const real *vz, /*io*/
                      real *fx, real *fy, real *fz) {

  int i;
  for (i = 0; i < NV; i++) {
    fx[i] -= nu*vx[i];
    fy[i] -= nu*vy[i];
    fz[i] -= nu*vz[i];
  }
  
}

static int diff(int i, int j, const real *x, const real *y, const real *z, /**/ real e[3]) {
    real a[3], b[3];
    vec_get(i, x, y, z, a);
    vec_get(j, x, y, z, b);
    vec_minus(a, b, e);
    return HE_OK;
}

static void visc_pair(real nu, real kBT,
                      const real *vx, const real *vy, const real *vz, /*io*/
                      real *fx, real *fy, real *fz) {

  int e, i, j;
  real u[3], r[3], rn[3], p[3];
  real ra, sigma;

  sigma = sqrt(2.0*nu*kBT);

  for (e = 0; e < NE; e++) {
    i = D1[e]; j = D2[e];
    
    diff(i, j, XX, YY, ZZ, r);
    diff(i, j, vx, vy, vz, u);
    vec_norm(r, rn);
    vec_project(u, r, p);
    //note that the viscous force is in the negative direction
    //to be consistent with other forces
    vec_scalar_append(p, nu, i, fx, fy, fz);
    vec_scalar_append(p, -nu, j, fx, fy, fz);

    //note that the random force is in the negative direction
    //to be consistent with other forces
    //in practice, this direction does not matter
    ra  = rand()/(real)RAND_MAX - 0.5;
    ra *=sigma;
    vec_scalar_append(rn, -ra, i, fx, fy, fz);
    vec_scalar_append(rn, ra, j, fx, fy, fz);
    
  }
  
  
}

static real Kinetic0(real *vx, real *vy, real *vz) {

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
  int cnt, i, j;
  real rnd;
  real A, V, Vr;
  real errA;
  real et, ep, ek;
  real *queue[] = {XX, YY, ZZ, NULL};
  int nsub;
  char file[4048];
  char filemsg[4048]="stat";
  FILE *fm;


  mkdir0(dir);
  if ((fm = fopen(fullpath(filemsg), "w")) == NULL)
    ER("fail to open '%s'", filemsg);
  fclose(fm);
  
  int freq_screen;

  freq_screen=100;
  
  nsub = 100;
  zero(NV, vx); zero(NV, vy); zero(NV, vz);

  Force0(XX, YY, ZZ, /**/ fx, fy, fz);
  visc_pair(nu, kBT, vx, vy, vz, /**/ fx, fy, fz);

  for (i = 0; i <= end; i++) {
    
    euler(-dt/2.0, fx, fy, fz, /**/ vx, vy, vz);
    euler(dt, vx, vy, vz, /**/ XX, YY, ZZ);
    
    Force0(XX, YY, ZZ, /**/ fx, fy, fz);
    visc_pair(nu, kBT, vx, vy, vz, /**/ fx, fy, fz);

    euler(-dt/2.0, fx, fy, fz, /**/ vx, vy, vz);

    j = 0;
    A  = area();
    errA = (A-A0)/A0;
    if (errA<0) {
      errA=-errA;
    }
    
    while ( j < nsub && errA > tolerA ) {
      
      euler(-dt/2.0, fx, fy, fz, /**/ vx, vy, vz);
      euler(dt, vx, vy, vz, /**/ XX, YY, ZZ);

      ForceArea(XX, YY, ZZ, /**/ fx, fy, fz);
      visc_pair(nu, kBT, vx, vy, vz, /**/ fx, fy, fz);

      euler(-dt/2.0, fx, fy, fz, /**/ vx, vy, vz);
      
      j++;
      A  = area();
      errA = (A-A0)/A0;
      if (errA<0) {
	errA=-errA;
      }
      
    }
    
    if ( i % freq_screen == 0 ) {
      
      if ( i > 0 ) {
	j = 0;
	do {
	  equiangulate(&cnt);
	  MSG("cnt : %d", cnt);
	  j++;
	} while (cnt > 0 && j < 10);
      }
      
      ep = Energy0(XX, YY, ZZ);
      ek = Kinetic0(vx, vy, vz);
      et = ep + ek;
      
      A = area(); V = volume(); Vr=reduced_volume(A,V);
      MSG("dt: %g", dt);
      MSG("eng: %g %g %g %g %g %g %g %g %g %g", et, eb, ea, ega, ev, ek, ee, ebl, ebn, es); 
      MSG("A/A0, V/V0, Vr: %g %g %g", A/A0, V/V0, Vr);

      fm = fopen(fullpath(filemsg), "a");
      static int First = 1;
      if (First) {
	fputs("A/A0 V/V0 Vr et eb ea ega ev ek ee ebl ebn es\n", fm);
	First = 0;
      }
      fprintf(fm, "%g %g %g %g %g %g %g %g %g %g %g %g %g \n", A/A0, V/V0, Vr, et, eb, ea, ega, ev, ek, ee, ebl, ebn, es);
      fclose(fm);

    }
    
    if ( i % freq == 0 ) {
      
      sprintf(file, "%07d.off", i);
      off_write(XX, YY, ZZ, file);
      
    }
  }
}

static real sph_volume(real area) { return 0.09403159725795977*pow(area, 1.5); }
static real target_volume(real area, real v) { return v*sph_volume(area); }
static real eq_tri_edg(real area) { return 2*sqrt(area)/pow(3, 0.25); }


int main(int __UNUSED argc, char *v[]) {
  real a0;
  real *fx, *fy, *fz;
  real *vx, *vy, *vz;
  real A, V, Vr;
  BendingParam bending_param;
  
  argv = v; argv++;
  arg();
  srand(time(NULL));

  ini("/dev/stdin");

  x_he(&he);
  str(fname);
  force_argv(fname, &argv, he,  &force);


  A0 = area();
  a0 = A0/NT;
  V0 = target_volume(A0, rVolume);
  e0 = eq_tri_edg(a0);

  A = A0;
  V = volume();
  Vr= reduced_volume(A, V);
  
  MSG("v              : %g", rVolume);
  MSG("bending scheme : %s", bscheme);
  MSG("strain scheme  : %s", fname);
 
  MSG("Targeted Area, Volume: %g %g", A0, V0);
  MSG("V/V0: %g", V/V0);
  MSG("A/A0: %g", A/A0);
  MSG("Vr  : %g", Vr);
  
  f_area_ini(a0,  Ka);
  f_garea_ini(A0, Kga);
  f_volume_ini(V0, Kv);
  f_edg_sq_ini(Ke);
  
  bending_param.Kb = Kb;
  bending_param.C0 = C0;
  bending_param.Kad = Kad;
  bending_param.DA0D = DA0D;
  f_bending_ini(bscheme, bending_param);
  
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
