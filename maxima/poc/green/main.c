#include <math.h>
#include <stdio.h>
#include <real.h>

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

int
main()
{
    real q;
    real z;
    real z0;
    real W;

    q = 1.4;
    z = 1.2;
    z0 = 1.8;
    W = 1.0;
    printf("%g %g %g %g %g\n", tnn(q, z, z0, W), tnp(q, z, z0, W), tpn(q, z, z0, W), tpp(q, z, z0, W), rpp(q, z, z0, W));
}
