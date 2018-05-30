#include <stdio.h>
#include <string.h>

#include "real.h"
#include "he/memory.h"
#include "he/err.h"

#include "he/off.h"

#define T HeOff

struct T {
    real *xyz;
    int  *tri;
};

int he_off_ini(const char *path, T **pq) {
    T *q;
    *pq = q;
    return HE_OK;
}
