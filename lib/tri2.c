#include <stdio.h>
#include <tgmath.h>
#include "real.h"
#include "co/vec2.h"
#include "co/tri2.h"
#include "co/predicate.h"

static const real pi = 3.141592653589793115997964;

real
tri2_angle(const real a[2], const real b[2], const real c[2])
{
    real u[2], v[2];

    vec2_minus(a, b, u);
    vec2_minus(c, b, v);
    return fabs(vec2_angle(u, v));
}

real
tri2_angle_sup(const real a[2], const real b[2], const real c[2])
{
    real s;

    s = pi - tri2_angle(a, b, c);
    return predicate_orient2d(a, c, b) > 0 ? s : -s;
}
