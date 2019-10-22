#include <stdio.h>
#include <real.h>
#include <alg/ode.h>
#include <co/array.h>
#include <co/err.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/ode/3v.h>
#include <co/skel.h>

#define T Ode3v

struct T {
	int n;
	real *r, *x, *y, *z, *vx, *vy, *vz, *fx, *fy, *fz;
	Ode *ode;
	int (*fun)(real, const real *, const real *, const real *,
			   const real *, const real *, const real *, real *, real *,
			   real *, void *);
	void *p;
};

static int
F(__UNUSED real t, const real * r, real * f, void *q0)
{
	T *q;
	int n;
	real *x, *y, *z, *vx, *vy, *vz, *fx, *fy, *fz;
	void *p;

	q = q0;
	n = q->n;
	x = q->x;
	y = q->y;
	z = q->z;
	vx = q->vx;
	vy = q->vy;
	vz = q->vz;
	fx = q->fx;
	fy = q->fy;
	fz = q->fz;
	p = q->p;
	array_zero(n, fx);
	array_zero(n, fy);
	array_zero(n, fz);
	array_copy(n, r, x);
	array_copy(n, r + n, y);
	array_copy(n, r + 2 * n, z);
	array_copy(n, r + 3 * n, vx);
	array_copy(n, r + 4 * n, vy);
	array_copy(n, r + 5 * n, vz);
	if (q->fun(t, x, y, z, vx, vy, vz, fx, fy, fz, p) != CO_OK)
		ERR(CO_NUM, "q->fun failed");
	array_copy(n, vx, f);
	array_copy(n, vy, f + n);
	array_copy(n, vz, f + 2 * n);
	array_negative(n, fx, f + 3 * n);
	array_negative(n, fy, f + 4 * n);
	array_negative(n, fz, f + 5 * n);
	return CO_OK;
}

int
ode3v_ini(int type, int n, real dt,
		  int (*fun)(real, const real *, const real *, const real *,
					 const real *, const real *, const real *, real *,
					 real *, real *, void *), void *p, T ** pq)
{
	T *q;
	real *r, *x, *y, *z, *vx, *vy, *vz, *fx, *fy, *fz;
	Ode *ode;

	MALLOC(1, &q);
	MALLOC(6 * n, &r);
	MALLOC3(n, &x, &y, &z);
	MALLOC3(n, &vx, &vy, &vz);
	MALLOC3(n, &fx, &fy, &fz);
	q->n = n;
	q->r = r;
	q->x = x;
	q->y = y;
	q->z = z;
	q->vx = vx;
	q->vy = vy;
	q->vz = vz;
	q->fx = fx;
	q->fy = fy;
	q->fz = fz;
	q->fun = fun;
	q->p = p;
	if (ode_ini(type, 6 * n, dt, F, q, &ode) != CO_OK)
		ERR(CO_MEMORY, "ode_ini failed");
	q->ode = ode;
	*pq = q;
	return CO_OK;
}

int
ode3v_fin(T * q)
{
	FREE(q->r);
	FREE3(q->x, q->y, q->z);
	FREE3(q->vx, q->vy, q->vz);
	FREE3(q->fx, q->fy, q->fz);
	if (ode_fin(q->ode) != CO_OK)
		ERR(CO_MEMORY, "ode_fin failed");
	FREE(q);
	return CO_OK;
}

int
ode3v_apply(T * q, real * time, real t, real * x, real * y, real * z,
			real * vx, real * vy, real * vz)
{
	int n;
	real *r;
	Ode *ode;

	n = q->n;
	r = q->r;
	ode = q->ode;
	array_copy(n, x, r);
	array_copy(n, y, r + n);
	array_copy(n, z, r + 2 * n);
	array_copy(n, vx, r + 3 * n);
	array_copy(n, vy, r + 4 * n);
	array_copy(n, vz, r + 5 * n);
	if (ode_apply(ode, time, t, r) != CO_OK)
		ERR(CO_NUM, "ode_apply failed");
	array_copy(n, r, x);
	array_copy(n, r + n, y);
	array_copy(n, r + 2 * n, z);
	array_copy(n, r + 3 * n, vx);
	array_copy(n, r + 4 * n, vy);
	array_copy(n, r + 5 * n, vz);
	return CO_OK;
}
