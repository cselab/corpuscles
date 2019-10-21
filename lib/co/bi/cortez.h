#define T BiCortez

typedef struct He He;
typedef struct T T;

int bi_cortez_ini(real eps, He *, /**/ T **);
int bi_cortez_argv(char ***, He *, /**/ T **);
int bi_cortez_fin(T *);
int bi_cortez_update(T *, He *, const real * x, const real * y,
                     const real * z);
int bi_cortez_single(T *, He *, real alpha, const real * x, const real * y,
                     const real * z, const real * fx, const real * fy,
                     const real * fz,
                     /*io */ real *, real *, real *);
int bi_cortez_double(T *, He *, real alpha,
                     const real * x, const real * y, const real * z,
                     const real * vx, const real * vy, const real * vz,
                     /*io */ real *, real *, real *);

#undef T
