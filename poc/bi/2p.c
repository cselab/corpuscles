#include <stdio.h>
#include <math.h>
#include <real.h>
#include <alg/ode.h>
#include <co/array.h>
#include <co/len.h>
#include <co/err.h>
#include <co/force2.h>
#include <co/macro.h>
#include <co/matrix.h>
#include <co/memory.h>
#include <co/ode/2.h>
#include <co/oseen2.h>
#include <co/skel.h>
#include <co/punto.h>
#include <co/vtk2.h>

#define FMT CO_REAL_OUT
enum
{
	X, Y
};

static
Force2 *Force[99] =
{
	NULL
};
static Skel *skel;
static Oseen2 *oseen;
static real gdot = 1, mu = 1, dt = 0.05, tend = 100;
static real lo[2] = {-2, -0.5};
static real hi[2] = {2, 0.5};
static int size[2] = {320, 80};

static real *x, *y, *fx, *fy, *pressure, *ux, *uy;
static real *Oxx, *Oxy, *Oyy;
static int n;

static int
fargv(char ***p, Skel *skel)
{
	char *name, **v;
	int i;

	i = 0;
	v = *p;
	while (1) {
		if (v[0] == NULL) break;
		name = v[0];
		v++;
		MSG("%s", name);
		if (!force2_good(name)) break;
		force2_argv(name, &v, skel, &Force[i]);
		i++;
	}

	*p = v;
	return CO_OK;
}

static int
force(Skel *skel, const real *x, const real *y, real *fx, real *fy)
{
	int i;

	array_zero(n, fx);
	array_zero(n, fy);
	i = 0;
	while (Force[i]) {
		force2_force(Force[i], skel, x, y, fx, fy);
		i++;
	}
	return CO_OK;
}


static int
fin(void)
{
	int i;
	i = 0;
	while (Force[i]) {
		force2_fin(Force[i]);
		i++;
	}
	return CO_OK;
}

static int
F(__UNUSED real t, const real *x, const real *y, real *vx,  real *vy, __UNUSED void *p0)
{
	int i, ga, be;
	real xx, xy, yy;
	for (i = 0; i < n; i++) {
		vx[i] = gdot*y[i];
		vy[i] = 0;
	}
	force(skel, x, y, fx, fy);
	oseen2_apply(oseen, skel, x, y, Oxx, Oxy, Oyy);
	for (ga = 0; ga < n; ga++)
		for (be = 0; be < n; be++) {
			xx = matrix_get(n, n, be, ga, Oxx)/mu;
			xy = matrix_get(n, n, be, ga, Oxy)/mu;
			yy = matrix_get(n, n, be, ga, Oyy)/mu;
			vx[be] -= xx*fx[ga] + xy*fy[ga];
			vy[be] -= xy*fx[ga] + yy*fy[ga];
		}
	for (i = 0; i < n; i++) {
		vx[i] = -vx[i];
		vy[i] = -vy[i];
	}
	//MSG("t " FMT, t);
	return CO_OK;
}

static void
get_pressure(void)
{
	int i, j, k;
	real sx, sy, r[2], u[2];
	
	sx = (hi[X] - lo[X])/(size[X] - 1);
	sy = (hi[Y] - lo[Y])/(size[Y] - 1);
	force(skel, x, y, fx, fy);
	for (i = 0; i < size[X]; i++)
		for (j = 0; j < size[Y]; j++) {
			r[X] = lo[X] + sx*i;
			r[Y] = lo[Y] + sy*j;
			k = j*size[X] + i;
			pressure[k] = oseen2_pressure(oseen, skel, x, y, fx, fy, r);
			oseen2_velocity(oseen, skel, mu, x, y, fx, fy, r, u);
			ux[k] = u[X] + gdot*r[Y];
			uy[k] = u[Y];
	}
}

int
main(__UNUSED int argc, char **argv)
{
	real *vx, *vy;
	real e, t, time;
	int k;
	Ode2 *ode;
	char file[999];
	FILE *f;
	Vtk2 *vtk;

	argv++;
	skel_read(stdin, &x, &y, &skel);
	fargv(&argv, skel);
	n = skel_nv(skel);
	e = 0.01;
	MALLOC(size[X]*size[Y], &pressure);
	MALLOC2(size[X]*size[Y], &ux, &uy);
	vtk2_ini(lo, hi, size, &vtk);
	oseen2_ini(e, &oseen);
	ode2_ini(RK4, n, dt/10, F, NULL, &ode);
	MSG("len " FMT, len(skel, x, y));
	CALLOC2(n, &vx, &vy);
	CALLOC2(n, &fx, &fy);
	matrix_ini(n, n, &Oxx);
	matrix_ini(n, n, &Oxy);
	matrix_ini(n, n, &Oyy);
	k = 0;
	t = time = 0;
	while (time < tend) {
		t = time + dt;
		ode2_apply(ode, &time, t, x, y);
		MSG("x[0] " FMT, x[0]);
		sprintf(file, "%05d.off", k);
		f = fopen(file, "w");
		skel_off_write(n, x, y, f);
		fclose(f);
		get_pressure();
		sprintf(file, "%05d.vtk", k);
		f = fopen(file, "w");
		vtk2_fwrite3(vtk, ux, uy, pressure, "ux", "uy", "pressure", f);
		fclose(f);
		k++;
	}
	FREE2(vx, vy);
	FREE2(fx, fy);
	FREE(pressure);
	FREE2(ux, uy);
	matrix_fin(Oxx);
	matrix_fin(Oxy);
	matrix_fin(Oyy);
	oseen2_fin(oseen);
	ode2_fin(ode);
	vtk2_fin(vtk);
	fin();
	skel_xy_fin(x, y, skel);
}

/*

Put

git clean -fdxq
m clean lint
A=0.8835572001943658
f=data/100.skel
./2p len $f 50 0.05 0.05 bend_sc 0.01 0 < $f

co.geomview -f 38 -a /u/a *0.off
vi.visit -o *.vtk
           
Kill git 

*/
 