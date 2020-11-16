#include <stdio.h>
#include <gsl/gsl_multimin.h>

#include <real.h>
#include <co/err.h>
#include <co/memory.h>
#include <co/real.h>
#include <co/macro.h>

#include "alg/min.h"

#define T AlgMin

struct T {
    int n;
    gsl_multimin_fdfminimizer *min;
    gsl_multimin_function_fdf func;

    Real *position, *force;

    AlgMinF f;
    AlgMinDF df;
    void *param;
};

static double
f(const gsl_vector * v, void *vq)
{
    int n;
    T *q;
    const real *position, *x, *y, *z;

    q = (T *) vq;
    n = q->n;

    position = real_from(q->position, v->data);
    x = position;
    y = position + n;
    z = position + 2 * n;
    return (*q->f) (n, x, y, z, q->param);
}

static void
df(const gsl_vector * v, void *vq, gsl_vector * df)
{
    int n;
    T *q;
    const real *position, *x, *y, *z;
    real *force, *fx, *fy, *fz;

    q = (T *) vq;
    n = q->n;

    position = real_from(q->position, v->data);
    x = position;
    y = position + n;
    z = position + 2 * n;

    force = real_from(q->force, df->data);
    fx = force;
    fy = force + n;
    fz = force + 2 * n;
    (*q->df) (n, x, y, z, q->param, /**/ fx, fy, fz);

    df->data = real_to(q->force, force);
}

static void
fdf(const gsl_vector * x, void *params, double *f0, gsl_vector * df0)
{
    *f0 = f(x, params);
    df(x, params, df0);
}

int
alg_min_ini(int itype, AlgMinF f0, AlgMinDF df0, void *param,
            int n, real * xx, real * yy, real * zz, /**/ T ** pq)
{
    T *q;

    double tol, step_size;
    int i, j;
    const gsl_multimin_fdfminimizer_type *type;
    gsl_vector *position;

    switch (itype) {
    case CONJUGATE_FR:
        type = gsl_multimin_fdfminimizer_conjugate_fr;
        break;
    case CONJUGATE_PR:
        type = gsl_multimin_fdfminimizer_conjugate_pr;
        break;
    case VECTOR_BFGS2:
        type = gsl_multimin_fdfminimizer_vector_bfgs2;
        break;
    case VECTOR_BFGS:
        type = gsl_multimin_fdfminimizer_vector_bfgs;
        break;
    case STEEPEST_DESCENT:
        type = gsl_multimin_fdfminimizer_steepest_descent;
        break;
    default:
        ERR(CO_INDEX, "wrong algorithm type = %d", itype);
    }

    MALLOC(1, &q);

    position = gsl_vector_alloc(3 * n);
    real_ini(3 * n, &q->position);
    real_ini(3 * n, &q->force);

    q->n = n;
    q->f = f0;
    q->df = df0;
    q->param = param;

    q->func.n = 3 * n;
    q->func.f = f;
    q->func.df = df;
    q->func.fdf = fdf;
    q->func.params = q;

    j = 0;
    for (i = 0; i < n; i++)
        gsl_vector_set(position, j++, xx[i]);
    for (i = 0; i < n; i++)
        gsl_vector_set(position, j++, yy[i]);
    for (i = 0; i < n; i++)
        gsl_vector_set(position, j++, zz[i]);

    q->min = gsl_multimin_fdfminimizer_alloc(type, 3 * n);
    gsl_multimin_fdfminimizer_set(q->min, &q->func,
                                  position, step_size = 0.01, tol = 0.1);
    *pq = q;
    gsl_vector_free(position);
    return CO_OK;
}

int
alg_min_fin(T * q)
{
    gsl_multimin_fdfminimizer_free(q->min);
    real_fin(q->position);
    real_fin(q->force);
    FREE(q);
    return CO_OK;
}

int
alg_min_iterate(T * q)
{
    return gsl_multimin_fdfminimizer_iterate(q->min);
}

int
alg_min_force(T * q, real ** pfx, real ** pfy, real ** pfz)
{
    int n;
    real *force, *fx, *fy, *fz;
    gsl_vector *v;

    n = q->n;

    v = gsl_multimin_fdfminimizer_gradient(q->min);
    force = real_from(q->force, v->data);

    fx = force;
    fy = force + n;
    fz = force + 2 * n;

    *pfx = fx;
    *pfy = fy;
    *pfz = fz;

    return CO_OK;
}

int
alg_min_position(T * q, /**/ real ** px, real ** py, real ** pz)
{
    int n;
    real *position, *x, *y, *z;
    gsl_vector *v;

    n = q->n;
    v = gsl_multimin_fdfminimizer_x(q->min);
    position = real_from(q->position, v->data);

    x = position;
    y = position + n;
    z = position + 2 * n;
    *px = x;
    *py = y;
    *pz = z;
    return CO_OK;
}

real
alg_min_energy(T * q)
{
    return gsl_multimin_fdfminimizer_minimum(q->min);
}

int
alg_min_end(T * q)
{
    real eps;

    eps = 1e-3;
    return GSL_SUCCESS == gsl_multimin_test_gradient(q->min->gradient,
                                                     eps);
}
