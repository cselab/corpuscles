#define T Bi

typedef struct He He;
typedef struct T T;

int bi_argv(const char *name, char***, He*, /**/ T**);
int bi_single(T*, He*, real alpha,
	      const real *x, const real *y, const real *z,
	      const real *fx, const real *fy, const real *fz,
	      /*io*/, real*, real*, real*);

int bi_double(T*, He*, real alpha,
	      const real *x, const real *y, const real *z,
	      const real *vx, const real *vy, const real *vz,
	      /*io*/, real*, real*, real*);

const char* bi_name(T*);
int bi_name(const char*);
int bi_good(const char*);
int bi_fin(T*);

#undef T
