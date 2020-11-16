#include <stdio.h>
#include <stdlib.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_odeiv2.h>
#include "real.h"
#include "co/err.h"
#include "co/memory.h"
#include "co/macro.h"
#include "alg/ode.h"

#define FMT CO_REAL_OUT
static const real EPSABS = 1e-8;
static const real EPSREL = 1e-8;

#define T Ode
typedef struct Param Param;
struct Param {
    void *params;
    real *x, *g;
    int dim;
    int (*f)(real, const real *, real * dy, void *);
};
struct T {
    gsl_odeiv2_driver *d;
    gsl_odeiv2_system sys;
    double *y;
    Param params;
};
static int env(const char *, real *);

static int Type[] = {
    RK2,
    RK4,
    RK8PD,
    RKCK,
    RKF45,
};

#define FILL \
	do { \
		i = 0; \
		Stype[i++] = gsl_odeiv2_step_rk2;\
		Stype[i++] = gsl_odeiv2_step_rk4;\
		Stype[i++] = gsl_odeiv2_step_rk8pd;\
		Stype[i++] = gsl_odeiv2_step_rkck;\
		Stype[i++] = gsl_odeiv2_step_rkf45;\
	} while(0);

static int
F(double t, const double y[], double f[], void *p0)
{
    Param *p;
    real *x, *g;
    void *params;
    int i, status, dim;

    p = p0;
    x = p->x;
    g = p->g;
    dim = p->dim;
    params = p->params;
    for (i = 0; i < dim; i++)
        x[i] = y[i];
    status = p->f(t, x, g, params);
    for (i = 0; i < dim; i++)
        f[i] = g[i];
    if (status == CO_OK)
        return GSL_SUCCESS;
    else
        return GSL_EINVAL;
}

int
ode_ini(int type, int dim, real dt,
        int (*f)(real, const real *, real * f, void *), void *param,
        T ** pq)
{
    T *q;
    int i, n;
    real epsabs, epsrel;
    gsl_odeiv2_driver *d;
    gsl_odeiv2_system *sys;
    const gsl_odeiv2_step_type *Stype[99];
    Param *p;

    FILL;
    MALLOC(1, &q);
    n = sizeof(Type) / sizeof(Type[0]);
    i = 0;
    while (1) {
        if (i == n)
            ERR(CO_INDEX, "unknown type %d", type);
        if (Type[i] == type)
            break;
        i++;
    }
    MALLOC(dim, &q->y);
    sys = &q->sys;
    p = &q->params;
    MALLOC(dim, &p->x);
    MALLOC(dim, &p->g);
    p->dim = dim;
    p->f = f;
    p->params = param;
    sys->function = F;
    sys->jacobian = NULL;
    sys->dimension = dim;
    sys->params = p;
    epsabs = EPSABS;
    epsrel = EPSREL;
    if (env("EPSABS", &epsabs) != CO_OK)
        ERR(CO_INDEX, "fail to read EPSABS");
    if (env("EPSREL", &epsrel) != CO_OK)
        ERR(CO_INDEX, "fail to read EPSREL");
    MSG(FMT " " FMT, epsabs, epsrel);
    d = gsl_odeiv2_driver_alloc_y_new(sys, Stype[i], dt, epsabs, epsrel);
    if (d == NULL)
        ERR(CO_MEMORY, "fail to allocate ode driver");
    q->d = d;
    *pq = q;
    return CO_OK;
}

int
ode_fin(T * q)
{
    gsl_odeiv2_driver_free(q->d);
    FREE(q->y);
    FREE(q->params.x);
    FREE(q->params.g);
    FREE(q);
    return CO_OK;
}

static int
step_addaptive(T * q, double *time, double t, double *y)
{
    int status;

    status = gsl_odeiv2_driver_apply(q->d, time, t, y);
    return status == GSL_SUCCESS ? CO_OK : CO_NUM;
}

static int
step_fixed(T * q, double *time, double t, double *y)
{
    int status, nstep;
    double dt;

    nstep = 1;
    dt = t - *time;
    status = gsl_odeiv2_driver_apply_fixed_step(q->d, time, dt, nstep, y);
    return status == GSL_SUCCESS ? CO_OK : CO_NUM;
}

static int
apply(T * q, real * ptime, real t, real * x,
      int (*step)(T *, double *, double, double *))
{
    int status;
    double time, *y;
    int dim, i;

    y = q->y;
    dim = q->params.dim;
    time = *ptime;
    for (i = 0; i < dim; i++)
        y[i] = x[i];
    status = step(q, &time, t, y);
    if (status != CO_OK)
        ERR(CO_NUM, "step failed (time = " FMT ", dt = " FMT ")", time,
            t - time);
    for (i = 0; i < dim; i++)
        x[i] = y[i];
    *ptime = time;
    return status;
}

int
ode_apply(T * q, real * ptime, real t, real * x)
{
    return apply(q, ptime, t, x, step_addaptive);
}

int
ode_apply_fixed(T * q, real * ptime, real t, real * x)
{
    return apply(q, ptime, t, x, step_fixed);
}

static int
env(const char *name, real * p)
{
    const char *v;

    if ((v = getenv(name)) != NULL)
        if (sscanf(v, CO_REAL_IN, p) != 1)
            ERR(CO_IO, "not a number %s=%s", name, v);
    return CO_OK;
}
