#include <stdio.h>
#include <stdlib.h>
#include "he/err.h"
#include "he/memory.h"

void he_malloc(int size, /**/ void **pq) {
    void *q;
    q = malloc(size);
    if (q == NULL) ERR("malloc failed for size=%ld", size);
    *pq = q;
}

void he_free(void *q) {
    free(q);
    q = NULL;
}
