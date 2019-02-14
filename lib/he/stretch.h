#define T Stretch

typedef struct T T;

int stretch_argv(char***, real *x, real *y, real *z, /**/ T**);
int stretch_force(T*, const real*, const real*, const real*, /*io*/ real*, real*, real*);

int stretch_fin(T*);

#undef T

