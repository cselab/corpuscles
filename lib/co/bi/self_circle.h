#define T BiSelfCircle

typedef struct He He;
typedef struct T T;

int bi_self_circle_ini(He*, /**/ T**);
int bi_self_circle_argv(char***, He*, /**/ T**);
int bi_self_circle_fin(T*);
int bi_self_circle_update(T*, He*, const real *x, const real *y, const real *z);
int bi_self_circle_single(T*, He*, real alpha,
		     const real *x, const real *y, const real *z,
		     const real *fx, const real *fy, const real *fz,
		     /*io*/ real*, real*, real*);
int bi_self_circle_double(T*, He*, real alpha,
		     const real *x, const real *y, const real *z,
		     const real *vx, const real *vy, const real *vz,
		     /*io*/ real*, real*, real*);
#undef T
