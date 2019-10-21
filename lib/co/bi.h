#define T BI

typedef struct He He;
typedef struct T T;

int bi_argv(const char *name, char ***, He *, T **);
int bi_fin(T *);
int bi_single(T *, He *, real alpha, const real * x, const real * y,
              const real * z, const real * fx, const real * fy,
              const real * fz, real *, real *, real *);
int bi_double(T *, He *, real alpha, const real * x, const real * y,
              const real * z, const real * vx, const real * vy,
              const real * vz, real *, real *, real *);
int bi_update(T *, He *, const real * x, const real * y, const real * z);
const char *bi_name(T *);
int bi_good(const char *);
const char *bi_list();

#undef T
