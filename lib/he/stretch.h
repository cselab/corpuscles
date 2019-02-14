#define T Stretch

typedef struct T T;
typedef struct He He;

int stretch_argv(char***, He *he, real *x, real *y, real *z, /**/ T**);
int stretch_force(T*, const real*, const real*, const real*, /*io*/ real*, real*, real*);

int stretch_fin(T*);

#undef T
