#include <stdio.h>

#include "real.h"
#include "co/argv.h"
#include "co/err.h"
#include "co/skel.h"
#include "co/memory.h"
#include "co/f2/glen.h"

#define T F2Glen

struct T
{
	int n;
	real *len;
	real l, k;
};

int
f2_glen_ini(real l, real k, Skel *skel, T **pq)
{
	return CO_OK;
}

int
f2_glen_argv(char ***pv, Skel *skel, T **pq)
{
	return CO_OK;
}

int f2_glen_fin(T *q)
{
	return CO_OK;
}

real
f2_glen_energy(T *q, Skel *skel, const real *x, const real *y)
{
	return CO_OK;
}

int
f2_glen_force(T *q, Skel *skel, const real *x, const real *y, real *u, real *v)
{
	return CO_OK;
}


