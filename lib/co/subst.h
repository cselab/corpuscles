/*
  solves for `x'
  x = u + D(alpha) . x
  where `D(alpha) . x' is computed by bi_double
 */

#define T Subst
typedef struct T T;
typedef struct He He;
typedef struct BI BI;
enum { SUBST_TOL, SUBST_ITER, SUBST_FAIL };
int subst_ini(int n, real alpha, real tol, int iter_max, T **);
int subst_fin(T *);
int subst_apply(T *, He *, BI *, const real * x, const real * y,
                const real * z, const real * ux, const real * uy,
                const real * uz, real *, real *, real *);
int subst_apply_initial(T *, He *, BI *, const real *, const real *,
                        const real * ux, const real *, const real *,
                        const real * ix, const real *, const real *,
                        const real *, real *, real *, real *);
int subst_status(T *);
int subst_niter(T *);

#undef T
