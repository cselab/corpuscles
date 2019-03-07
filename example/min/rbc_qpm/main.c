//**********************************************************
//This is a minimization procedure using
//steepest descent + viscosity and one may
//intepret this as a version of conjugate gradient method.
//
//The two constratints on area and volume are implemented
//as quadratic penalty meothod (QPM) so that the constrained
//optimization is converted to an unconstratined one.
//
//We note that the force is currently in the opposite direction
//due to an obsolete convention.
//Therefore, the negative force is employed to update velocity.
//**********************************************************
#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <real.h>
#include <co/argv.h>
#include <co/err.h>
#include <co/punto.h>
#include <co/vec.h>
#include <co/macro.h>
#include <co/util.h>
#include <co/memory.h>
#include <co/tri.h>
#include <co/dedg.h>
#include <co/ddih.h>
#include <co/dtri.h>
#include <co/bending.h>
#include <co/x.h>
#include <co/force.h>

#define FMT_IN   CO_REAL_IN

//This Macro defines if we need adaptive time step
#define DT_ADPTIVE

static const char *me = "min/rbc_qpm";
static const real pi = 3.141592653589793115997964;

static He *he;

static Force *force_a;
static Force *force_v;
static Force *force_b;
static Force *force_s;


static char fname_b[1024];
static char fname_s[1024];
static char **argv;
static char bscheme[1024];
static char dir[4049], fpath[4049];

static real R, rho, v;
static real Kc;
static real Kb, C0, Kad, DA0D, D;
static real xi, dt_in, kBT;
static int  end;
static int  freq, freq_stat;

static real mass;
static real A0, V0, e0;
static real eb, ega, ev, ebl, ebn, es;

static void zero(int n, real *a) {
    int i;
    for (i = 0; i < n; i++) a[i] = 0;
}

static void usg(void) {
    fprintf(stderr, "%s juelicher_xin R rho v Kc Kb C0 Kad DA0D xi dt_in kBT", me);
    fprintf(stderr, "end: total number of steps\n");
    fprintf(stderr, "freq: frequency in steps to output off files\n");
    fprintf(stderr, "freq: frequency in steps to output statistics\n");
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
    return CO_OK;
}

static char *fullpath(const char *path) {
  strncpy(fpath, dir, 4048);
  strncat(fpath, "/", 4048);
  strncat(fpath, path, 4048);
  return fpath;
}

static real reduced_volume(real area, real volume) {
  return (6*sqrt(pi)*volume)/pow(area, 3.0/2);
}

static int eq(const char *a, const char *b) {
  return util_eq(a, b);
}

static int num(/**/ int *p) {
  if (*argv == NULL) {
    usg();
    ER("not enough args");
  }
  if (sscanf(*argv, "%d", p) != 1)
    ER("not a number '%s'", *argv);
  argv++;
  return CO_OK;
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
  scl(&R);
  scl(&rho);
  scl(&v);
  scl(&Kc);
  scl(&Kb);
  scl(&C0);
  scl(&Kad);
  scl(&DA0D);
  scl(&D);
  scl(&xi);
  scl(&dt_in);
  scl(&kBT);
  num(&end);
  num(&freq);
  num(&freq_stat);
  str(dir);

}

real Energy0(const real *x, const real *y, const real *z) {
  
  real et;
  
  ega = f_garea_energy(x, y, z);
  ev  = f_volume_energy(x, y, z);
  eb  = f_bending_energy(x, y, z);
  
  ebl = f_bending_energy_bend();
  ebn = f_bending_energy_ad();
  
  es  = force_energy(force_s, he, x, y, z);

  et  = ega + ev + eb + es;
  
  return et;
}

void Force0(const real *x, const real *y, const real *z, /**/
	    real *fx, real *fy, real *fz) {
  
  zero(NV, fx); zero(NV, fy); zero(NV, fz);
  f_garea_force(x, y, z, /**/ fx, fy, fz);
  f_volume_force(x, y, z, /**/ fx, fy, fz);
  f_bending_force(x, y, z, /**/ fx, fy, fz);
  force_force(force_s, he, x, y, z, /**/ fx, fy, fz);
  
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

static int diff(int i, int j, const real *x, const real *y, const real *z, /**/ real e[3]) {
    real a[3], b[3];
    vec_get(i, x, y, z, a);
    vec_get(j, x, y, z, b);
    vec_minus(a, b, e);
    return CO_OK;
}

static void visc_pair(real xi, 
                      const real *vx, const real *vy, const real *vz, /*io*/
                      real *fx, real *fy, real *fz) {

  int e, i, j;
  real u[3], r[3], rn[3], p[3];
  
  for (e = 0; e < NE; e++) {
    i = D1[e]; j = D2[e];
    
    diff(i, j, XX, YY, ZZ, r);
    diff(i, j, vx, vy, vz, u);
    vec_norm(r, rn);
    vec_project(u, r, p);
    //note that the viscous force is in the negative direction
    //to be consistent with other forces
    vec_scalar_append(p, xi, i, fx, fy, fz);
    vec_scalar_append(p, -xi, j, fx, fy, fz);
    
  }
    
}

static void rand_pair(real xi, real kBT, real dt,
                      const real *vx, const real *vy, const real *vz, /*io*/
                      real *fx, real *fy, real *fz) {

  int e, i, j;
  real u[3], r[3], rn[3], p[3];
  real ra, sigma, coef;

  coef = sqrt(12.0);
  sigma = sqrt(2.0*xi*kBT/dt);
  
  for (e = 0; e < NE; e++) {
    i = D1[e]; j = D2[e];
    
    diff(i, j, XX, YY, ZZ, r);
    diff(i, j, vx, vy, vz, u);
    vec_norm(r, rn);

    ra  = coef*(rand()/(real)RAND_MAX - 0.5);
    ra *=sigma;
    vec_scalar_append(rn, -ra, i, fx, fy, fz);
    vec_scalar_append(rn, ra, j, fx, fy, fz);
    
  }
    
}

static real Kinetic0(real *vx, real *vy, real *vz, real m) {

  int i;
  real s;
  s = 0;
  for (i = 0; i < NV; i++) {
    s += vx[i]*vx[i];
    s += vy[i]*vy[i];
    s += vz[i]*vz[i];
  }
  return m*s/2.0;
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

static int main0(real *vx, real *vy, real *vz,
		 real *fx, real *fy, real *fz) {
  int cnt, i, j;
  real rnd;
  real A, V, Vr;
  real errA;
  real et, ep, ek;
  real *queue[] = {XX, YY, ZZ, NULL};
  char file[4048];
  char filemsg[4048]="stat.dat";
  FILE *fm;
  real dt, h, time;

  time = 0.0;
  dt   = dt_in;
  
  mkdir0(dir);

  MSG("mass: %g", mass);

  if ( (fm = fopen(fullpath(filemsg), "w") ) == NULL) {
    ER("fail to open '%s'", filemsg);
  }
  
  fclose(fm);
  
  zero(NV, vx); zero(NV, vy); zero(NV, vz);

  Force0(XX, YY, ZZ, /**/ fx, fy, fz);
  
  visc_pair(xi, vx, vy, vz, /**/ fx, fy, fz);

#ifdef DT_ADPTIVE
  h  = 0.25*0.25*e0;
  dt = fmin(dt_in, sqrt(h*mass/max_vec(fx, fy, fz)));  
#endif

  if ( kBT > 0 ) {
    rand_pair(xi, kBT, dt, vx, vy, vz, /**/ fx, fy, fz);
  }
  
  
  for (i = 0; i <= end; i++) {
    
    if ( i % freq == 0 ) {
      
      sprintf(file, "%07d.off", i);
      off_write(XX, YY, ZZ, file);
      
    }
    
    if ( i % freq_stat == 0 ) {
      
      ep = Energy0(XX, YY, ZZ);
      ek = Kinetic0(vx, vy, vz, mass);
      et = ep + ek;
      
      A = area();
      V = volume();
      Vr=reduced_volume(A,V);
      MSG("dt, s, t: %g %d %g", dt, i, time);
      MSG("A/A0, V/V0, v: %g %g %g", A/A0, V/V0, Vr);
      MSG("et,  ega,  ev,  ek,  eb, ebl, ebn, es");
      MSG("%g %g %g %g %g %g %g %g", et, ega, ev, ek, eb, ebl, ebn, es); 
      
      fm = fopen(fullpath(filemsg), "a");
      static int First = 1;
      if (First) {
	fputs("dt s t A/A0 V/V0 v et ega ev ek eb ebl ebn es\n", fm);
	First = 0;
      }
      fprintf(fm, "%g %d %g %g %g %g %g %g %g %g %g %g %g %g \n", dt, i, time, A/A0, V/V0, Vr, et, ega, ev, ek, eb, ebl, ebn, es);
      fclose(fm);
      
    }//i%freq_screen
    
    euler(-dt/2.0/mass, fx, fy, fz, /**/ vx, vy, vz);
    euler(dt, vx, vy, vz, /**/ XX, YY, ZZ);

    time += dt;
    
    Force0(XX, YY, ZZ, /**/ fx, fy, fz);
    
    visc_pair(xi, vx, vy, vz, /**/ fx, fy, fz);
    
#ifdef DT_ADPTIVE
    dt = fmin(dt_in, sqrt(h*mass/max_vec(fx, fy, fz)));
#endif

    if ( kBT > 0 ) {
      rand_pair(xi, kBT, dt, vx, vy, vz, /**/ fx, fy, fz);
    }
    
    euler(-dt/2.0/mass, fx, fy, fz, /**/ vx, vy, vz);
    
  }//i=0;i<end
  
  return 1;
}


static real sph_volume(real area) {
  return 0.09403159725795977*pow(area, 1.5);
}
static real target_volume(real area, real v) {
  return v*sph_volume(area);
}
static real eq_tri_edg(real area) {
  return 2*sqrt(area)/pow(3, 0.25);
}

int main(int __UNUSED argc, char *v[]) {

  real *fx, *fy, *fz;
  real *vx, *vy, *vz;
  real A, V, Vr;
  real a0;
  BendingParam bending_param;
  
  argv = v;
  argv++;
  arg();
  srand(time(NULL));

  ini("/dev/stdin");

  x_he(&he);

  force_argv("darea", &argv, he,  &force_a);
  force_argv("dvolume", &argv, he,  &force_v);

  str(fname_b);
  force_argv(fname_b, &argv, he,  &force_b);

  str(fname_s);
  force_argv(fname_s, &argv, he,  &force_s);

  A0 = area();
  V0 = target_volume(A0, Vr);
  a0 = A0/NT;
  e0 = eq_tri_edg(a0);

  A  = A0;
  V  = volume();
  Vr = reduced_volume(A, V);

  mass = 4.0*pi*R*R*D*rho/NV;

  MSG("v              : %g", v);
  MSG("bending scheme : %s", bscheme);
  MSG("strain scheme  : %s", fname_s);
 
  MSG("Targeted Area, Volume: %g %g", A0, V0);
  MSG("V/V0: %g", V/V0);
  MSG("A/A0: %g", A/A0);
  MSG("v   : %g", v);
  MSG("R   : %g", R);
  MSG("rho : %g", rho);
  MSG("mass: %g", mass);
  MSG("Nt, Ne, Nv: %d %d %d", NT, NE, NV);
  MSG("Kc  : %g", Kc);
  MSG("Kb  : %g", Kb);
  MSG("C0  : %g", C0);
  MSG("Kad : %g", Kad);
  MSG("DA0D: %g", DA0D);
  MSG("D   : %g", D);
  MSG("xi  : %g", xi);
  MSG("kBT : %g", kBT);
  
  f_dgarea_ini(A0, Kc);
  f_dvolume_ini(V0, Kc);
  
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
  f_volume_fin();
  f_garea_fin();
  fin();
  
  return 0;
}
