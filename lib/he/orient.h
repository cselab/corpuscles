#define T Orient

typedef struct He He;
typedef struct T T;

int orient_ini(He*, /**/ T**);
int orient_fin(T*);

int orient_apply(T*, /*io*/ real *x, real *y, real *z);

#undef T
