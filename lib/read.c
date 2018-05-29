#include <stdio.h>
#include <string.h>

#include "he/err.h"
#include "he/memory.h"
#include "he/read.h"

#define T HeRead

struct T {
    int nv, nt, ne, nh;
    int *next, *flip;
    int *ver, *tri, *edg;

    int *hdg_ver, *hdg_edg, *hdg_tri;
};

int he_read_ini(const char *path, T **pq) {
    T *q;
    MALLOC(1, &q);
    *pq = q;

    return HE_OK;
}
