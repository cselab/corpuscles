#define T HeRead

typedef struct T T;

int he_read(const char *path, T**);
int he_fin(T*);

#undef T
