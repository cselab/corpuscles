#include <stdio.h>
#include <stdlib.h>
#include "he/err.h"
#include "he/memory.h"

void tri_malloc(int size, /**/ void **pq) {
    void *q;
    q = malloc(size);
    if (q == NULL) ERR("malloc failed for size=%ld", size);
    *pq = q;
}

void tri_free(void *q) {
    free(q);
    q = NULL;
}
