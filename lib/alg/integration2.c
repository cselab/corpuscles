#include <stdio.h>
#include "real.h"
#include "co/err.h"
#include "co/memory.h"
#include "co/macro.h"
#include "alg/integration.h"
#include "alg/integration2.h"

#define T AlgIntegration2

struct Fparam {
    void *param;
     real(*v0) (real, void *);
     real(*v1) (real, void *);
     real(*f) (real, real, void *);
    AlgIntegration *integ;
};

struct Gparam {
    void *param;
    real u;
     real(*f) (real, real, void *);
};

static real
G(real v, void *pv)
{
    struct Gparam *param;

    param = pv;
    return param->f(param->u, v, param->param);
}

static real
F(real u, void *vp)
{
    struct Fparam *fparam;
    struct Gparam gparam;
    real v0, v1, res;
    int status;

    fparam = vp;
    v0 = fparam->v0(u, fparam->param);
    v1 = fparam->v1(u, fparam->param);

    gparam.f = fparam->f;
    gparam.u = u;
    gparam.param = fparam->param;
    status =
        alg_integration_apply(fparam->integ, v0, v1, G, &gparam, &res);
    if (status != CO_OK)
        ERR(CO_NUM, "alg_integration_apply failed");
    return res;
}

struct T {
    AlgIntegration *iu, *iv;
};

int
alg_integration2_ini(int type, T ** pq)
{
    int status;
    T *q;

    MALLOC(1, &q);
    status = alg_integration_ini(type, &q->iu);
    if (status != CO_OK)
        ERR(CO_MEMORY, "alg_integration_ini failed");
    status = alg_integration_ini(type, &q->iv);
    if (status != CO_OK)
        ERR(CO_MEMORY, "alg_integration_ini failed");
    *pq = q;
    return CO_OK;
}

int
alg_integration2_fin(T * q)
{
    alg_integration_fin(q->iu);
    alg_integration_fin(q->iv);
    FREE(q);
    return CO_OK;
}

int
alg_integration2_apply(T * q, real u0, real u1,
                       real(*v0) (real u, void *), real(*v1) (real u,
                                                              void *),
                       real(*f) (real u, real v, void *), void *p,
                       /**/ real * res)
{
    int status;
    struct Fparam param;

    param.f = f;
    param.v0 = v0;
    param.v1 = v1;
    param.param = p;
    param.integ = q->iv;
    status = alg_integration_apply(q->iu, u0, u1, F, &param, res);
    if (status != CO_OK)
        ERR(CO_NUM, "alg_integration_apply failed");
    return CO_OK;
}
