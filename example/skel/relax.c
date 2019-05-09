#include <stdio.h>
#include <tgmath.h>
#include <real.h>
#include <co/err.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/skel.h>

#define FMT CO_REAL_OUT

int
main(__UNUSED int argc, char **argv)
{
	Skel *skel;
	real *x, *y, *vx, *vy, *fx, *fy;
	char file[9999];
	FILE *f;
	int n, i, nstep = 100;

	skel_read(stdin, &x, &y, &skel);
	n = skel_nv(skel);

	CALLOC(n, &vx);
	CALLOC(n, &vy);
	CALLOC(n, &fx);
	CALLOC(n, &fy);

	for (i = 0; i < nstep; i++) {
		sprintf(file, "%05d.skel", i);
		f = fopen(file, "w");
		skel_write(skel, x, y, f);
		fclose(f);
	}

	FREE(vx);
	FREE(vy);
	FREE(fx);
	FREE(fy);
	skel_xy_fin(x, y, skel);
}
