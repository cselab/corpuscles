#define MALLOC(n, pp) he_malloc((n)*sizeof(**(pp)), (void**)(pp))
#define FREE(p) he_free(p)
int he_malloc(int, void**);
int he_free(void *ptr);
