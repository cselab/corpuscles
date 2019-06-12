#include <stdio.h>
#include <co/omp.h>

int
main()
{
	int n;

	n = 100;
	omp_test(n);
}