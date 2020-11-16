#define T AlgRng
typedef struct T T;
int alg_rng_ini(T **);
int alg_rng_fin(T *);
real alg_rng_gaussian(T *, real);
real alg_rng_uniform(T *, real, real);

#undef T
