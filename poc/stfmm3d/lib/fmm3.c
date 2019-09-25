#include <stdlib.h>
#include <real.h>
#include <co/err.h>
#include <co/memory.h>

#include "fmm3.h"
#include "stfmm3.h"

#define T FMM3
struct T
{
    
};

int
fmm3_ini(int n, T **pq)
{
    T *q;
    MALLOC(n, &q);
    *pq = q;
}
