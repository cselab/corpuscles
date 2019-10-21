#define T Dnormal
typedef struct He He;
typedef struct Ten Ten;
typedef struct T T;
int dnormal_ini(He *, /**/ T **);
int dnormal_fin(T *);
int dnormal_apply(T *, He *, const real * x, const real * y,
                  const real * z, /**/ Ten **);
#undef T
