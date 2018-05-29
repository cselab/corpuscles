#define MALLOC(n, pp) he_malloc((n)*sizeof(**(pp)), (void**)(pp))
#define FREE(p) he_free(p)
#define MEMCPY(n, dest, src) he_memcpy((void**)(dest), (void**)(src), (n)*sizeof(**(src)))

int he_malloc(int, void**);
int he_free(void *ptr);
int he_memcpy(void *dest, const void *src, size_t n);
