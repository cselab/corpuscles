#include <stdio.h>
#include <string.h>
#include <math.h>
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
#include <co/oseen3.h>
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
static real gamdot, eta, dt;
static int start, end, freq_out, freq_stat;
static real *fx, *fy, *fz;
static real *Oxx, *Oxy, *Oxz, *Oyy, *Oyz, *Ozz;
static int n;

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

	MSG("gamdot:     %s", v[0]);
	scl(v, &gamdot);
	v++;
	MSG("eta:        %s", v[0]);
	scl(v, &eta);
	v++;
	MSG("dt:         %s", v[0]);
	scl(v, &dt);
	v++;
	MSG("start:      %s", v[0]);
	num(v, &start);
	v++;
	MSG("end:        %s", v[0]);
	num(v, &end);
	v++;
	MSG("freq_out:   %s", v[0]);
	num(v, &freq_out);
	v++;
	MSG("freq_stat:  %s", v[0]);
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
	int i, ga, be;
	real xx, xy, xz, yy, yz, zz;
	for (i = 0; i < n; i++) {
		vx[i] = gamdot*z[i];
		vy[i] = vz[i] = 0;
	}
	array_zero3(n, fx, fy, fz);
	force(he, x, y, z, fx, fy, fz);
	oseen3_apply(oseen, he, x, y, z, Oxx, Oxy, Oxz, Oyy, Oyz, Ozz);
	for (ga = 0; ga < n; ga++)
		for (be = 0; be < n; be++) {
			xx = matrix_get(n, n, be, ga, Oxx)/eta;
			xy = matrix_get(n, n, be, ga, Oxy)/eta;
			xz = matrix_get(n, n, be, ga, Oxz)/eta;
			yy = matrix_get(n, n, be, ga, Oyy)/eta;
			yz = matrix_get(n, n, be, ga, Oyz)/eta;
			zz = matrix_get(n, n, be, ga, Ozz)/eta;
			vx[be] -= xx*fx[ga] + xy*fy[ga] + xz*fz[ga];
			vy[be] -= xy*fx[ga] + yy*fy[ga] + yz*fz[ga];
			vz[be] -=  xz*fx[ga] + yz*fy[ga] + zz*fz[ga];
		}
	for (i = 0; i < n; i++) {
		vx[i] = -vx[i];
		vy[i] = -vy[i];
		vz[i] = -vz[i];
	}
	return CO_OK;
}

static real reduced_volume(real area, real volume) {
    return (6*sqrt(pi)*volume)/pow(area, 3.0/2);
}

int
main(__UNUSED int argc, char **argv)
{
	real *x, *y, *z, *vx, *vy, *vz;
	real e;
	char file_out[999];
	char file_stat[99]="stat.dat";
	FILE *fm;
	Ode3 *ode;
	real t, time;
	int s, i;
	real eng, ega, ev, eb, ebl, ebn, es;
	char name[99];
	real A0, V0, v0;
	real A, V, v;

	err_set_ignore();
	argv++;
	y_inif(stdin, &he, &x, &y, &z);
	fargv(&argv, he);
	n = he_nv(he);
	e = 0.025;
	oseen3_ini(he, e, &oseen);
	ode3_ini(RK4, n, dt, F, NULL, &ode);

	CALLOC3(n, &vx, &vy, &vz);
	CALLOC3(n, &fx, &fy, &fz);
	matrix_ini(n, n, &Oxx);
	matrix_ini(n, n, &Oxy);
	matrix_ini(n, n, &Oxz);
	matrix_ini(n, n, &Oyy);
	matrix_ini(n, n, &Oyz);
	matrix_ini(n, n, &Ozz);
	
	t = time = start*dt;
	s = start;

	if ( (fm = fopen(file_stat, "w") ) == NULL) {
	  ER("Failed to open '%s'", file_stat);
	}
	fputs("#dt s t A/A0 V/V0 v ega ev eb ebl ebn es\n", fm);
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
	MSG("A0, V0, v0 = %f %f %f", A0, V0, v0);
	
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
	    
	    i = 0;
	    while (Fo[i]) {
	      
	      strcpy(name, force_name(Fo[i]));
	      eng = force_energy(Fo[i], he, x, y, z);

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
	      MSG("ega ev eb ebl ebn es = %f %f %f %f %f %f", ega, ev, eb, ebl, ebn, es);
	    }
	    
	    if ( (fm = fopen(file_stat, "a") ) == NULL) {
	      ER("Failed to open '%s'", file_stat);
	    }
	    fprintf(fm, "%f %i %f% f %f %f %f %f %f %f %f %f\n", dt, s, t, A/A0, V/V0, v, ega, ev, eb, ebl, ebn, es);
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
