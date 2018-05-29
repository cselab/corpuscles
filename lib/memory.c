#include <stdio.h>
#include <stdlib.h>
#include "he/err.h"
#include "he/memory.h"

int he_malloc(int size, /**/ void **pq) {
    void *q;
    q = malloc(size);
    if (q == NULL) ERR(HE_MEMORY, "malloc failed for size=%ld", size);
    *pq = q;
    return HE_OK;
}

int he_free(void *q) {
    free(q);
    q = NULL;
    return HE_OK;
}
