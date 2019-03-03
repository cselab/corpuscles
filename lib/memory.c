#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "co/err.h"
#include "co/memory.h"

int he_malloc(int size, /**/ void **pq) {
    void *q;
    q = malloc(size);
    if (q == NULL) ERR(CO_MEMORY, "malloc failed for size=%ld", size);
    *pq = q;
    return CO_OK;
}

int he_calloc(int size, /**/ void **pq) {
    void *q;
    q = calloc(1, size);
    if (q == NULL) ERR(CO_MEMORY, "calloc failed for size=%ld", size);
    *pq = q;
    return CO_OK;
}

int he_free(void *q) {
    free(q);
    q = NULL;
    return CO_OK;
}

int he_memcpy(void *dest, const void *src, size_t n) {
    if (dest == NULL) ERR(CO_MEMORY, "dest == NULL");
    if (src == NULL) ERR(CO_MEMORY, "src == NULL");
    memcpy(dest, src, n);
    return CO_OK;
}
