#include <stdlib.h>
#include <real.h>
#include <co/err.h>
#include <co/memory.h>

#include "fm.h"
#include "stfmm3.h"

static real pi = 3.141592653589793;

#define T FM
struct T {
	int n;
	double *source, *sigma_sl, *sigma_dl, *sigma_dv, *vel, *pre;
};

int
fm_ini(int n, T ** pq)
{
	T *q;

	MALLOC(n, &q);
	q->n = n;
	MALLOC(3 * n, &q->source);
	MALLOC(3 * n, &q->sigma_sl);
	MALLOC(3 * n, &q->sigma_dl);
	MALLOC(3 * n, &q->sigma_dv);
	MALLOC(3 * n, &q->vel);
	MALLOC(n, &q->pre);
	*pq = q;
	return CO_OK;
}

int
fm_fin(T * q)
{
	FREE(q->source);
	FREE(q->sigma_sl);
	FREE(q->sigma_dl);
	FREE(q->sigma_dv);
	FREE(q->vel);
	FREE(q->pre);
	FREE(q);
	return CO_OK;
}

int
fm_single(T * q,
		  const real * x, const real * y, const real * z,
		  const real * fx, const real * fy, const real * fz,
		  /*io */ real * vx, real * vy, real * vz)
{
	int n, i, j;
	double *source, *sigma_sl, *vel, *pre;
	int ier, iprec, ifsingle, ifdouble, ifvel, ifgrad;
	double *sigma_dl, *sigma_dv, *grad;
	real coeff;

	n = q->n;
	source = q->source;
	sigma_sl = q->sigma_sl;
	vel = q->vel;
	pre = q->pre;
	for (i = j = 0; i < n; i++) {
		source[j++] = x[i];
		source[j++] = y[i];
		source[j++] = z[i];
	}
	for (i = j = 0; i < n; i++) {
		sigma_sl[j++] = fx[i];
		sigma_sl[j++] = fy[i];
		sigma_sl[j++] = fz[i];
	}
	iprec = 0;					/* < 0.5e-2 */
	ifsingle = 1;
	ifdouble = 0;
	sigma_dl = sigma_dv = grad = NULL;
	ifvel = 1;
	ifgrad = 0;
	stfmm3_dpartself(&ier, iprec, n, source,
					 ifsingle, sigma_sl, ifdouble, sigma_dl, sigma_dv,
					 ifvel, vel, pre, ifgrad, grad);
	if (ier != 0)
		ERR(CO_MEMORY, "stfmm3 fail to allocate (ier = %d, n = %d)",
			ier, n);
	coeff = 1 / (4 * pi);
	for (i = j = 0; i < n; i++) {
		vx[i] += coeff * vel[j++];
		vy[i] += coeff * vel[j++];
		vz[i] += coeff * vel[j++];
	}
	return CO_OK;
}

int
fm_double(T * q,
		  const real * x, const real * y, const real * z,
		  const real * ux, const real * uy, const real * uz,
		  const real * nx, const real * ny, const real * nz,
		  /*io */ real * vx, real * vy, real * vz)
{
	int n, i, j;
	double *source, *sigma_sl, *vel, *pre;
	int ier, iprec, ifsingle, ifdouble, ifvel, ifgrad;
	double *sigma_dl, *sigma_dv, *grad;
	real coeff;

	n = q->n;
	source = q->source;
	sigma_dl = q->sigma_dl;
	sigma_dv = q->sigma_dv;
	vel = q->vel;
	pre = q->pre;
	for (i = j = 0; i < n; i++) {
		source[j++] = x[i];
		source[j++] = y[i];
		source[j++] = z[i];
	}
	for (i = j = 0; i < n; i++) {
		sigma_dl[j++] = ux[i];
		sigma_dl[j++] = uy[i];
		sigma_dl[j++] = uz[i];
	}
	for (i = j = 0; i < n; i++) {
		sigma_dv[j++] = nx[i];
		sigma_dv[j++] = ny[i];
		sigma_dv[j++] = nz[i];
	}
	iprec = 0;					/* < 0.5e-2 */
	ifsingle = 0;
	ifdouble = 1;
	grad = NULL;
	sigma_sl = NULL;
	ifvel = 1;
	ifgrad = 0;
	stfmm3_dpartself(&ier, iprec, n, source,
					 ifsingle, sigma_sl, ifdouble, sigma_dl, sigma_dv,
					 ifvel, vel, pre, ifgrad, grad);
	if (ier != 0)
		ERR(CO_MEMORY, "stfmm3 fail to allocate (ier = %d, n = %d)",
			ier, n);
	coeff = 1 / (4 * pi);
	for (i = j = 0; i < n; i++) {
		vx[i] += coeff * vel[j++];
		vy[i] += coeff * vel[j++];
		vz[i] += coeff * vel[j++];
	}
	return CO_OK;
}
