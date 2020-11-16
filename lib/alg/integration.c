#include <stdio.h>
#include <string.h>
#include <gsl/gsl_integration.h>
#include <gsl/gsl_errno.h>

#include "real.h"
#include "co/err.h"
#include "co/memory.h"
#include "co/macro.h"
#include "alg/integration.h"

static const char *String[] =
    { "gauss15", "gauss21", "gauss31", "gauss61", "qng", "qags" };

#define T AlgIntegration
enum {
    N = 10000
};
static const double EPSABS = 0;
static const double EPSREL = 1e-3;
static const size_t LIMIT = N;

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
    int key;
    gsl_integration_workspace *w;
    int (*fin)(T *);
    int (*apply)(T *, real, real, real(*)(real, void *), void *, real *);
};

static int
ini_qag(T * q)
{
    q->w = gsl_integration_workspace_alloc(N);
    if (q->w == NULL)
        ERR(CO_MEMORY, "fail to allocate");
    return CO_OK;
}

static int
fin_qag(T * q)
{
    gsl_integration_workspace_free(q->w);
    return CO_OK;
}

static int
apply_qag(T * q, real a, real b, real(*f) (real, void *), void *p,
          real * result)
{
    gsl_function F;
    Param param;
    int status;
    double abserr, dresult;

    param.function = f;
    param.param = p;
    F.function = G;
    F.params = &param;
    status =
        gsl_integration_qag(&F, a, b, EPSABS, EPSREL, LIMIT, q->key, q->w,
                            &dresult, &abserr);
    if (status != GSL_SUCCESS)
        ERR(CO_NUM, "staus: %s", gsl_strerror(status));

    *result = dresult;
    return CO_OK;
}

static int
ini_qags(T * q)
{
    q->w = gsl_integration_workspace_alloc(N);
    if (q->w == NULL)
        ERR(CO_MEMORY, "fail to allocate");
    return CO_OK;
}

static int
fin_qags(T * q)
{
    gsl_integration_workspace_free(q->w);
    return CO_OK;
}

static int
apply_qags(T * q, real a, real b, real(*f) (real, void *), void *p,
           real * result)
{
    gsl_function F;
    Param param;
    int status;
    double abserr, dresult;

    param.function = f;
    param.param = p;
    F.function = G;
    F.params = &param;
    status =
        gsl_integration_qags(&F, a, b, EPSABS, EPSREL, LIMIT, q->w,
                             &dresult, &abserr);
    if (status != GSL_SUCCESS)
        ERR(CO_NUM, "staus: %s", gsl_strerror(status));

    *result = dresult;
    return CO_OK;
}


static int
apply_qng(__UNUSED T * q, real a, real b, real(*f) (real, void *), void *p,
          real * result)
{
    gsl_function F;
    Param param;
    int status;
    size_t neval;
    double abserr, dresult;

    param.function = f;
    param.param = p;
    F.function = G;
    F.params = &param;
    status =
        gsl_integration_qng(&F, a, b, EPSABS, EPSREL, &dresult, &abserr,
                            &neval);
    if (status != GSL_SUCCESS)
        ERR(CO_NUM, "staus: %s", gsl_strerror(status));

    *result = dresult;
    return CO_OK;
}


static int
ini_qng(__UNUSED T * q)
{
    return CO_OK;
}

static int
fin_qng(__UNUSED T * q)
{
    return CO_OK;
}

int
alg_integration_str2enum(const char *str, int *penum)
{
    int i;

    for (i = 0; i < sizeof(String) / sizeof(String[0]); i++)
        if (strncmp(str, String[i], 999) == 0) {
            *penum = i;
            return CO_OK;
        }
    ERR(CO_NUM, "unknown integration type '%s'", str);
}

int
alg_integration_ini(int type, T ** pq)
{
    T *q;
    int (*ini)(T *);

    MALLOC(1, &q);
    switch (type) {
    case QNG:
        ini = ini_qng;
        q->fin = fin_qng;
        q->apply = apply_qng;
        break;
    case QAGS:
        ini = ini_qags;
        q->fin = fin_qags;
        q->apply = apply_qags;
        break;
    case GAUSS15:
        q->key = GSL_INTEG_GAUSS15;
        ini = ini_qag;
        q->fin = fin_qag;
        q->apply = apply_qag;
        break;
    case GAUSS21:
        q->key = GSL_INTEG_GAUSS21;
        ini = ini_qag;
        q->fin = fin_qag;
        q->apply = apply_qag;
        break;
    case GAUSS31:
        q->key = GSL_INTEG_GAUSS31;
        ini = ini_qag;
        q->fin = fin_qag;
        q->apply = apply_qag;
        break;
    case GAUSS61:
        q->key = GSL_INTEG_GAUSS61;
        ini = ini_qag;
        q->fin = fin_qag;
        q->apply = apply_qag;
        break;
    default:
        ERR(CO_INDEX, "unknown integration type: %d", type);
    }

    ini(q);
    *pq = q;
    return CO_OK;
}

int
alg_integration_fin(T * q)
{
    q->fin(q);
    FREE(q);
    return CO_OK;
}

int
alg_integration_apply(T * q, real a, real b, real(*fun) (real, void *),
                      void *p, real * res)
{
    if (a > b)
        ERR(CO_NUM, "a > b");
    return q->apply(q, a, b, fun, p, res);
}
