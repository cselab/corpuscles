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
    FILE *f;
    MALLOC(1, &q);
    *pq = q;

    f = fopen(path, "r");
    if (f == NULL)
        ERR(HE_IO, "fail to open '%s'", path);

    fscanf(f, "%s", &line);

    return HE_OK;
}
