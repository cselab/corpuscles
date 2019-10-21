#define T Ode3v
typedef struct Ode3v Ode3v;
int ode3v_ini(int type, int n, real dt,
              int (*)(real, const real *, const real *, const real *,
                      const real *, const real *, const real *, real *,
                      real *, real *, void *), void *, T **);
int ode3v_fin(T *);
int ode3v_apply(T *, real * time, real t, real * x, real * y, real * z,
                real * vx, real * vy, real * vz);
#undef T
