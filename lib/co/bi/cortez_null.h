#define T BiCortezNull

typedef struct He He;
typedef struct T T;

int bi_cortez_null_ini(He *, /**/ T **);
int bi_cortez_null_argv(char ***, He *, /**/ T **);
int bi_cortez_null_fin(T *);
int bi_cortez_null_update(T *, He *, const real * x, const real * y,
                          const real * z);
int bi_cortez_null_single(T *, He *, real alpha, const real * x,
                          const real * y, const real * z, const real * fx,
                          const real * fy, const real * fz,
                          /*io */ real *, real *, real *);
int bi_cortez_null_double(T *, He *, real alpha,
                          const real * x, const real * y, const real * z,
                          const real * vx, const real * vy,
                          const real * vz,
                          /*io */ real *, real *, real *);

int bi_cortez_null_single_velocity(T * q, He * he,
                                   const real * x, const real * y,
                                   const real * z, const real * fx,
                                   const real * fy, const real * fz,
                                   const real r[3], /**/ real v[3]);

int bi_cortez_null_double_velocity(T * q, He * he,
                                   const real * x, const real * y,
                                   const real * z, const real * fx,
                                   const real * fy, const real * fz,
                                   const real r[3], /**/ real v[3]);

#undef T
