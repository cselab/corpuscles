#include <stdio.h>

#include <real.h>
#include <co/array.h>
#include <co/err.h>
#include <co/he.h>
#include <co/list/tri3.h>
#include <co/y.h>

#define FMT CO_REAL_OUT

int main(void)
{
	He *he;
	Tri3List *list;
	real *x, *y, *z;
	real lo[] = {-5, -5, -5};
	real hi[] = {5, 5, 5};
	real size = 2;

	y_inif(stdin, &he, &x, &y, &z);
	tri3list_ini(lo, hi, size, &list);

	tri3list_get(list, 1, 0, 0);
	MSG("status: %d", tri3list_status(list));
	
	tri3list_fin(list);
	y_fin(he, x, y, z);
}

