#define T Oseen2
typedef struct T T;
typedef struct Skel Skel;
int oseen2_ini(real, T **);
int oseen2_fin(T *);
int oseen2_apply(T *, Skel *, const real *, const real *, real * xx,
                 real * xy, real * yy);
real oseen2_pressure(T *, Skel *, const real *, const real *, const real *,
                     const real *, const real[2]);
int oseen2_velocity(T *, Skel *, real mu, const real *, const real *,
                    const real * fx, const real * fy, const real r[2],
                    real[2]);
int oseen2_stresslet(T * q, Skel *, const real *, const real *, real * xx,
                     real * xy, real * yy);
#undef T
