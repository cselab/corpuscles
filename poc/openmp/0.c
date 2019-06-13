#include <stdio.h>
#include <co/omp.h>

int
main()
{
	int n;
	n = 4;
	omp_test(n);
}