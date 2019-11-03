#include <stdio.h>
#ifdef _OPENMP
#include <omp.h>
#endif
#include "co/err.h"
#include "co/omp.h"

int
omp_test(int n)
{
    int i, t;

#pragma omp parallel for
    for (i = 0; i < n; i++) {
        t = omp_get_thread_num();
        printf("tid %d\n", t);
    }
    return CO_OK;
}
