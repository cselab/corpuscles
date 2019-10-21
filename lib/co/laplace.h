#define T Laplace
typedef struct He He;
typedef struct T T;
int laplace_ini(He *, /**/ T **);
int laplace_fin(T *);
int laplace_apply(T *, He *, const real * x, const real * y,
                  const real * z, /**/ real ** lx, real ** ly, real ** lz,
                  real ** parea);
#undef T
