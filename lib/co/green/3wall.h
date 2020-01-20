#define T Green3Wall
typedef struct T T;
typedef struct He He;
int green3_wall_ini(He *, T **);
int green3_wall_fin(T *);
int green3_wall_apply(T *, He *, const real *, const real *, const real *,
                      real * xx, real * xy, real * xz, real * yy,
                      real * yz, real * zz);
int green3_wall_s(T *, const real[3], const real[3], real *, real *, real *, real *, real *, real *);
int green3_wall_t(T *, const real[3], const real normal[3], const real[3], real *, real *, real *, real *, real *, real *);
int green3_wall_stresslet(T *, He *, const real *, const real *,
                          const real *, real * xx, real * xy, real * xz,
                          real * yy, real * yz, real * zz);
int green3_wall_single_velocity(T * q, He * he,
				const real * x, const real * y,
                               const real * z, const real * fx,
                               const real * fy, const real * fz,
                               const real r[3], /**/ real v[3]);
int green3_wall_double_velocity(T * q, He * he,
				const real * x, const real * y,
				const real * z, const real * ux,
				const real * uy, const real * uz,
				const real r[3], /**/ real v[3]);
#undef T
