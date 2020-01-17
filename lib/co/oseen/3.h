#define T Oseen3
typedef struct T T;
typedef struct He He;
int oseen3_ini(He *, real, T **);
int oseen3_fin(T *);
int oseen3_apply(T *, He *, const real *, const real *, const real *,
                 real * xx, real * xy, real * xz, real * yy, real * yz,
                 real * zz);
int oseen3_velocity(T *, He *, real mu, const real *, const real *,
                    const real *, const real * fx, const real * fy,
                    const real * fz, const real r[3], real[3]);
int oseen3_stresslet(T *, He *, const real *, const real *, const real *,
                     real * xx, real * xy, real * xz, real * yy, real * yz,
                     real * zz);

/*  [u, v, w] += s * Tensor . [x, y, z] */
int oseen3_vector_tensor(int n, real s, const real * x, const real * y,
                         const real * z, real * Txx, real * Txy,
                         real * Txz, real * Tyy, real * Tyz, real * Tzz,
                         /**/ real * u, real * v, real * w);
#undef T
