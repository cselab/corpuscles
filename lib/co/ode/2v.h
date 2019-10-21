#define T Ode2v
typedef struct Ode2v Ode2v;
int ode2v_ini(int type, int n, real dt,
              int (*)(real, const real *, const real *, const real *,
                      const real *, real *, real *, void *), void *, T **);
int ode2v_fin(T *);
int ode2v_apply(T *, real * time, real t, real *, real *, real *, real *);

#undef T
