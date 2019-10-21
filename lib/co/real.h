#define T Real
typedef struct T T;
int real_ini(int n, T **);
int real_fin(T *);
double *real_to(T *, real *);
real *real_from(T *, double *);

#undef T
