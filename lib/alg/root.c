#include <stdio.h>
#include <gsl/gsl_roots.h>
#include <gsl/gsl_errno.h>
#include "real.h"
#include "co/err.h"
#include "co/macro.h"
#include "co/memory.h"
#include "alg/root.h"

static const double EPSABS = 1e-10;
static const double EPSREL = 1e-8;
static const size_t NITER = 1000;

#define FMT CO_REAL_OUT

#define T AlgRoot

static int Type[] = { BISECTION, FALSEPOS, BRENT };

#define FILL \
	do { \
		i = 0; \
		Stype[i++] = gsl_root_fsolver_bisection; \
		Stype[i++] = gsl_root_fsolver_falsepos; \
		Stype[i++] = gsl_root_fsolver_brent; \
	} while(0);

struct Param {
    void *param;
     real(*function) (real, void *);
};
typedef struct Param Param;
static double
G(double x, void *vp)
{
    Param *p;

    p = vp;
    return p->function(x, p->param);
}

struct T {
    gsl_root_fsolver *s;
    int niter;
};

int
alg_root_ini(int type, T ** pq)
{
    T *q;
    int i, n;
    gsl_root_fsolver *s;
    const gsl_root_fsolver_type *Stype[999];

    FILL;
    MALLOC(1, &q);
    n = sizeof(Type) / sizeof(Type[0]);
    for (i = 0; /**/; i++)
        if (i == n)
            ERR(CO_INDEX, "unknown type: %d", type);
        else if (type == Type[i])
            break;
    s = gsl_root_fsolver_alloc(Stype[i]);
    if (s == NULL)
        ERR(CO_MEMORY, "fail to allocate root");
    q->s = s;
    *pq = q;
    return CO_OK;
}

int
alg_root_fin(T * q)
{
    gsl_root_fsolver_free(q->s);
    FREE(q);
    return CO_OK;
}

int
alg_root_apply(T * q, real lo, real hi, real(*f) (real, void *), void *p,
               real * r)
{
    gsl_function F;
    Param param;
    int i, status;
    gsl_root_fsolver *s;
    gsl_error_handler_t *h;

    s = q->s;
    param.function = f;
    param.param = p;
    F.function = G;
    F.params = &param;

    h = gsl_set_error_handler_off();
    status = gsl_root_fsolver_set(q->s, &F, lo, hi);
    if (status != GSL_SUCCESS) {
        MSG("lo, hi: " FMT " " FMT, lo, hi);
        MSG("f(lo), f(hi) " FMT " " FMT, f(lo, p), f(hi, p));
        ERR(CO_NUM, "staus: %s", gsl_strerror(status));
    }

    do {
        i++;
        status = gsl_root_fsolver_iterate(s);
        if (status != GSL_SUCCESS)
            ERR(CO_NUM, "staus: %s", gsl_strerror(status));
        *r = gsl_root_fsolver_root(s);
        lo = gsl_root_fsolver_x_lower(s);
        hi = gsl_root_fsolver_x_upper(s);
        status = gsl_root_test_interval(lo, hi, EPSABS, EPSREL);
        if (status == GSL_SUCCESS)
            break;
    } while (status == GSL_CONTINUE && i < NITER);
    q->niter = i;
    gsl_set_error_handler(h);

    return CO_OK;
}

const char *
alg_root_name(T * q)
{
    return gsl_root_fsolver_name(q->s);
}

int
alg_root_niter(T * q)
{
    return q->niter;
}
