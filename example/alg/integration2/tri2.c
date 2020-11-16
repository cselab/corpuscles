#include <stdio.h>
#include <real.h>

#include <co/err.h>
#include <alg/integration.h>
#include <alg/integration2.h>

#define FMT CO_REAL_OUT

static int cnt;
static real ax, ay, az, bx, by, bz, cx, cy, cz;

static real
f(real u, real v, void *p)
{
    real x, y, z;

    x = (cx - ax) * v + (bx - ax) * u + ax;
    y = (cy - ay) * v + (by - ay) * u + ay;
    z = (cz - az) * v + (bz - az) * u + az;
    cnt++;
    return x * y;
}

static real
v0(real u, void *p)
{
    return 0;
}

static real
v1(real u, void *p)
{
    return 1 - u;
}

int
main(void)
{
    AlgIntegration2 *integ;
    real u0, u1, result, alpha;

    alg_integration2_ini(GAUSS31, &integ);
    u0 = 0;
    u1 = 1;
    ax = 0;
    ay = 0;
    az = 0;
    bx = 1;
    by = 0;
    bz = 0;
    cx = 0;
    cy = 1;
    cz = 1;
    alg_integration2_apply(integ, u0, u1, &v0, &v1, &f, NULL, &result);
    printf(FMT "\n", result);
    printf("%d\n", cnt);
    alg_integration2_fin(integ);
}
