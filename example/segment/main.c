#include <stdio.h>

#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/macro.h>
#include <co/segment.h>
#include <co/util.h>
#include <co/y.h>

#define FMT CO_REAL_OUT
static const char **argv;


int main()
{
	real *x, *y, *z;
	He *he;

	y_inif(stdin, &he, &x, &y, &z);	
	printf(FMT "\n", segment_average(he, x, y, z));
	y_fin(he, x, y, z);
}

