#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <real.h>
#include <alg/linsolve.h>
#include <alg/ode.h>
#include <co/array.h>
#include <co/bi.h>
#include <co/err.h>
#include <co/f/garea.h>
#include <co/f/juelicher_xin.h>
#include <co/force.h>
#include <co/f/volume.h>
#include <co/he.h>
#include <co/len.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/ode/3.h>
#include <co/off.h>
#include <co/punto.h>
#include <co/vec.h>
#include <co/subst.h>
#include <co/y.h>

static const char *me = "poc/2020";
static const real pi = 3.141592653589793115997964;
static const real tol = 0.0001;
static const int iter_max = 200;

#define FMT_IN CO_REAL_IN
#define O CO_REAL_OUT
static Force *Fo[20] = {
    NULL
};

static LinSolve *linsolve;
static HeFVolume *fvolume;
static He *he;
static BI *bi;
static Subst *subst;
static real R, D;
static real rho, eta, lambda, gamdot, dt;
static int start, end, freq_out, freq_stat;
static real *fx, *fy, *fz;
static real *ux, *uy, *uz;
static real *wx, *wy, *wz;
static real *Vx, *Vy, *Vz;
static real *ix, *iy, *iz;
static int nv, nt;
char file_out[999];
char file_stat[99] = "stat.dat";
char file_msg[99] = "msg.out";
FILE *fm;
static int compute_moment(int, const real *, const real *, const real *,
			  const real *, const real *, const real *,
			  real[3]);
static int compute_inertia(int, const real *, const real *, const real *,
			  real[6]);

static void
usg(void)
{
    fprintf(stderr, "%s volume V Kv garea A Kga area a Ka \n", me);
    fprintf(stderr, "juelicher_xin Kb C0 Kad DA0D\n");
    fprintf(stderr, "strain ref_file lim mua mub a3 a4 b1 b2\n");
    fprintf(stderr, "cortez R D rho eta lamda gamdot dt\n");
    fprintf(stderr, "start end freq_out freq_stat\n");
    fprintf(stderr, "< OFF input file\n");
}

static int
num(char **v, /**/ int *p)
{
    if (*v == NULL) {
	usg();
	ER("not enough args");
    }
    if (sscanf(*v, "%d", p) != 1)
	ER("not a number '%s'", *v);
    return CO_OK;
}

static int
scl(char **v, /**/ real * p)
{
    if (*v == NULL)
	ER("not enough args");
    if (sscanf(*v, FMT_IN, p) != 1)
	ER("not a number '%s'", *v);
    return CO_OK;
}

static int
fargv(char ***p, He * he)
{

    char *name, **v;
    int i;

    i = 0;
    v = *p;

    if (strcmp(v[0], "volume") != 0) {
	ER("not a volume %s", v[0]);
    }
    v++;
    he_f_volume_argv(&v, he, &fvolume);
    MALLOC3(nv, &Vx, &Vy, &Vz);
    MALLOC3(nv, &ix, &iy, &iz);

    while (1) {
	if (v[0] == NULL)
	    break;
	name = v[0];
	if (!force_good(name))
	    break;
	MSG("%s", name);
	v++;
	force_argv(name, &v, he, &Fo[i]);
	i++;
    }

    name = v[0];
    if (name == NULL)
	ER("expecting BI");
    if (!bi_good(name)) {
	MSG("not a bi name: '%s'", name);
	ER("possible names are: %s", bi_list());
    }
    v++;
    bi_argv(name, &v, he, &bi);

    scl(v, &R);
    v++;
    scl(v, &D);
    v++;
    scl(v, &rho);
    v++;
    scl(v, &eta);
    v++;
    scl(v, &lambda);
    v++;
    scl(v, &gamdot);
    v++;
    scl(v, &dt);
    v++;
    num(v, &start);
    v++;
    num(v, &end);
    v++;
    num(v, &freq_out);
    v++;
    //MSG("freq_stat:  %s", v[0]);
    num(v, &freq_stat);
    v++;

    *p = v;
    return CO_OK;
}

static int
force(He * he, const real * x, const real * y, const real * z, real * fx,
      real * fy, real * fz)
{
    int i;
    int status;
    i = 0;
    while (Fo[i]) {
	status = force_force(Fo[i], he, x, y, z, fx, fy, fz);
	if (status != CO_OK) {
	    MSG("%s: force '%s' failed", me, force_name(Fo[i]));
	    off_he_xyz_write(he, x, y, z, "fail.off");
	    ER("force: write fail.off");
	    exit(2);
	}
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
F(__UNUSED real t, const real * x, const real * y, const real * z,
  real * vx, real * vy, real * vz, __UNUSED void *p0)
{
    enum {X, Y, Z};
    int i;
    real coef, al;

    coef = 2 / (1 + lambda);
    al = -2 / (eta * (1 + lambda));
    array_zero3(nv, fx, fy, fz);
    force(he, x, y, z, fx, fy, fz);

    bi_update(bi, he, x, y, z);

    array_zero3(nv, vx, vy, vz);
    for (i = 0; i < nv; i++)
	vx[i] += coef * gamdot * z[i];
    bi_single(bi, he, al, x, y, z, fx, fy, fz, vx, vy, vz);
    array_zero3(nv, ix, iy, iz);
    //subst_apply_initial(subst, he, bi, x, y, z, vx, vy, vz, ix, iy, iz, ux, uy, uz);
    subst_apply(subst, he, bi, x, y, z, vx, vy, vz, ux, uy, uz);
    if (subst_niter(subst) >= iter_max) {
	MSG("Subst.iiter: %d", subst_niter(subst));
	off_he_xyz_write(he, x, y, z, "fail.off");
	ER("write fail.off");
    }
    array_copy3(nv, ux, uy, uz, vx, vy, vz);
    array_zero3(nv, Vx, Vy, Vz);
    he_f_volume_force(fvolume, he, x, y, z, Vx, Vy, Vz);
    array_axpy3(nv, -dt, Vx, Vy, Vz, vx, vy, vz);

    /*real Moment[3];
    real Inertia[6];
    real Omega[3];
    real rad[3];
    real vel[3];
    compute_moment(nv, x, y, z, vx, vy, vz, Moment);
    compute_inertia(nv, x, y, z, Inertia);
    lin_solve_sym(linsolve, Inertia, Moment, Omega);
    Omega[Y] = Omega[Z] = 0;
    for (i = 0; i < nv; i++) {
	vec_get(i, x, y, z, rad);
	vec_cross(rad, Omega, vel);
	vec_substr(vel, i, vx, vy, vz);
    }
    compute_moment(nv, x, y, z, vx, vy, vz, Moment);*/
    return CO_OK;
}


static real
reduced_volume(real area, real volume)
{
    return (6 * sqrt(pi) * volume) / pow(area, 3.0 / 2);
}

int
main(__UNUSED int argc, char **argv)
{
    real *x, *y, *z;
    Ode3 *ode;
    real t, time;
    int s, i;
    real eng, et, ega, ev, eb, ebl, ebn, es;
    char name[99];
    real A0, V0, v0;
    real A, V, v;
    real a, e, reg;
    real M, m;
    real alpha;

    err_set_ignore();
    argv++;
    y_inif(stdin, &he, &x, &y, &z);
    nv = he_nv(he);
    nt = he_nt(he);
    fargv(&argv, he);
    if ((fm = fopen(file_stat, "w")) == NULL) {
	ER("Failed to open '%s'", file_stat);
    }
    alpha = 2 * (1 - lambda) / (1 + lambda);
    subst_ini(nv, alpha, tol, iter_max, &subst);
    fputs("#dt s t A/A0 V/V0 v et ega ev eb ebl ebn es\n", fm);
    fclose(fm);

    V0 = he_f_volume_V0(fvolume);
    MSG("V0 = " O, V0);

    i = 0;
    A0 = -1;
    while (Fo[i]) {

	strcpy(name, force_name(Fo[i]));

	if (strcmp(name, "garea") == 0) {
	    A0 = he_f_garea_A0(force_pointer(Fo[i]));
	}
	//else if ( strcmp(name, "volume") == 0 ) {
	//V0 = he_f_volume_V0(force_pointer(Fo[i]));}
	i++;
    }

    v0 = reduced_volume(A0, V0);

    M = rho * A0 * D * 2;
    m = M / nv;
    a = A0 / nt;
    e = 2 * sqrt(a) / sqrt(sqrt(3.0));
    reg = 0.1 * e;

    if ((fm = fopen(file_msg, "w")) == NULL) {
	ER("Failed to open '%s'", file_msg);
    }

    fprintf(fm, "A0 V0 v0 = " O " " O " " O "\n", A0, V0, v0);
    fprintf(fm, "R D rho eta lambda gamdot = %g %g %g %g %g %g\n", (double)R, (double)D,
	    (double)rho, (double)eta, (double)lambda, (double)gamdot);
    fprintf(fm, "Nv Nt = %i %i\n", nv, nt);
    fprintf(fm, "M m a e reg dt = %g %g %g %g %g %g\n", (double)M, (double)m, (double)a, (double)e, (double)reg,
	    (double)dt);
    fclose(fm);

    lin_solve_ini(3, &linsolve);
    ode3_ini(RKF45, nv, dt, F, NULL, &ode);

    CALLOC3(nv, &ux, &uy, &uz);
    CALLOC3(nv, &wx, &wy, &wz);
    CALLOC3(nv, &fx, &fy, &fz);

    t = time = start * dt;
    s = start;

    while (1) {

	if (s % freq_out == 0) {
	    sprintf(file_out, "%08d.off", s);
	    off_he_xyz_write(he, x, y, z, file_out);
	}

	if (s % freq_stat == 0) {

	    eng = 0.0;
	    ega = 0.0;
	    ev = 0.0;
	    eb = 0.0;
	    ebl = 0.0;
	    ebn = 0.0;
	    es = 0.0;
	    A = 0.0;
	    V = 0.0;
	    v = 0.0;

	    et = 0.0;

	    ev = he_f_volume_energy(fvolume, he, x, y, z);
	    V = he_f_volume_V(fvolume);

	    i = 0;
	    while (Fo[i]) {

		strcpy(name, force_name(Fo[i]));
		eng = force_energy(Fo[i], he, x, y, z);
		et += eng;

		if (strcmp(name, "garea") == 0) {
		    ega = eng;
		    A = he_f_garea_A(force_pointer(Fo[i]));
		}
		//else if ( strcmp(name, "volume") == 0 ) {
		//  ev = eng;
		//  V  = he_f_volume_V(force_pointer(Fo[i]));
		//}
		else if (strcmp(name, "juelicher_xin") == 0) {
		    eb = eng;
		    ebl =
			he_f_juelicher_xin_energy_bend(force_pointer
						       (Fo[i]));
		    ebn =
			he_f_juelicher_xin_energy_ad(force_pointer(Fo[i]));

		} else if (strcmp(name, "strain") == 0) {
		    es = eng;
		}

		i++;
	    }

	    v = reduced_volume(A, V);

	    if (s % freq_stat == 0) {
		MSG("dt s t = " O " %i  " O, dt, s, t);
		MSG("A/A0 V/V0 v  = " O " " O " " O, A / A0, V / V0, v);
		MSG("et ega ev eb ebl ebn es = " O " " O " " O " " O " " O " " O " " O, et,
		    ega, ev, eb, ebl, ebn, es);
	    }

	    if ((fm = fopen(file_stat, "a")) == NULL) {
		ER("Failed to open '%s'", file_stat);
	    }
	    fprintf(fm, O " %i " O " " O " " O " " O " " O " " O " " O " " O " " O " " O " " O"\n",
		    dt, s, t, A / A0, V / V0, v, et, ega, ev, eb, ebl, ebn, es);
	    fclose(fm);

	}


	s++;
	t = time + dt;

	if (s > end)
	    break;
	if (ode3_apply(ode, &time, t, x, y, z) != CO_OK) {
	    off_he_xyz_write(he, x, y, z, "fail.off");
	    ER("ode3: write fail.off");
	}
	y_tocm_xy(he, x, y, z);
    }

    FREE3(Vx, Vy, Vz);
    FREE3(ux, uy, uz);
    FREE3(wx, wy, wz);
    FREE3(fx, fy, fz);
    FREE3(ix, iy, iz);
    lin_solve_fin(linsolve);
    ode3_fin(ode);
    bi_fin(bi);
    fin();
    y_fin(he, x, y, z);
    subst_fin(subst);
    he_f_volume_fin(fvolume);

}

static int
compute_moment(int n, const real * x, const real * y, const real * z,
	       const real * vx, const real * vy, const real * vz,
	       real Moment[3])
{
    int i;
    real rad[3];
    real vel[3];
    real moment[3];

    vec_zero(Moment);
    for (i = 0; i < n; i++) {
	vec_get(i, x, y, z, rad);
	vec_get(i, vx, vy, vz, vel);
	vec_cross(vel, rad, moment);
	vec_add(moment, Moment);
    }
    vec_scale(1.0 / n, Moment);
    return CO_OK;
}

static int
compute_inertia(int n, const real * xx, const real * yy, const real * zz,
		 real inertia[6])
{
    enum {XX, XY, XZ, YY, YZ, ZZ};
    real x;
    real y;
    real z;
    int i;
    inertia[XX] = inertia[XY] = inertia[XZ] = 0;
    inertia[YY] = inertia[YZ] = inertia[ZZ] = 0;
    for (i = 0; i < n; i++) {
	x = xx[i];
	y = yy[i];
	z = zz[i];
	inertia[XX] += z*z + y*y;
	inertia[YY] += z*z + x*x;
	inertia[ZZ] += y*y + x*x;
	inertia[XY] -= x*y;
	inertia[XZ] -= x*z;
	inertia[YZ] -= x*y;
    }
    inertia[XX] /= n;
    inertia[XY] /= n;
    inertia[XZ] /= n;
    inertia[YY] /= n;
    inertia[YZ] /= n;
    inertia[ZZ] /= n;
    return CO_OK;
}
