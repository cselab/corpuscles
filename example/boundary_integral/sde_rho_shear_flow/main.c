#include <stdio.h>
#include <string.h>
#include <math.h>
#include <omp.h>
#include <real.h>
#include <alg/ode.h>
#include <co/array.h>
#include <co/len.h>
#include <co/err.h>
#include <co/force.h>
#include <co/macro.h>
#include <co/matrix.h>
#include <co/memory.h>
#include <co/ode/3.h>
#include <co/oseen/3.h>
#include <co/he.h>
#include <co/punto.h>
#include <co/y.h>
#include <co/off.h>
#include <co/f/garea.h>
#include <co/f/volume.h>
#include <co/f/juelicher_xin.h>

static const char *me = "sde_shear_flow";
static const real pi = 3.141592653589793115997964;

#define FMT_IN CO_REAL_IN
#define FMT_OUT CO_REAL_OUT

static
Force *Fo[20] =
{
	NULL
};
static He *he;
static Oseen3 *oseen;
static real R, D;
static real rho, eta, gamdot, dt;
static int start, end, freq_out, freq_stat;
static real *fx, *fy, *fz;
static real *Oxx, *Oxy, *Oxz, *Oyy, *Oyz, *Ozz;
static int nv, nt;

static void usg(void) {
  fprintf(stderr, "%s garea A Kga area a Ka volume V Kv \n", me);
  fprintf(stderr, "juelicher_xin Kb C0 Kad DA0D \n");
  fprintf(stderr, "strain ref_file lim mua mub a3 a4 b1 b2 \n");
  fprintf(stderr, "gamdot, eta, dt \n");
  fprintf(stderr, "start, end, freq_out, freq_stat \n");
  fprintf(stderr, "< OFF input file \n");
  
  //exit(0);
}

static int num(char **v, /**/ int *p) {
  if (*v == NULL) {
        usg();
        ER("not enough args");
    }
    if (sscanf(*v, "%d", p) != 1)
        ER("not a number '%s'", *v);
    return CO_OK;
}
static int scl(char **v, /**/ real *p) {
    if (*v == NULL) ER("not enough args");
    if (sscanf(*v, FMT_IN, p) != 1)
        ER("not a number '%s'", *v);
    return CO_OK;
}

static int
fargv(char ***p, He *he)
{
	char *name, **v;
	int i;

	i = 0;
	v = *p;
	while (1) {
		if (v[0] == NULL) break;
		name = v[0];
		if (!force_good(name)) break;
		MSG("%s", name);
		v++;
		force_argv(name, &v, he, &Fo[i]);
		i++;
	}

	scl(v, &R);
	v++;
	scl(v, &D);
	v++;
	scl(v, &rho);
	v++;
	scl(v, &eta);
	v++;
	scl(v, &gamdot);
	v++;
	scl(v, &dt);
	v++;
	//MSG("start:      %s", v[0]);
	num(v, &start);
	v++;
	//MSG("end:        %s", v[0]);
	num(v, &end);
	v++;
	//MSG("freq_out:   %s", v[0]);
	num(v, &freq_out);
	v++;
	//MSG("freq_stat:  %s", v[0]);
	num(v, &freq_stat);
	v++;

	*p = v;
	return CO_OK;
}

static int
force(He *he, const real *x, const real *y, const real *z, real *fx, real *fy, real *fz)
{
	int i;
	i = 0;
	while (Fo[i]) {
		force_force(Fo[i], he, x, y, z, fx, fy, fz);
		i++;
	}
	return CO_OK;
}


static int
fin(void)
{
	int i;
	i = 0;
	while (Fo[i]) {
		force_fin(Fo[i]);
		i++;
	}
	return CO_OK;
}

static int
F(__UNUSED real t, const real *x, const real *y, const real *z, real *vx,  real *vy, real *vz, __UNUSED void *p0)
{
	int i, ga;
	
#pragma omp parallel for
	for (i = 0; i < nv; i++) {
		vx[i] = gamdot*z[i];
		vy[i] = vz[i] = 0;
	}
	array_zero3(nv, fx, fy, fz);
	force(he, x, y, z, fx, fy, fz);
	oseen3_apply(oseen, he, x, y, z, Oxx, Oxy, Oxz, Oyy, Oyz, Ozz);
#pragma omp parallel for
	for (ga = 0; ga < nv; ga++) {

	  int be;
	  real xx, xy, xz, yy, yz, zz;
	  real tx, ty, tz;
	  
	  for (be = 0; be < nv; be++) {
	    
	    xx = matrix_get(nv, nv, ga, be, Oxx)/eta;
	    xy = matrix_get(nv, nv, ga, be, Oxy)/eta;
	    xz = matrix_get(nv, nv, ga, be, Oxz)/eta;
	    yy = matrix_get(nv, nv, ga, be, Oyy)/eta;
	    yz = matrix_get(nv, nv, ga, be, Oyz)/eta;
	    zz = matrix_get(nv, nv, ga, be, Ozz)/eta;
	    tx = xx*fx[be] + xy*fy[be] + xz*fz[be];
	    ty = xy*fx[be] + yy*fy[be] + yz*fz[be];
	    tz = xz*fx[be] + yz*fy[be] + zz*fz[be];
	    
	    vx[ga] -= tx;
	    vy[ga] -= ty;
	    vz[ga] -= tz;    
	    
	  }
	  
	}

	
	/*for (i = 0; i < nv; i++) {
	  vx[i] = -vx[i];
	  vy[i] = -vy[i];
	  vz[i] = -vz[i];
	  }*/
	return CO_OK;
}

static real reduced_volume(real area, real volume) {
    return (6*sqrt(pi)*volume)/pow(area, 3.0/2);
}

int
main(__UNUSED int argc, char **argv)
{
	real *x, *y, *z, *vx, *vy, *vz;
	char file_out[999];
	char file_stat[99]="stat.dat";
	char file_msg[99]="msg.out";
	FILE *fm;
	Ode3 *ode;
	real t, time;
	int  s, i;
	real eng, et, ega, ev, eb, ebl, ebn, es;
	char name[99];
	real A0, V0, v0;
	real A, V, v;
	real a, e, reg;
	real M, m;
	
	err_set_ignore();
	argv++;
	y_inif(stdin, &he, &x, &y, &z);
	fargv(&argv, he);

	nv = he_nv(he);
	nt = he_nt(he);

	if ( (fm = fopen(file_stat, "w") ) == NULL) {
	  ER("Failed to open '%s'", file_stat);
	}
	fputs("#dt s t A/A0 V/V0 v et ega ev eb ebl ebn es\n", fm);
	fclose(fm);

	i = 0;
	while (Fo[i]) {

	  strcpy(name, force_name(Fo[i]));
	  
	  if ( strcmp(name, "garea") == 0 ) {
	    A0 = he_f_garea_A0(force_pointer(Fo[i]));
	  }
	  else if ( strcmp(name, "volume") == 0 ) {
	    V0 = he_f_volume_V0(force_pointer(Fo[i]));
	  }
	  i++;
	}

	v0 = reduced_volume(A0, V0);
	
	M = rho*A0*D*2;
	m = M/nv;
	a = A0/nt;
	e = 2*sqrt(a)/sqrt(sqrt(3.0));
	reg = 0.1*e;

	if ( (fm = fopen(file_msg, "w") ) == NULL) {
	  ER("Failed to open '%s'", file_msg);
	}
	
	fprintf(fm, "A0 V0 v0 = %f %f %f\n", A0, V0, v0);
	fprintf(fm, "R D rho eta gamdot = %f %f %f %f %f\n", R, D, rho, eta, gamdot);
	fprintf(fm, "Nv Nt = %i %i\n", nv, nt);
	fprintf(fm, "M m a e reg dt = %f %f %f %f %f %f\n", M, m, a, e, reg, dt);
	fclose(fm);
	
	oseen3_ini(he, reg, &oseen);
	ode3_ini(RK4, nv, dt, F, NULL, &ode);

	CALLOC3(nv, &vx, &vy, &vz);
	CALLOC3(nv, &fx, &fy, &fz);
	matrix_ini(nv, nv, &Oxx);
	matrix_ini(nv, nv, &Oxy);
	matrix_ini(nv, nv, &Oxz);
	matrix_ini(nv, nv, &Oyy);
	matrix_ini(nv, nv, &Oyz);
	matrix_ini(nv, nv, &Ozz);
	
	t = time = start*dt;
	s = start;
	
	while ( 1 ) {
	  
	  if ( s % freq_out == 0 ) {
	    sprintf(file_out, "%07d.off", s);
	    off_he_xyz_write(he, x, y, z, file_out);
	  }
	  
	  if ( s % freq_stat == 0 ) {

	    eng=0.0;
	    ega=0.0;
	    ev=0.0;
	    eb=0.0;
	    ebl=0.0;
	    ebn=0.0;
	    es=0.0;
	    A=0.0;
	    V=0.0;
	    v=0.0;
	    
	    i  = 0;
	    et = 0.0;
	    while (Fo[i]) {
	      
	      strcpy(name, force_name(Fo[i]));
	      eng = force_energy(Fo[i], he, x, y, z);
	      et += eng;
	      
	      if ( strcmp(name, "garea") == 0 ) {
		ega = eng;
		A   = he_f_garea_A(force_pointer(Fo[i]));
	      }
	      else if ( strcmp(name, "volume") == 0 ) {
		ev = eng;
		V  = he_f_volume_V(force_pointer(Fo[i]));
	      }
	      else if ( strcmp(name, "juelicher_xin") == 0 ) {
		eb  = eng;
		ebl = he_f_juelicher_xin_energy_bend(force_pointer(Fo[i]));
		ebn = he_f_juelicher_xin_energy_ad(force_pointer(Fo[i]));

	      }
	      else if ( strcmp(name, "strain") == 0 ) {
		es = eng;
	      }
	      
	      i++;
	    }

	    v = reduced_volume(A, V);

	    if ( s / 10 % freq_stat == 0 ) {
	      MSG("dt s t = %f %i %f", dt, s, t);
	      MSG("A/A0 V/V0 v  = %f %f %f", A/A0, V/V0, v);
	      MSG("et ega ev eb ebl ebn es = %f %f %f %f %f %f %f", et, ega, ev, eb, ebl, ebn, es);
	    }
	    
	    if ( (fm = fopen(file_stat, "a") ) == NULL) {
	      ER("Failed to open '%s'", file_stat);
	    }
	    fprintf(fm, "%f %i %f% f %f %f %f %f %f %f %f %f %f\n", dt, s, t, A/A0, V/V0, v, et, ega, ev, eb, ebl, ebn, es);
	    fclose(fm);

	  }
	  
	  
	  s ++;
	  t = time + dt;

	  if ( s > end ) break;

	  ode3_apply(ode, &time, t, x, y, z);
	  
	}

	FREE3(vx, vy, vz);
	FREE3(fx, fy, fz);
	matrix_fin(Oxx);
	matrix_fin(Oxy);
	matrix_fin(Oxz);
	matrix_fin(Oyy);
	matrix_fin(Oyz);
	matrix_fin(Ozz);
	oseen3_fin(oseen);
	ode3_fin(ode);
	fin();
	y_fin(he, x, y, z);
}
