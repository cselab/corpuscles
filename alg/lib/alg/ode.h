#define T Ode
typedef struct T T;
enum {
    RK2,
    RK4,
    RK8PD,
    RKCK,
    RKF45,
};
int ode_ini(int type, int dim, real dt,
            int (*)(real, const real *, real * dy, void *), void *, T **);
int ode_fin(T *);
int ode_apply(T *, real * time, real t, real *);
int ode_apply_fixed(T *, real * time, real t, real *);

#undef T
