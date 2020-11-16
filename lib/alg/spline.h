#define T AlgSpline
typedef struct T T;

enum { LINEAR, POLYNOMIAL, CSPLINE, AKIMA, STEFFEN };
int alg_spline_ini(int n, real *, real *, int type, T **);
int alg_spline_fscanf(FILE *, int type, T **);
int alg_spline_fin(T *);
real alg_spline_apply(T *, real);

real alg_spline_lo(T *);
real alg_spline_hi(T *);

#undef T
