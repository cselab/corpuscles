#include <stdlib.h>
#include <real.h>
#include <co/err.h>
#include <co/memory.h>

#include "fmm3.h"
#include "stfmm3.h"

#define T FMM3
struct T
{
    int n;
    double *source, *sigma_sl, *vel, *pre;
};

int
fmm3_ini(int n, T **pq)
{
    T *q;
    MALLOC(n, &q);
    q->n = n;
    MALLOC(3*n, &q->source);
    MALLOC(3*n, &q->sigma_sl);
    MALLOC(3*n, &q->vel);
    MALLOC(n, &q->pre);
    *pq = q;
    return CO_OK;
}

int
fmm3_fin(T *q)
{
    FREE(q->source);
    FREE(q->sigma_sl);
    FREE(q->vel);
    FREE(q->pre);
    return CO_OK;
}

int
fmm3_single(T *q,
	    const real *x, const real *y, const real *z,
	    const real *fx, const real *fy, const real *fz,
	    /*io*/ real *vx, real *vy, real *vz)
{
    int n, i, j;
    double *source, *sigma_sl, *vel, *pre;
    int ier, iprec, ifsingle, ifdouble, ifvel, ifgrad;
    double *sigma_dl, *sigma_dv, *grad;

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

    iprec = 3; /* < 0.5e-9 */
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

    for (i = j = 0; i < n; i++) {
	vx[i] += vel[j++];
	vy[i] += vel[j++];
	vz[i] += vel[j++];
    }

    return CO_OK;
}