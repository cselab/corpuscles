#include <stdio.h>
#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/macro.h>
#include <co/segment.h>
#include <co/off.h>
#include <co/y.h>

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

