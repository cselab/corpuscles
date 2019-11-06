#include <stdio.h>
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
#include <co/equiangulate.h>

static const char *me = "shear_flow/vesicle";

#define FMT_IN CO_REAL_IN
#define FMT_OUT CO_REAL_OUT

static
Force *Fo[99] =
{
	NULL
};
static He *he;
static Oseen3 *oseen;
//static real gamdot = 1, eta = 1, dt = 1e-2, tend = 100;
static real gamdot, eta, dt;
static int end, freq;
static real *fx, *fy, *fz;
static real *Oxx, *Oxy, *Oxz, *Oyy, *Oyz, *Ozz;
static int n;

static void usg(void) {
  fprintf(stderr, "%s garea A Kga area a Ka volume V Kv juelicher_xin Kb C0 Kad DA0D strain lim mua mub a3 a4 b1 b2 < OFF > msg\n", me);
  fprintf(stderr, "gamdot, eta, dt: shear rate, viscosity of solvent, time step\n");
  fprintf(stderr, "end: number of iterations\n");
  fprintf(stderr, "freq: frequency of output off files\n");
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
		MSG("%s", name);
		if (!force_good(name)) break;
		v++;
		force_argv(name, &v, he, &Fo[i]);
		i++;
	}

	MSG("gamdot %s:", v[0]);
	scl(v, &gamdot);
	v++;
	MSG("eta    %s:", v[0]);
	scl(v, &eta);
	v++;
	MSG("dt     %s:", v[0]);
	scl(v, &dt);
	v++;
	MSG("end    %s:", v[0]);
	num(v, &end);
	v++;
	MSG("freq    %s:", v[0]);
	num(v, &freq);
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
	//MSG("t " FMT_OUT, t);
	return CO_OK;
}

int
main(__UNUSED int argc, char **argv)
{
	real *x, *y, *z, *vx, *vy, *vz;
	real e;
	int k;
	real t, time;
	char file[999];
	Ode3 *ode;
	int cnt, i, j;

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
	k = 0;
	t = time = 0;
	i = 0;
	
	while (i <= end) {
	  
	  if ( i % freq == 0 ) {
	    sprintf(file, "%06d.off", i);
	    off_he_xyz_write(he, x, y, z, file);
	  }
	  
	  i ++;
	  t = time + dt;
	
	  ode3_apply(ode, &time, t, x, y, z);
	  
	  MSG("x[0] " FMT_OUT, x[0]);

	  /*if (i % 100 == 0 ) {
	    
	    j = 0;
	    do {
	      he_equiangulate(he, x, y, z, &cnt);
	      MSG("cnt : %d", cnt);
	      j++;
	    } while (cnt > 0 && j < 10);
	    
	    }*/
	  
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
