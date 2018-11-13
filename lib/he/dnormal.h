#define T Dnormal

typedef struct He He;
typedef struct Ten Ten;
typedef struct T T;

int dnormal_ini(He*, /**/ T**);
int dnormal_fin(T*);

int dnormal_apply(T*, /**/ Ten**);

#undef T
