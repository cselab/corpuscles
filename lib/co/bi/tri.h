#define T BiTri

typedef struct He He;
typedef struct T T;

int bi_tri_ini(He*, /**/ T**);
int bi_tri_argv(char***, He*, /**/ T**);
int bi_tri_fin(T*);
int bi_tri_update(T*, He*, const real *x, const real *y, const real *z);
int bi_tri_single(T*, He*, real alpha,
		     const real *x, const real *y, const real *z,
		     const real *fx, const real *fy, const real *fz,
		     /*io*/ real*, real*, real*);
int bi_tri_double(T*, He*, real alpha,
		     const real *x, const real *y, const real *z,
		     const real *vx, const real *vy, const real *vz,
		     /*io*/ real*, real*, real*);
#undef T
