#define T Surface

typedef struct T T;
typedef struct He He;

int surface_ini(T**);
int surface_fin(T*);
int surface_update(T*, He*, const real *x, const real *y, const real *z);
int surface_inside(T*, real, real, real);

#undef T