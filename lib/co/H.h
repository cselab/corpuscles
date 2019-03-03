#define T H

typedef struct He He;
typedef struct T T;

int H_ini(He*, /**/ T**);
int H_fin(T*);
int H_apply(T*, He*, const real *x, const real *y, const real *z, /**/ real **H, real **area);
#undef T