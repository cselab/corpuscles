#include <stdio.h>

#include "real.h"
#include "co/argv.h"
#include "co/err.h"
#include "co/f2/len.h"
#include "co/macro.h"
#include "co/memory.h"
#include "co/skel.h"
#include "co/vec2.h"
#include "co/edg2.h"
#include "co/dedg2.h"
#include "co/sum.h"
#include "inc/def.h"

#define T F2Len

struct T {
	real Ka, a3, a4;
	real *x, *y;
	Skel *skel;
};

int
f2_len_ini(FILE * f, real Ka, real a3, real a4, T ** pq)
{
	T *q;
	Skel *skel;
	real *x, *y;

	MALLOC(1, &q);
	if (skel_read(f, &x, &y, &skel) != CO_OK)
		ERR(CO_IO, "fail to open skel file");
	q->Ka = Ka;
	q->a3 = a3;
	q->a4 = a4;
	q->x = x;
	q->y = y;
	q->skel = skel;
	*pq = q;
	return CO_OK;
}

int
f2_len_argv(char ***p, __UNUSED Skel * skel, T ** pq)
{
	int status;
	char file[MAX_STRING_SIZE];
	FILE *f;
	real Ka, a3, a4;

	if ((status = argv_str(p, file)) != CO_OK)
		return status;
	if ((f = fopen(file, "r")) == NULL)
		ERR(CO_IO, "fail to open '%s'", file);
	if ((status = argv_real(p, &Ka)) != CO_OK)
		return status;
	if ((status = argv_real(p, &a3)) != CO_OK)
		return status;
	if ((status = argv_real(p, &a4)) != CO_OK)
		return status;
	status = f2_len_ini(f, Ka, a3, a4, pq);
	if (fclose(f) != 0)
		ERR(CO_IO, "fail to close '%s'", file);
	return status;
}

int
f2_len_fin(T * q)
{
	skel_xy_fin(q->x, q->y, q->skel);
	FREE(q);
	return CO_OK;
}

real
f2_len_energy(T * q, Skel * skel0, const real * x, const real * y)
{
	real Ka, a3, a4, a[2], b[2], a0[2], b0[2];
	real *x0, *y0;
	real E, E0;
	Skel *skel;
	HeSum *sum;
	int n, e, i, j;

	Ka = q->Ka;
	a3 = q->a3;
	a4 = q->a4;
	skel = q->skel;
	x0 = q->x;
	y0 = q->y;
	n = skel_ne(skel);
	if (n != skel_ne(skel0))
		ERR(CO_INDEX, "n=%d != skel_ne(skel0)=%d", n, skel_ne(skel0));

	he_sum_ini(&sum);
	for (e = 0; e < n; e++) {
		skel_edg_ij(skel, e, &i, &j);
		vec2_get(i, x, y, a);
		vec2_get(j, x, y, b);
		vec2_get(i, x0, y0, a0);
		vec2_get(j, x0, y0, b0);
		E0 = edg2_strain(Ka, a3, a4, a0, b0, a, b);
		he_sum_add(sum, E0);
	}
	E = he_sum_get(sum);
	he_sum_fin(sum);
	return E;
}

int
f2_len_force(T * q, Skel * skel0, const real * x, const real * y, real * u,
			 real * v)
{
	real Ka, a3, a4, a[2], b[2], a0[2], b0[2], da[2], db[2];
	real *x0, *y0;
	Skel *skel;
	int n, e, i, j;

	Ka = q->Ka;
	a3 = q->a3;
	a4 = q->a4;
	skel = q->skel;
	x0 = q->x;
	y0 = q->y;
	n = skel_ne(skel);
	if (n != skel_ne(skel0))
		ERR(CO_INDEX, "n=%d != skel_ne(skel0)=%d", n, skel_ne(skel0));

	for (e = 0; e < n; e++) {
		skel_edg_ij(skel, e, &i, &j);
		vec2_get(i, x, y, a);
		vec2_get(j, x, y, b);
		vec2_get(i, x0, y0, a0);
		vec2_get(j, x0, y0, b0);
		dedg2_strain(Ka, a3, a4, a0, b0, a, b, da, db);
		vec2_append(da, i, u, v);
		vec2_append(db, j, u, v);
	}
	return CO_OK;
}
