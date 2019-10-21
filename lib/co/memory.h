#define MALLOC(n, pp) co_malloc((n)*sizeof(**(pp)), (void**)(pp))
#define MALLOC2(n, x, y) MALLOC(n, x), MALLOC(n, y)
#define MALLOC3(n, x, y, z) MALLOC(n, x), MALLOC(n, y), MALLOC(n, z)
#define REALLOC(n, pp) co_realloc((n)*sizeof(**(pp)), (void**)(pp))
#define CALLOC(n, pp) co_calloc((n)*sizeof(**(pp)), (void**)(pp))
#define CALLOC2(n, x, y) CALLOC(n, x), CALLOC(n, y)
#define CALLOC3(n, x, y, z) CALLOC(n, x), CALLOC(n, y), CALLOC(n, z)
#define FREE(p) co_free(p)
#define FREE2(x, y) FREE(x), FREE(y)
#define FREE3(x, y, z) FREE(x), FREE(y), FREE(z)
#define MEMCPY(n, src, dest) co_memcpy((void*)(dest), (const void*)(src), (n)*sizeof(*(dest)))
int co_malloc(int, void **);
int co_realloc(int, void **);
int co_calloc(int, void **);
int co_free(void *ptr);
int co_memcpy(void *dest, const void *src, size_t n);
