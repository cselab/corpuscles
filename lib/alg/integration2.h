#define T AlgIntegration2
typedef struct T T;
int alg_integration2_ini(int, T **);
int alg_integration2_fin(T *);
int alg_integration2_apply(T *, real u0, real u1,
                           real(*v0) (real u, void *), real(*v1) (real u,
                                                                  void *),
                           real(*f) (real u, real v, void *), void *,
                           /**/ real *);
#undef T
