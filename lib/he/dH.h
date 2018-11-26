#define T Dh

typedef struct He He;
typedef struct Ten Ten;
typedef struct T T;

int dh_ini(He*, /**/ T**);
int dh_fin(T*);

int dh_apply(T*, He*, const real *x, const real *y, const real *z, /**/ real *fx, real *fy, real *fz);

#undef T
