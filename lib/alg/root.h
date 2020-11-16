#define T AlgRoot
typedef struct T T;

enum { BISECTION, FALSEPOS, BRENT };
int alg_root_ini(int, T **);
int alg_root_fin(T *);
int alg_root_apply(T *, real, real, real(*)(real, void *), void *,
                   /**/ real *);
const char *alg_root_name(T *);
int alg_root_niter(T *);

#undef T
