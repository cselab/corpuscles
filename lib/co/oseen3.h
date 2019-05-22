#define T Oseen3
typedef struct T T;
typedef struct He He;
int oseen3_ini(real, T**);
int oseen3_fin(T*);
int oseen3_apply(T*, He*, const real*, const real*, const real*, real *xx, real *xy, real *xz, real *yy, real *yz, real *zz);
#undef T
