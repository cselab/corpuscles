#define MALLOC(n, pp) tri_malloc((n)*sizeof(**(pp)), (void**)(pp))
#define FREE(p) tri_free(p)
void tri_malloc(int, void**);
void tri_free(void *ptr);
