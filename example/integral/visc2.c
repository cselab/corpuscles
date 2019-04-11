#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

#include <real.h>
#include <co/err.h>
#include <co/memory.h>
#include <co/macro.h>
#include <co/vec.h>
#include <co/kernel.h>
#include <co/pre/visc.h>

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
	real R, beta;
	Kernel *kernel;
	PreVisc *pre_visc;
	real r[3], point[3], norm[3], f[3];
	R = 1;
	beta = 0.1;
	kernel_ini(KERNEL_3D, KERNEL_QUINTIC, &kernel);
	pre_visc_kernel_ini(R, beta, kernel, &pre_visc);
	point[X] = point[Y] = point[Z] = 0;
	r[X] = 0.2;
	r[Y] = 0.2;
	r[Z] = 0.2;
	norm[X] = 1;
	norm[Y] = 0;
	norm[Z] = 0;
	pre_visc_apply(pre_visc, r, point, norm, f);
	vec_printf(f, FMT);
	pre_visc_fin(pre_visc);
	kernel_fin(kernel);
}
