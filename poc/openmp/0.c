#include <stdio.h>
#include <omp.h>

int
main()
{
	int i, t, n;
	n = 100;
#pragma omp parallel for
	for (i = 0; i < n; i++) {
		t = omp_get_thread_num();
		printf("tid %d\n", t);
	}
}