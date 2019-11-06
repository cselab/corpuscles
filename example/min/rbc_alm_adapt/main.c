//**********************************************************
//corpuslces/exmample/min/rbc_qpm
//
//This is a minimization procedure using
//steepest descent + viscosity and one may
//intepret this as a version of conjugate gradient method.
//
//The two constratints on area and volume are implemented
//as quadratic penalty meothod (QPM) so that the constrained
//optimization is converted to an unconstratined one.
//
//
//We note that the force is currently in the opposite direction
//due to an obsolete convention.
//Therefore, the negative force is employed to update velocity.
//**********************************************************
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <real.h>

#include <co/force.h>
#include <co/argv.h>
#include <co/err.h>
#include <co/vec.h>
#include <co/macro.h>
#include <co/util.h>
#include <co/memory.h>
#include <co/he.h>
#include <co/y.h>
#include <co/off.h>
#include <co/area.h>
#include <co/volume.h>
#include <co/f/juelicher_xin.h>

//This Macro defines if we need adaptive time step
#define DT_ADPTIVE

static const char *me   = "min/rbc_qpm_adapt";
static const real pi    = 3.141592653589793115997964;
static const real toler = 1.0e-3;

static char **argv;

static He *he;

static Force *force_area;
static Force *force_volume;
static Force *force_bend;
static Force *force_strain;

static char fname_bend[1024];
static char fname_strain[1024];

static char dir[4049], fpath[4049];

static real R, rho, v;
static real Kc;
static int  freqk;
static real coefk;
static real Kb, C0, Kad, DA0D, D;
static real xi, dt_in, kBT;
static int  end;
static int  freqo, freqs;

static real mass;
static real A0, V0, e0;
static real A, V, vc;
static real eb, ea, ev, ebl, ebn, es;

static real *x, *y, *z;
static int Nt, Ne, Nv;
static int *D0, *D1, *D2, *D3;

static real ca, cv;

static real lama0, lama;
static real lamv0, lamv;

static void zero(int n, real *a) {
    int i;
    for (i = 0; i < n; i++) a[i] = 0;
}

static void usg(void) {

    fprintf(stderr, "%s juelicher_xin R rho v Kc Kb C0 Kad DA0D xi dt_in kBT", me);
    fprintf(stderr, "end: total number of steps\n");
    fprintf(stderr, "freqo: frequency in steps to output off files\n");
    fprintf(stderr, "freqs: frequency in steps to output statistics\n");
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

static void init() {
    //Print out help message
    if (*argv != NULL && eq(*argv, "-h")) {
        usg();
    }

    //reading input parameters
    scl(&R);
    scl(&rho);
    scl(&v);

    scl(&Kc);
    num(&freqk);
    scl(&coefk);
    
    str(fname_bend);
    force_argv(fname_bend, &argv, he, &force_bend);

    scl(&D);
    scl(&xi);
    scl(&dt_in);
    scl(&kBT);

    str(fname_strain);
    force_argv(fname_strain, &argv, he, &force_strain);

    num(&end);
    num(&freqo);
    num(&freqs);

    force_argv("darea",   &argv, he, &force_area);
    force_argv("dvolume", &argv, he, &force_volume);

    lama0 = 0.0;
    lamv0 = 0.0;
    
    lama  = lama0;
    lamv  = lamv0;

}
real ConstraintArea(const real *x, const real *y, const real *z) {
  
  real c;

  A  = force_energy(force_area, he, x, y, z);
  c  = (A/A0-1);
  ca = c;
  
  return c;
}
real ConstraintVolume(const real *x, const real *y, const real *z) {
  
  real c;

  V  = force_energy(force_volume, he, x, y, z);
  c  = (V/V0-1);
  cv = c;
  
  return c;
}

real EnergyArea(const real *x, const real *y, const real *z) {

  real c, e1, e2, e;

  c   = ConstraintArea(x, y, z);
  e1  = c*c;
  e1 *= Kc/2.0;
  
  e2 = -lama*c;

  e = e1 + e2;

  return e;
    
}
real EnergyVolume(const real *x, const real *y, const real *z) {

  real c, e1, e2, e;

  c   = ConstraintVolume(x, y, z);
  e1  = c*c;
  e1 *= Kc/2.0;

  e2 = -lamv*c;

  e = e1 + e2;
    
  return e;

}

real Energy0(const real *x, const real *y, const real *z) {

    real et;
    
    ea  = EnergyArea(x, y, z);
    ev  = EnergyVolume(x, y, z);

    eb  = force_energy(force_bend, he, x, y, z);
    ebl = he_f_juelicher_xin_energy_bend(force_pointer(force_bend));
    ebn = he_f_juelicher_xin_energy_ad(force_pointer(force_bend));

    es  = force_energy(force_strain, he, x, y, z);

    et  = ea + ev + eb + es;

    return et;
}

void ForceArea(const real *x, const real *y, const real *z,
               /**/ real *fx, real *fy, real *fz) {

    real *fax, *fay, *faz;
    real coef;
    int i;

    MALLOC(Nv, &fax); MALLOC(Nv, &fay); MALLOC(Nv, &faz);
    zero(Nv, fax); zero(Nv, fay); zero(Nv, faz);

    force_force(force_area, he, x, y, z, /**/ fax, fay, faz);

    ca = ConstraintArea(x, y, z);
    
    coef = (Kc * ca - lama) / A0;

    for ( i = 0; i < Nv; i ++ ) {
        fx[i] += coef * fax[i];
        fy[i] += coef * fay[i];
        fz[i] += coef * faz[i];
    }

    FREE(fax); FREE(fay); FREE(faz);

}
void ForceVolume(const real *x, const real *y, const real *z,
                 /**/ real *fx, real *fy, real *fz) {

    real *fvx, *fvy, *fvz;
    real coef;
    int i;

    MALLOC(Nv, &fvx); MALLOC(Nv, &fvy); MALLOC(Nv, &fvz);
    zero(Nv, fvx); zero(Nv, fvy); zero(Nv, fvz);

    force_force(force_volume, he, x, y, z, /**/ fvx, fvy, fvz);

    cv = ConstraintVolume(x, y, z);

    coef = (Kc * cv - lamv) / V0;

    for ( i = 0; i < Nv; i ++ ) {
        fx[i] += coef * fvx[i];
        fy[i] += coef * fvy[i];
        fz[i] += coef * fvz[i];
    }

    FREE(fvx); FREE(fvy); FREE(fvz);

}

void Force0(const real *x, const real *y, const real *z,
            /**/ real *fx, real *fy, real *fz) {

    zero(Nv, fx); zero(Nv, fy); zero(Nv, fz);
    
    ForceArea(x, y, z, /**/ fx, fy, fz);
    ForceVolume(x, y, z, /**/ fx, fy, fz);

    force_force(force_bend, he, x, y, z, /**/ fx, fy, fz);
    force_force(force_strain, he, x, y, z, /**/ fx, fy, fz);

}

static void euler(real dt, const real *fx, const real *fy, const real *fz,
                  real *vx, real *vy, real *vz) {
    int i;

    for (i = 0; i < Nv; i++) {
        vx[i] += dt*fx[i];
        vy[i] += dt*fy[i];
        vz[i] += dt*fz[i];
    }

}

static int diff(int i, int j, const real *x, const real *y, const real *z,
                /**/ real e[3]) {

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

    for (e = 0; e < Ne; e++) {
        i = D1[e]; j = D2[e];

        diff(i, j, x, y, z, r);
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
                      const real *vx, const real *vy, const real *vz,
                      /*io*/real *fx, real *fy, real *fz) {

    int e, i, j;
    real u[3], r[3], rn[3], p[3];
    real ra, sigma, coef;

    coef = sqrt(12.0);
    sigma = sqrt(2.0*xi*kBT/dt);

    for (e = 0; e < Ne; e++) {
        i = D1[e]; j = D2[e];

        diff(i, j, x, y, z, r);
        diff(i, j, vx, vy, vz, u);
        vec_norm(r, rn);

        ra  = coef*(rand()/(real)RAND_MAX - 0.5);
        ra *= sigma;
        vec_scalar_append(rn, -ra, i, fx, fy, fz);
        vec_scalar_append(rn, ra, j, fx, fy, fz);

    }

}

static real Kinetic0(real *vx, real *vy, real *vz, real m) {

    int i;
    real s;
    s = 0;
    for (i = 0; i < Nv; i++) {
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
    for (i = 0; i < Nv; i++) {
        c = sqrt(fx[i]*fx[i] + fy[i]*fy[i] + fz[i]*fz[i]);
        if (c > m)
            m = c;
    }
    return m;
}

static int main0(real *vx, real *vy, real *vz,
                 real *fx, real *fy, real *fz) {

    int i, j;
    real rnd;
    real et, ep, ek;
    char file[4048];
    char filemsg[4048]="stat.dat";
    FILE *fm;
    real dt, h, time;

    real err1, err2;
    
    strcpy(dir, ".");
    time = 0.0;
    dt   = dt_in;

    err1 = 0.0;
    err2 = 0.0;
    
    mkdir0(dir);

    if ( (fm = fopen(fullpath(filemsg), "w") ) == NULL) {
        ER("fail to open '%s'", filemsg);
    }

    fclose(fm);

    zero(Nv, vx); zero(Nv, vy); zero(Nv, vz);

    Force0(x, y, z, /**/ fx, fy, fz);

    visc_pair(xi, vx, vy, vz, /**/ fx, fy, fz);

#ifdef DT_ADPTIVE
    h  = 0.25*0.25*e0;
    dt = fmin(dt_in, sqrt(h*mass/max_vec(fx, fy, fz)));
#endif

    if ( kBT > 0 ) {

        rand_pair(xi, kBT, dt, vx, vy, vz, /**/ fx, fy, fz);

    }

    for (i = 0; i <= end; i++) {

      if ( i % freqo == 0 ) {
	
	sprintf(file, "%07d.off", i);
	off_he_xyz_write(he, x, y, z, file);
	
      }

      if ( i % freqs == 0 ) {
	
            ep = Energy0(x, y, z);
            ek = Kinetic0(vx, vy, vz, mass);
            et = ep + ek;

	    vc = reduced_volume(A, V);

            MSG("dt, s, t: %g %d %g", dt, i, time);
            MSG("v, ca, cv, Kc: %g %g %g %g", vc, ca, cv, Kc);
            MSG("et,  ea,  ev,  ek,  eb, ebl, ebn, es: %g %g %g %g %g %g %g %g", et, ea, ev, ek, eb, ebl, ebn, es);

            fm = fopen(fullpath(filemsg), "a");
            static int First = 1;
            if (First) {
                fputs("dt s t v ca cv Kc et ea ev ek eb ebl ebn es\n", fm);
                First = 0;
            }
            fprintf(fm, "%g %d %g %g %g %g %g %g %g %g %g %g %g %g %g \n",
                    dt, i, time, ca, cv, vc, Kc, et, ea, ev, ek, eb, ebl, ebn, es);
            fclose(fm);

        }//i%freq_screen
	

        if ( i % freqk == 0 ) {
	
	err1 = ca > 0 ? ca : -ca;
	err2 = cv > 0 ? cv : -cv;
	
	if ( err1 > toler || err2 > toler ) {

	  lama = lama - Kc * ca;
	  lamv = lamv - Kc * cv;
	  Kc  *= coefk;
	  
	}
	
      }

	euler(-dt/2.0/mass, fx, fy, fz, /**/ vx, vy, vz);
        euler(dt, vx, vy, vz, /**/ x, y, z);

        time += dt;

        Force0(x, y, z, /**/ fx, fy, fz);

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

int main(int __UNUSED ac, char *av[]) {

    real *fx, *fy, *fz;
    real *vx, *vy, *vz;
    real a0;

    //printf("Here is the first line\n");
    //printf("ac = %d\n", ac);
    //printf("av = %s\n", av[0]);

    argv = av;
    argv++;

    y_inif(stdin, &he, &x, &y, &z);

    init();

    he_D(he, &D0, &D1, &D2, &D3);

    Nt = he_nt(he);
    Ne = he_ne(he);
    Nv = he_nv(he);

    srand(time(NULL));

    A0 = 4.0*pi*R*R;
    A  = force_energy(force_area, he, x, y, z);

    V0 = target_volume(A0, v);
    a0 = A0/Nt;
    e0 = eq_tri_edg(a0);

    V  = force_energy(force_volume, he, x, y, z);
    vc = reduced_volume(A, V);

    mass = 4.0*pi*R*R*D*rho/Nv;

    MSG("bending scheme : %s", force_name(force_bend));
    MSG("strain scheme  : %s", force_name(force_strain));

    MSG("A0   : %g", A0);
    MSG("V0   : %g", V0);
    MSG("A/A0 : %g", A/A0);
    MSG("V/V0 : %g", V/V0);
    MSG("v    : %g", v);
    MSG("vc   : %g", vc);
    MSG("R    : %g", R);
    MSG("rho  : %g", rho);
    MSG("mass : %g", mass);
    MSG("Nt   : %d", Nt);
    MSG("Ne   : %d", Ne);
    MSG("Nv   : %d", Nv);
    MSG("Kc   : %g", Kc);
    MSG("freqk: %d", freqk);
    MSG("coefk: %g", coefk);
    MSG("D    : %g", D);
    MSG("xi   : %g", xi);
    MSG("kBT  : %g", kBT);
    MSG("end  : %d", end);
    MSG("freqo: %d", freqo);
    MSG("freqs: %d", freqs);

    MALLOC(Nv, &fx); MALLOC(Nv, &fy); MALLOC(Nv, &fz);
    MALLOC(Nv, &vx); MALLOC(Nv, &vy); MALLOC(Nv, &vz);

    main0(vx, vy, vz, fx, fy, fz);

    FREE(fx); FREE(fy); FREE(fz);
    FREE(vx); FREE(vy); FREE(vz);

    y_fin(he, x, y, z);

    return 0;
}
