#define T Oseen3Zero
typedef struct T T;
typedef struct He He;
int oseen3_zero_ini(He *, T **);
int oseen3_zero_fin(T *);
int oseen3_zero_apply(T *, He *, const real *, const real *, const real *,
                      real * xx, real * xy, real * xz, real * yy,
                      real * yz, real * zz);
int oseen3_zero_stresslet(T *, He *, const real *, const real *,
                          const real *, real * xx, real * xy, real * xz,
                          real * yy, real * yz, real * zz);

int oseen3zero_single_velocity(T * q, He * he,
                               const real * x, const real * y,
                               const real * z, const real * fx,
                               const real * fy, const real * fz,
                               const real r[3], /**/ real v[3]);
#undef T
