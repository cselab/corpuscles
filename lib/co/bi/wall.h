#define T BiWall

typedef struct He He;
typedef struct T T;

int bi_wall_ini(real w, He *, /**/ T **);
int bi_wall_argv(char ***, He *, /**/ T **);
int bi_wall_fin(T *);
int bi_wall_update(T *, He *, const real * x, const real * y,
                   const real * z);
int bi_wall_single(T *, He *, real alpha, const real * x,
                   const real * y, const real * z, const real * fx,
                   const real * fy, const real * fz,
                   /*io */ real *, real *, real *);
int bi_wall_double(T *, He *, real alpha,
                   const real * x, const real * y, const real * z,
                   const real * vx, const real * vy, const real * vz,
                   /*io */ real *, real *, real *);

int bi_wall_single_velocity(T * q, He * he,
                            const real * x, const real * y,
                            const real * z, const real * fx,
                            const real * fy, const real * fz,
                            const real r[3], /**/ real v[3]);

int bi_wall_double_velocity(T * q, He * he,
                            const real * x, const real * y,
                            const real * z, const real * fx,
                            const real * fy, const real * fz,
                            const real r[3], /**/ real v[3]);

#undef T
