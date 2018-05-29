#define MALLOC(n, pp) he_malloc((n)*sizeof(**(pp)), (void**)(pp))
#define FREE(p) he_free(p)
void he_malloc(int, void**);
void he_free(void *ptr);
