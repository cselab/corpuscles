#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

#include <real.h>

#include <co/err.h>
#include <co/memory.h>

#define pi (3.141592653589793)
#define wendland6_1d (55/32.0)
#define wendland6_2d (78/(7*pi))
#define wendland6_3d (1365/(64*pi))

static real wendland6_w0(real q)
{
	if (q < 1) return pow(1-q,8)*(32*pow(q,3)+25*pow(q,2)+8*q+1);
	else return 0;
}

static real wendland6_dw0(real q)
{
	if (q < 1) return 22*pow(q-1,7)*q*(16*pow(q,2)+7*q+1);
	else return 0;
}

static real wendland6_w2(real c, real x)
{
	real q;
	q = fabs(x)/c;
	return wendland6_2d * wendland6_w0(q) / (c*c);
}

static real wendland6_dw2(real c, real x)
{
	real q;
	q = fabs(x)/c;
	return wendland6_2d * wendland6_dw0(q) / (c*c*c);
}

int main(void)
{
	return CO_OK;
}
