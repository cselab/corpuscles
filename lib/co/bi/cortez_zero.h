#define T BiCortezZero

typedef struct He He;
typedef struct T T;

int bi_cortez_zero_ini(He *, /**/ T **);
int bi_cortez_zero_argv(char ***, He *, /**/ T **);
int bi_cortez_zero_fin(T *);
int bi_cortez_zero_update(T *, He *, const real * x, const real * y,
                          const real * z);
int bi_cortez_zero_single(T *, He *, real alpha, const real * x,
                          const real * y, const real * z, const real * fx,
                          const real * fy, const real * fz,
                          /*io */ real *, real *, real *);
int bi_cortez_zero_double(T *, He *, real alpha,
                          const real * x, const real * y, const real * z,
                          const real * vx, const real * vy,
                          const real * vz,
                          /*io */ real *, real *, real *);

int bi_cortez_zero_single_velocity(T * q, He * he,
				   const real * x, const real * y, const real * z,
				   const real * fx, const real * fy, const real * fz,
				   const real r[3], /**/ real v[3]);

#undef T
