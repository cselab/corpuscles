#include <stdio.h>
#include <stdlib.h>
#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/macro.h>
#include <co/force.h>
#include <co/util.h>
#include <co/off.h>
#include <co/y.h>

static const char *me = "co.xcolor";

static void
usg(void)
{
	fprintf(stderr, "%s A.off B.off > C.off\n", me);
	fprintf(stderr, "color vertices in B acording to x coordinate in A\n");
	exit(2);
}

int
main(int c, char **a)
{
	int status;
	real *x, *y, *z;
	real *u, *v, *w;
	He *p, *q;
	err_set_ignore();
	a++;
	if (*a == NULL)
		ER("not enougth arguments");
	if (util_eq(*a, "-h"))
		usg();

	status = y_ini(*a, &q, &u, &v, &w);
	if (status != CO_OK)
		ER("not an off file '%s'", a[0]);
	a++;
	if (*a == NULL)
		ER("not enougth arguments");
	status = y_ini(*a, &p, &x, &y, &z);
	if (status != CO_OK)
		ER("not an off file '%s'", a[0]);
	boff_ver_fwrite(p, x, y, z, u, stdout);
	y_fin(p, x, y, z);
	y_fin(q, u, v, w);
}


