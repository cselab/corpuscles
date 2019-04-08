#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

#include <real.h>

#include <co/array.h>
#include <co/err.h>
#include <co/memory.h>
#include <co/macro.h>

#include <alg/spline.h>
#include <alg/integration.h>

#define PI (3.141592653589793)
#define FMT CO_REAL_OUT

static AlgIntegration *integ;
static real R = 1;
static real d = 0.1;

static real p0;
static real p, t, (*E)(real, real, real);

static real
one(real r, real p, real t)
{
	return 1;
}

static real
f(real r, void *v)
{
	return r*r*sin(p)*E(r, t, p);
}

static real
g(real pp, void *v)
{
	real a, b, res;
	p = pp;
	a = -d/cos(p);
	b = R;
	alg_integration_apply(integ, a, b, f, NULL, &res);
	return res;
}

static real
h(real tt, void *v)
{
	real a, b, res;
	t = tt;
	a = p0;
	b = PI;
	alg_integration_apply(integ, a, b, g, NULL, &res);
	return res;
}

static real
q(void *v)
{
	real a, b, res;
	a = 0;
	b = 2*PI;
	alg_integration_apply(integ, a, b, h, NULL, &res);
	return res;	
}
 
int
main(void)
{
	real ans;

	p0 = acos(-d/R);
	E = one;
	alg_integration_ini(GAUSS61, &integ);
	ans = q(NULL);
	printf(FMT "\n", ans);
	alg_integration_fin(integ);
}
