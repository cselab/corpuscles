#define T HeRead

typedef struct T T;

int he_read_ini(const char *path, T**);
int he_read_fin(T*);

int he_read_nv(T*);
int he_read_nt(T*);
int he_read_ne(T*);
int he_read_nh(T*);

int he_read_next(T*, int**);
int he_read_flip(T*, int**);

int he_read_ver(T*, int**);
int he_read_tri(T*, int**);
int he_read_edg(T*, int**);

int he_read_validate(T*);
int he_info(T*, FILE*);

#undef T
