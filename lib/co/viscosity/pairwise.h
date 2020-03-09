#define T ViscosityPairwise

typedef struct He He;
typedef struct T T;

int viscosity_pairwise_ini(real K, He *, T **);
int viscosity_pairwise_force(T *, He *, const real *, const real *,
                             const real *, const real *, const real *,
                             const real *, /**/ real *, real *, real *);
int viscosity_pairwise_fin(T *);
