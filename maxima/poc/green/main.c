#include <math.h>
#include <stdio.h>

#include <real.h>
#include <co/err.h>
#include <alg/integration.h>
#include <alg/special.h>

#define FMT CO_REAL_OUT

static const real qmax = 100;
struct Param {
    real(*j) (real);
    real s;
    real z;
    real z0;
    real W;
     real(*f) (real, real, real, real);
};

struct Input {
    real x;
    real y;
    real z;
    real z0;
    real W;
};

static real
F(real q, void *v)
{
    real s;
    real z;
    real z0;
    real W;
    struct Param *p;

    real(*f) (real, real, real, real);
    real(*j) (real);

    p = v;
    j = p->j;
    s = p->s;
    z = p->z;
    z0 = p->z0;
    W = p->W;
    f = p->f;
    return j(q * s) * f(q, z, z0, W);
}

static real
coth(real x)
{
    return 1 / tanh(x);
}

static real
fAp(real u)
{
    return u + sinh(u) * exp(-u);
}

static real
fAm(real u)
{
    return u - sinh(u) * exp(-u);
}

static real
fBp(real u)
{
    return u + cosh(u) * exp(-u);
}

static real
fBm(real u)
{
    return u - cosh(u) * exp(-u);
}

static real
fCp(real u)
{
    return u * (1 + u) + sinh(u) * exp(-u);
}

static real
fCm(real u)
{
    return u * (1 + u) - sinh(u) * exp(-u);
}

static real
fDp(real u)
{
    return u * (1 + u) + cosh(u) * exp(-u);
}

static real
fDm(real u)
{
    return u * (1 + u) - cosh(u) * exp(-u);
}

static real
fEp(real u)
{
    return 1 / (sinh(u) * cosh(u) + u);
}

static real
fEm(real u)
{
    return 1 / (sinh(u) * cosh(u) - u);
}

static real
tnn(real q, real z, real z0, real W)
{
    real u;
    real v;
    real w;
    real Ap;
    real Bp;
    real Cp;
    real Dp;
    real Ep;
    real Em;

    u = W * q;
    v = q * z0;
    w = q * z;
    Ap = fAp(u);
    Bp = fBp(u);
    Cp = fCp(u);
    Dp = fDp(u);
    Ep = fEp(u);
    Em = fEm(u);
    return Ep * (Ap * cosh(v) - v * sinh(v)) * w * sinh(w) +
        Em * (Bp * v * cosh(v) - Dp * sinh(v)) * sinh(w) +
        Em * (Bp * sinh(v) - v * cosh(v)) * w * cosh(w) +
        Ep * (Ap * v * sinh(v) - Cp * cosh(v)) * cosh(w);
}

static real
tnp(real q, real z, real z0, real W)
{
    real u;
    real v;
    real w;
    real Ap;
    real Am;
    real Bp;
    real Bm;
    real Ep;
    real Em;

    u = W * q;
    u = W * q;
    v = q * z0;
    w = q * z;
    Ap = fAp(u);
    Am = fAm(u);
    Bp = fBp(u);
    Bm = fBm(u);
    Ep = fEp(u);
    Em = fEm(u);
    return Ep * (v * cosh(v) - Bm * sinh(v)) * w * sinh(w) +
        Em * (u * u * cosh(v) - Bp * v * sinh(v)) * sinh(w) +
        Em * (v * sinh(v) - Am * cosh(v)) * w * cosh(w) +
        Ep * (u * u * sinh(v) - Ap * v * cosh(v)) * cosh(w);
}

static real
tpn(real q, real z, real z0, real W)
{
    real u;
    real v;
    real w;
    real Ap;
    real Am;
    real Bp;
    real Bm;
    real Ep;
    real Em;

    u = W * q;
    u = W * q;
    v = q * z0;
    w = q * z;
    Ap = fAp(u);
    Am = fAm(u);
    Bp = fBp(u);
    Bm = fBm(u);
    Ep = fEp(u);
    Em = fEm(u);
    return Em * (Bp * sinh(v) - v * cosh(v)) * w * sinh(w) +
        Ep * (Bm * v * sinh(v) - u * u * cosh(v)) * sinh(w) +
        Ep * (Ap * cosh(v) - v * sinh(v)) * w * cosh(w) +
        Em * (Am * v * cosh(v) - u * u * sinh(v)) * cosh(w);
}

static real
tpp(real q, real z, real z0, real W)
{
    real u;
    real v;
    real w;
    real Am;
    real Bm;
    real Cm;
    real Dm;
    real Ep;
    real Em;

    u = W * q;
    u = W * q;
    v = q * z0;
    w = q * z;
    Am = fAm(u);
    Bm = fBm(u);
    Cm = fCm(u);
    Dm = fDm(u);
    Ep = fEp(u);
    Em = fEm(u);
    return Em * (v * sinh(v) - Am * cosh(v)) * w * sinh(w) +
        Ep * ((-2 * u * coth(u) * sinh(v)) + Dm * sinh(v) -
              Bm * v * cosh(v)) * sinh(w) + Ep * (v * cosh(v) -
                                                  Bm * sinh(v)) * w *
        cosh(w) + Em * ((-Am * v * sinh(v)) - 2 * u * tanh(u) * cosh(v) +
                        Cm * cosh(v)) * cosh(w);
}

static real
rpp(real q, real z, real z0, real W)
{
    real u;
    real v;
    real w;

    u = W * q;
    u = W * q;
    v = q * z0;
    w = q * z;
    return (-(2 * exp(-u) * sinh(v) * sinh(w)) / sinh(u)) -
        (2 * exp(-u) * cosh(v) * cosh(w)) / cosh(u);
}

static int
f_xx_yy(struct Input *i, AlgIntegration * integration, real * xx, real * yy)
{
    real a;
    real b;
    real c;
    real s2;
    real x2;
    real y2;
    struct Param p;

    x2 = i->x * i->x;
    y2 = i->y * i->y;
    s2 = x2 + y2;
    if (s2 == 0)
        ERR(CO_NUM, "s2 == 0");
    p.s = sqrt(s2);
    p.z = i->z;
    p.z0 = i->z0;
    p.W = i->W;
    p.f = tpp;
    p.j = alg_special_bessel_J2;
    if (alg_integration_apply(integration, 0, qmax, F, &p, &b) != CO_OK)
        ERR(CO_NUM, "alg_integration_apply failed");
    p.j = alg_special_bessel_J0;
    if (alg_integration_apply(integration, 0, qmax, F, &p, &a) != CO_OK)
        ERR(CO_NUM, "alg_integration_apply failed");
    p.f = rpp;
    if (alg_integration_apply(integration, 0, qmax, F, &p, &c) != CO_OK)
        ERR(CO_NUM, "alg_integration_apply failed");
    *xx = -0.5 * a - 0.5 * (y2 - x2) / s2 * b + c;
    *yy = -0.5 * a - 0.5 * (x2 - y2) / s2 * b + c;
    return CO_OK;
}

static int
f_zz(struct Input *i, AlgIntegration * integration, real * result)
{
    real s2;
    real x2;
    real y2;
    struct Param p;

    x2 = i->x * i->x;
    y2 = i->y * i->y;
    s2 = x2 + y2;
    if (s2 == 0)
        ERR(CO_NUM, "s2 == 0");
    p.s = sqrt(s2);
    p.z = i->z;
    p.z0 = i->z0;
    p.W = i->W;
    p.j = alg_special_bessel_J0;
    p.f = tnn;
    if (alg_integration_apply(integration, 0, qmax, F, &p, result) != CO_OK)
        ERR(CO_NUM, "alg_integration_apply failed");
    return CO_OK;
}

static int
f_xy(struct Input *i, AlgIntegration * integration, real * result)
{
    real s2;
    real x2;
    real y2;
    struct Param p;

    x2 = i->x * i->x;
    y2 = i->y * i->y;
    s2 = x2 + y2;
    if (s2 == 0)
        ERR(CO_NUM, "s2 == 0");
    p.s = sqrt(s2);
    p.z = i->z;
    p.z0 = i->z0;
    p.W = i->W;
    p.j = alg_special_bessel_J2;
    p.f = tpp;
    if (alg_integration_apply(integration, 0, qmax, F, &p, result) != CO_OK)
        ERR(CO_NUM, "alg_integration_apply failed");

    *result *= (i->x * i->y) / s2;
    return CO_OK;
}

static int
f_xz_yz(struct Input *i, AlgIntegration * integration, real * xz, real * yz)
{
    real s2;
    real x2;
    real y2;
    real s;
    real result;
    struct Param p;

    x2 = i->x * i->x;
    y2 = i->y * i->y;
    s2 = x2 + y2;
    if (s2 == 0)
        ERR(CO_NUM, "s2 == 0");
    p.s = s = sqrt(s2);
    p.z = i->z;
    p.z0 = i->z0;
    p.W = i->W;
    p.j = alg_special_bessel_J1;
    p.f = tpn;
    if (alg_integration_apply(integration, 0, qmax, F, &p, &result) != CO_OK)
        ERR(CO_NUM, "alg_integration_apply failed");
    *xz = - (i->x) * result  / s;
    *yz = - (i->y) * result  / s;
    return CO_OK;
}

static int
f_zx_zy(struct Input *i, AlgIntegration * integration, real * zx, real * zy)
{
    real s2;
    real x2;
    real y2;
    real s;
    real result;
    struct Param p;

    x2 = i->x * i->x;
    y2 = i->y * i->y;
    s2 = x2 + y2;
    if (s2 == 0)
        ERR(CO_NUM, "s2 == 0");
    p.s = s = sqrt(s2);
    p.z = i->z;
    p.z0 = i->z0;
    p.W = i->W;
    p.j = alg_special_bessel_J1;
    p.f = tnp;
    if (alg_integration_apply(integration, 0, qmax, F, &p, &result) != CO_OK)
        ERR(CO_NUM, "alg_integration_apply failed");
    *zx = - (i->x) * result  / s;
    *zy = - (i->y) * result  / s;
    return CO_OK;
}

int
main()
{
    real xx;
    real yy;
    real zz;
    real xy;
    real xz;
    real yz;
    real zx;
    real zy;
    struct Input i;
    AlgIntegration *integration;

    i.x = 0.1;
    i.y = 0.2;
    i.z = 0.3;
    i.z0 = 0.4;
    i.W = 1.0;
    alg_integration_ini(QAGS, &integration);

    f_xx_yy(&i, integration, &xx, &yy);
    f_zz(&i, integration, &zz);
    f_xy(&i, integration, &xy);
    f_xz_yz(&i, integration, &xz, &yz);
    f_xz_yz(&i, integration, &zx, &zy);
    printf(FMT " " FMT " " FMT "\n", xx, yy, zz);
    printf(FMT " " FMT " " FMT "\n", xy, xz, yz);
    printf(FMT " " FMT "\n", zx, zy);

    alg_integration_fin(integration);
}
