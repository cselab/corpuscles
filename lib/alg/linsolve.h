#define T LinSolve
typedef struct LinSolve LinSolve;

int lin_solve_ini(int n, T **);

/* Ax = b */
int lin_solve_apply(T *, const real * A, const real * b, real * x);
int lin_solve_sym(T *, const real * A, const real * b, real * x);
int lin_solve_fin(T *);

#undef T
