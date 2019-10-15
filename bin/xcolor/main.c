#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>
#include <real.h>
#include <co/array.h>
#include <co/err.h>
#include <co/he.h>
#include <co/macro.h>
#include <co/force.h>
#include <co/util.h>
#include <co/off.h>
#include <co/y.h>
#include <co/memory.h>

static const char *me = "co.xcolor";

#define FMT_IN CO_REAL_IN

static int scl(char **v, /**/ real *p) {
    if (*v == NULL) ER("not enough args");
    if (sscanf(*v, FMT_IN, p) != 1)
        ER("not a number '%s'", *v);
    return CO_OK;
}

static void
usg(void)
{
	fprintf(stderr, "%s A.off lo hi B.off > C.off\n", me);
	fprintf(stderr, "%s -a A.off lo hi B.off > C.off\n", me);
	fprintf(stderr, "color vertices in B acording to (x-x_min)/(x_max-x_min) in A\n");
	fprintf(stderr, "if -a is given color vertices in B acording to abs(x)/x_max in A\n");
	exit(2);
}

int main(int argc, char **a)
{
	int status;
	real *x, *y, *z, *c;
	real *u, *v, *w;
	He *p, *q;
	int i, n, Abs;
	real min, max, d;
	real lo, hi;
       	err_set_ignore();
	a++;
	if (*a == NULL)
		ER("not enougth arguments");
	if (util_eq(*a, "-h"))
		usg();
	Abs = 0;
	if (util_eq(*a, "-a")) {
	    Abs = 1;
	    a++;
	}
	status = y_ini(*a, &q, &u, &v, &w);
	if (status != CO_OK)
		ER("not an off file '%s'", a[0]);
	scl(++a, &lo);
	scl(++a, &hi);
	a++;
	if (*a == NULL)
		ER("not enougth arguments");
	status = y_ini(*a, &p, &x, &y, &z);
	if (status != CO_OK)
		ER("not an off file '%s'", a[0]);
	n = he_nv(q);
	CALLOC(n, &c);
	min = array_min(n, u);
	max = array_max(n, u);
	d = max - min;
	if (Abs)
	    for (i = 0; i < n; i++)
		c[i] = fabs(u[i])/max;
	else
	    for (i = 0; i < n; i++)
		c[i] = (u[i] - min)/d;
	boff_lh_ver_fwrite(p, x, y, z, lo, hi, c, stdout);
	FREE(c);
	y_fin(p, x, y, z);
	y_fin(q, u, v, w);
}


