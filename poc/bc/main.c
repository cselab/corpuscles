#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

#include <real.h>

#include <co/err.h>
#include <co/macro.h>

#include <alg/spline.h>
#include <alg/integration.h>

#define PI (3.141592653589793)
#define FMT CO_REAL_OUT

struct Q
{
	AlgIntegration *integ;
	real R, d;
	real p0, p, t;
	void *param;
	real (*E)(real, real, real, void*);
};
typedef struct Q Q;
static Q qq;

static real
one(real r, real p, real t, void *param)
{
	real *alpha;
	alpha = param;
	return *alpha;
}

static real
f(real r, void *v)
{
	void *param;
	real p, t;
	p = qq.p;
	t = qq.t;
	param = qq.param;
	return r*r*sin(p)*qq.E(r, t, p, param);
}

static real
g(real pp, void *v)
{
	real a, b, res;
	qq.p = pp;
	a = -qq.d/cos(qq.p);
	b = qq.R;
	alg_integration_apply(qq.integ, a, b, f, NULL, &res);
	return res;
}

static real
h(real tt, void *v)
{
	real a, b, res;
	qq.t = tt;
	a = qq.p0;
	b = PI;
	alg_integration_apply(qq.integ, a, b, g, NULL, &res);
	return res;
}

static real
q(void *v)
{
	real a, b, res;
	a = 0;
	b = 2*PI;
	alg_integration_apply(qq.integ, a, b, h, NULL, &res);
	return res;	
}
 
int
main(void)
{
	real ans, alpha;
	qq.R = 1;
	qq.d = 0.1;
	qq.p0 = acos(-qq.d/qq.R);
	qq.E = one;
	alg_integration_ini(QNG, &qq.integ);

	alpha = 10;
	qq.param = &alpha;


	ans = q(NULL);
	printf(FMT "\n", ans);
	alg_integration_fin(qq.integ);
}
