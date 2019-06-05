#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <real.h>

#include <co/force2.h>
#include <co/punto.h>
#include <co/skel.h>
#include <co/memory.h>
#include <co/err.h>
#include <co/util.h>
#include <co/argv.h>
#include <co/macro.h>
#include <co/fd2.h>

#define FMT_IN   CO_REAL_IN

static const char **argv;

static char name[1024];

static real *fx, *fy, *x, *y;
static real *gx, *gy;
static int nv;
static Skel *skel;
static Force2 *force;
static real delta = 1e-6;

static const char *me = "force2/generic";

static void
usg(void)
{
	const char *list;
	list = force2_list();
	fprintf(stderr, "%s %s [args..] < OFF > PUNTO\n", me, list);
	exit(2);
}

static void
main0()
{
	int i;
	real e, r[3], f[3];

	force2_force(force, skel, x, y, /**/ fx, fy);
	fd2(force, skel, delta, x, y, /**/ gx, gy);
	e = force2_energy(force, skel, x, y);

	MSG("name: %s", force2_name(force));
	MSG("energy: %g", e);
	MSG("f0: %g %g", fx[0], fy[0]);
	MSG("g0: %g %g", gx[0], gy[0]);

	char *key = "x y fx fy gx gy";
	const real *queue[] = {
		x, y, fx, fy, gx, gy, NULL	};
	puts(key);
	punto_fwrite(nv, queue, stdout);
	force2_fin(force);
}

int
main(int __UNUSED argc, char *argv[])
{
	err_set_ignore();
	argv++;
	if (util_eq(*argv, "-h"))
		usg();
	skel_read(stdin, &x, &y, &skel);

	argv_str(&argv, name);
	force2_argv(name, &argv, skel,  &force);
	nv = skel_nv(skel);

	CALLOC(nv, &fx); 
	CALLOC(nv, &fy);
	MALLOC(nv, &gx); 
	MALLOC(nv, &gy);

	main0();

	FREE(fx); 
	FREE(fy);
	FREE(gx); 
	FREE(gy);

	skel_xy_fin(x, y, skel);
}

/*

m clean; m
./main bend_ade 2 3 < data/rbc.skel  > q
gp <<'!'
plot "q" u 3:5, "" u 4:6, x
!

*/