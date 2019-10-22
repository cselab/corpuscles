#include <stdio.h>
#include <tgmath.h>

#include "real.h"

#include "co/dedg.h"
#include "co/edg.h"
#include "co/err.h"
#include "co/he.h"
#include "co/macro.h"
#include "co/memory.h"
#include "co/vec.h"
#include "co/argv.h"


#include "co/f/wlc.h"

#define T HeFWlc

struct T {
	int n;
	real *edg, *dedg;
	real *e0;
	real x0, K;
};

static void
get_ij(int m, He * he, /**/ int *pi, int *pj)
{
	int h, n, i, j;

	h = he_hdg_edg(he, m);
	n = he_nxt(he, h);
	i = he_ver(he, h);
	j = he_ver(he, n);
	*pi = i;
	*pj = j;
}

static void
get(int m, He * he,
	const real * x, const real * y, const real * z, /**/
	real a[3], real b[3])
{
	int i, j;

	get_ij(m, he, &i, &j);
	vec_get(i, x, y, z, /**/ a);
	vec_get(j, x, y, z, /**/ b);
}


static real
wlc(real x)
{
	if (1 - x == 0)
		ERR(CO_NUM, "1 - x == 0");
	return (3 * pow(x, 2) - 2 * pow(x, 3)) / (1 - x);
}

static real
dwlc(real x)
{
	if (1 - x == 0)
		ERR(CO_NUM, "1 - x == 0");
	return x * (((3 - 2 * x) * x) / pow(1 - x, 2) + 6);
}

static real
compute_energy0(real x0, real l0, real r)
{
	int m;
	real kp, Epow, Ewlc;

	m = 2;						/* TODO */
	kp = (pow(l0, m) * dwlc(x0)) / (4 * (m - 1));

	Epow = kp / pow(r, m - 1);
	Ewlc = ((l0 * wlc((r * x0) / l0)) / x0) / 4;

	return Epow + Ewlc;
}

static real
compute_energy(real x0, int n, real * e, const real * e0)
{
	int i;
	real v;

	v = 0;
	for (i = 0; i < n; i++)
		v += compute_energy0(x0, e0[i], e[i]) - compute_energy0(x0, e0[i],
																e0[i]);
	return v;
}

int
he_f_wlc_ini(real x0, real K, const real * x, const real * y,
			 const real * z, He * he, T ** pq)
{
	T *q;
	int n, m;
	real a[3], b[3];
	real *e0;

	MALLOC(1, &q);
	n = he_ne(he);

	MALLOC(n, &q->edg);
	MALLOC(n, &q->dedg);
	MALLOC(n, &q->e0);

	q->n = n;
	q->x0 = x0;
	q->K = K;
	e0 = q->e0;
	for (m = 0; m < n; m++) {
		get(m, he, x, y, z, /**/ a, b);
		e0[m] = edg_abs(a, b);
	}

	*pq = q;
	return CO_OK;
}

int
he_f_wlc_argv(__UNUSED char ***p, __UNUSED He * he, __UNUSED T ** pq)
{
	ERR(CO_IO, "wlc is not implimented");
}

int
he_f_wlc_fin(T * q)
{
	FREE(q->edg);
	FREE(q->dedg);
	FREE(q->e0);
	FREE(q);
	return CO_OK;
}

int
he_f_wlc_e(T * q, /**/ real ** pa)
{
	*pa = q->edg;
	return CO_OK;
}

static void
compute_edg(He * he, const real * e0, const real * x, const real * y,
			const real * z, /**/ real * edg, real * dedg)
{
	real one, a[3], b[3];
	int n, m;

	n = he_ne(he);
	for (m = 0; m < n; m++) {
		get(m, he, x, y, z, /**/ a, b);
		edg[m] = one = edg_abs(a, b);
		dedg[m] = one - e0[m];
	}
}

static void
compute_force(const real * e0, real K, real * dedg,
			  He * he, const real * x, const real * y, const real * z, /**/
			  real * fx, real * fy, real * fz)
{
	int n, m, i, j;
	real a[3], b[3], da[3], db[3], coeff;

	n = he_ne(he);
	for (m = 0; m < n; m++) {
		get_ij(m, he, /**/ &i, &j);
		vec_get(i, x, y, z, /**/ a);
		vec_get(j, x, y, z, /**/ b);
		dedg_abs(a, b, /**/ da, db);

		coeff = 2 * K * dedg[m] / e0[m];

		vec_scalar_append(da, coeff, i, /**/ fx, fy, fz);
		vec_scalar_append(db, coeff, j, /**/ fx, fy, fz);
	}
}

int
he_f_wlc_force(T * q, He * he,
			   const real * x, const real * y, const real * z, /**/
			   real * fx, real * fy, real * fz)
{
	int n;
	real *edg, *dedg, *e0, K;

	n = q->n;
	edg = q->edg;
	dedg = q->dedg;
	K = q->K;
	e0 = q->e0;
	if (he_ne(he) != n)
		ERR(CO_INDEX, "he_ne(he)=%d != n = %d", he_ne(he), n);
	compute_edg(he, e0, x, y, z, /**/ edg, dedg);
	compute_force(e0, K, dedg, he, x, y, z, /**/ fx, fy, fz);
	return CO_OK;
}

real
he_f_wlc_energy(T * q, He * he,
				const real * x, const real * y, const real * z)
{
	int n;
	real *edg, *dedg, *e0, v, x0, K;

	n = q->n;
	edg = q->edg;
	dedg = q->dedg;
	e0 = q->e0;
	x0 = q->x0;
	K = q->K;

	if (he_ne(he) != n)
		ERR(CO_INDEX, "he_ne(he)=%d != n = %d", he_ne(he), n);

	compute_edg(he, e0, x, y, z, /**/ edg, dedg);
	v = compute_energy(x0, n, edg, e0);

	return K * v;
}
