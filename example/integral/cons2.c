#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <real.h>
#include <co/arg.h>
#include <co/err.h>
#include <co/kernel.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/pre/cons.h>
#include <co/vec.h>

#define FMT CO_REAL_OUT

int
main(void)
{
	enum
	{
		X, Y, Z
	};
	real R;
	Kernel *kernel;
	PreCons *pre_cons;
	real r[3], point[3], norm[3], f[3];
	int n, i;
	real x, y, lo, hi;

	n = 100;
	R = 1;
	lo = 0;
	hi = R;
	kernel_ini(KERNEL_3D, KERNEL_YANG, &kernel);
	pre_cons_kernel_ini(R, kernel, &pre_cons);
	point[X] = point[Y] = point[Z] = 0;

	r[Y] = 0.2;
	r[Z] = 0.2;
	norm[X] = 1;
	norm[Y] = 0;
	norm[Z] = 0;

	for (i = 0; i < n; i++) {
		x = lo + (hi - lo)/(n - 1)*i;
		r[X] = x;
		pre_cons_apply(pre_cons, r, point, norm, f);
		y = f[X];
		printf(FMT " " FMT "\n", x, y);
	}
	pre_cons_fin(pre_cons);
	kernel_fin(kernel);
}
