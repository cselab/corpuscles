#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

#include <real.h>
#include <co/err.h>
#include <co/memory.h>
#include <co/macro.h>
#include <co/vec.h>
#include <co/kernel.h>
#include <co/pre/cons.h>

#define FMT CO_REAL_OUT

/*
	r/|r| * dw = z*dwr
	R, Fz(param, r), param
	r, normal, point return Force

*/

int
main(void)
{
	enum {
		X, Y, Z
	};
	real R;
	Kernel *kernel;
	PreCons *pre;
	real r[3], point[3], norm[3];
	real lo, hi, x, y;
	int i, n;

	n = 100;
	R = 2;
	lo = 0;
	hi = R;
	kernel_ini(KERNEL_2D, KERNEL_YANG, &kernel);
	pre_cons2_kernel_ini(R, kernel, &pre);
	point[X] = point[Y] = point[Z] = 0;
	r[Y] = 0;
	r[Z] = 0;
	norm[X] = 1;
	norm[Y] = 0;
	norm[Z] = 0;
	for (i = 0; i < n; i++) {
		x = lo + (hi - lo)/(n - 1)*i;
		r[X] = x;
		pre_cons_apply(pre, r, point, norm, &y);
		printf(FMT " " FMT "\n", x, y);
	}
	pre_cons_fin(pre);
	kernel_fin(kernel);
}
