#define MALLOC(n, pp) co_malloc((n)*sizeof(**(pp)), (void**)(pp))
#define REALLOC(n, pp) co_realloc((n)*sizeoff(**(pp)), (void**)(pp))
#define CALLOC(n, pp) co_calloc((n)*sizeof(**(pp)), (void**)(pp))
#define FREE(p) co_free(p)
#define MEMCPY(n, src, dest) co_memcpy((void*)(dest), (const void*)(src), (n)*sizeof(*(dest)))
int co_malloc(int, void**);
int co_realloc(int, void**);
int co_calloc(int, void**);
int co_free(void *ptr);
int co_memcpy(void *dest, const void *src, size_t n);
