#define T Ode2
typedef struct Ode2 Ode2;
int ode2_ini(int type, int n, real dt,
             int (*)(real, const real *, const real *, real *, real *,
                     void *), void *, T **);
int ode2_fin(T *);
int ode2_apply(T *, real * time, real t, real *, real *);

#undef T
