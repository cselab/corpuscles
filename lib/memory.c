#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "co/err.h"
#include "co/memory.h"

int
co_malloc(int size, /**/ void **pq)
{
  void *q;

  q = malloc(size);
  if (q == NULL)
    ERR(CO_MEMORY, "malloc failed for size=%ld", size);
  *pq = q;
  return CO_OK;
}

int
co_realloc(int size, /**/ void **pa)
{
  void *a, *b;

  a = *pa;
  b = realloc(a, size);
  if (b == NULL)
    ERR(CO_MEMORY, "remalloc failed for size=%ld", size);
  *pa = b;
  return CO_OK;
}


int
co_calloc(int size, /**/ void **pq)
{
  void *q;

  q = calloc(1, size);
  if (q == NULL)
    ERR(CO_MEMORY, "calloc failed for size=%ld", size);
  *pq = q;
  return CO_OK;
}

int
co_free(void *q)
{
  free(q);
  q = NULL;
  return CO_OK;
}

int
co_memcpy(void *dest, const void *src, size_t n)
{
  if (dest == NULL)
    ERR(CO_MEMORY, "dest == NULL");
  if (src == NULL)
    ERR(CO_MEMORY, "src == NULL");
  memcpy(dest, src, n);
  return CO_OK;
}
