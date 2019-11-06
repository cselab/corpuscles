#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <real.h>

#include <co/arg.h>
#include <co/array.h>
#include <co/err.h>
#include <co/kernel.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/sum.h>

#include <alg/rng.h>

#define FMT CO_REAL_OUT

static const char me[]  = "mc";
static int N = 100000;
static real rho = 1.0;
static real lo = -1;
static real hi = -1;
static real R = 1;
static real d;
static real (*f)(real, real, real, real);

static real
sq(real x)
{
	return x*x;
}

static real
wc(real r)
{
	return 1 - r/R;
}

static real
wd(real r)
{
	return sq(1 - r/R);
}

static real
fc(real r, real x, real y, real z)
{
	return wc(r)*z/r;
}

static real
fdx(real r, real x, real y, real z)
{
	real d0;
	d0 = d < 0.1 ? 0.1 : d;
	return wd(r)/r*(1 - z/d0);
}

static real
fdz(real r, real x, real y, real z)
{
	return -1/d*wd(r)* sq(z/r) * z;
}

char *argv0;

void
usg(void)
{
	fprintf(stderr, "%s: -d [distance to the wall]\n", me);
}

real
mc(real d0)
{
	AlgRng *rng;
	HeSum *sum;
	int i;
	real x, y, z, s, r2, r, V, l, g;

	d = d0;
	lo = -1; hi = 1;
	he_sum_ini(&sum);
	alg_rng_ini(&rng);
	for (i = 0, s = 0; i < N; i++) {
		x = alg_rng_uniform(rng, lo, hi);
		y = alg_rng_uniform(rng, lo, hi);
		z = alg_rng_uniform(rng, lo, hi);
		if (z > -d) continue;
		r2 = x*x + y*y + z*z;
		if (r2 > R*R) continue;
		r = sqrt(r2);
		g = f(r, x, y, z);
		he_sum_add(sum, g);
	}
	l = hi - lo;
	V = l*l*l;
	s = V*he_sum_get(sum)/N;	
	he_sum_fin(sum);
	alg_rng_fin(rng);
	return s;
}

int
main(int argc, char **argv)
{
	int i, n = 200;
	real d, lo, hi;
	f = fc;
	ARGBEGIN {
	case 'c':
		f = fc;
		break;
	case 'z':
		f = fdz;
		break;
	case 'x':
		f = fdx;
		break;	
	} ARGEND
	lo = 0; hi = 1;
	for (i = 0; i < n; i++) {
		d = lo + (hi - lo)/(n - 1)*i;
		printf(FMT "\n", mc(d));
	}
}
