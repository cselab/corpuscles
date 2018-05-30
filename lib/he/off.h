#define T HeOff

typedef struct T T;

int he_off_ini(const char *path, T**);
int he_off_fin(T*);

int he_off_nt(T*);
int he_off_nv(T*);
int he_off_ver(T*, real**);
int he_off_tri(T*, int**);

#undef T
