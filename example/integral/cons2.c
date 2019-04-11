#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

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
	real size, R;
	Kernel *kernel;
	PreCons *pre_cons;
	real r[3], point[3], norm[3], f[3];

	R = 1;
	kernel_ini(KERNEL_3D, KERNEL_QUINTIC, &kernel);
	pre_cons_kernel_ini(R, kernel, &pre_cons);
	point[X] = point[Y] = point[Z] = 0;
	r[X] = 0.2;
	r[Y] = 0.2;
	r[Z] = 0.2;
	norm[X] = 1;
	norm[Y] = 0;
	norm[Z] = 0;
	pre_cons_apply(pre_cons, r, point, norm, f);
	vec_printf(f, FMT);
	pre_cons_fin(pre_cons);
	kernel_fin(kernel);
}
