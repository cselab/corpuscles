#define T BiCortezFm

typedef struct He He;
typedef struct T T;

int bi_cortez_fm_ini(He*, /**/ T**);
int bi_cortez_fm_argv(char***, He*, /**/ T**);
int bi_cortez_fm_fin(T*);
int bi_cortez_fm_update(T*, He*, const real *x, const real *y, const real *z);
int bi_cortez_fm_single(T*, He*, real alpha,
		     const real *x, const real *y, const real *z,
		     const real *fx, const real *fy, const real *fz,
		     /*io*/ real*, real*, real*);
int bi_cortez_fm_double(T*, He*, real alpha,
		     const real *x, const real *y, const real *z,
		     const real *vx, const real *vy, const real *vz,
		     /*io*/ real*, real*, real*);
#undef T
