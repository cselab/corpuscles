#define T Ode3
typedef struct Ode3 Ode3;
int ode3_ini(int type, int n, real dt,
             int (*)(real, const real *, const real *, const real *,
                     real *, real *, real *, void *), void *, T **);
int ode3_fin(T *);
int ode3_apply(T *, real * time, real t, real *, real *, real *);
int ode3_apply_fixed(T *, real * time, real t, real *, real *, real *);

#undef T
