#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <real.h>
#include <co/err.h>
#include <co/memory.h>
#include <co/macro.h>
#include <co/vec.h>
#include <co/kernel.h>
#include <co/pre/visc.h>

#define FMT CO_REAL_OUT

int
main(void)
{
	enum {
		X, Y, Z
	};
	real R, beta;
	Kernel *kernel;
	PreVisc *pre_visc;
	real r[3], point[3], norm[3];
	real lo, hi, x, y;
	int i, n;

	n = 100;
	R = 1;
	beta = 0.01;
	lo = 0;
	hi = R;
	kernel_ini(KERNEL_2D, KERNEL_QUINTIC, &kernel);
	pre_visc2_kernel_ini(R, beta, kernel, &pre_visc);
	point[X] = point[Y] = point[Z] = 0;
	r[Y] = 0.2;
	r[Z] = 0.2;
	norm[X] = 1;
	norm[Y] = 0;
	norm[Z] = 0;
	for (i = 0; i < n; i++) {
		x = lo + (hi - lo)/(n - 1)*i;
		r[X] = x;
		pre_visc_apply(pre_visc, r, point, norm, &y);
		printf(FMT " " FMT "\n", x, y);
	}
	pre_visc_fin(pre_visc);
	kernel_fin(kernel);
}
