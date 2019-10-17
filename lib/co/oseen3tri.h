#define T Oseen3Tri
typedef struct T T;
typedef struct He He;
int oseen3_tri_ini(He*, T**);
int oseen3_tri_fin(T*);
int oseen3_tri_apply(T*, He*, const real*, const real*, const real*, real *xx, real *xy, real *xz, real *yy, real *yz, real *zz);
int oseen3_tri_stresslet(T*, He*, const real*, const real*, const real*, real *xx, real *xy, real *xz, real *yy, real *yz, real *zz);
#undef T
