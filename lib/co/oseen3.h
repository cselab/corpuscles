#define T Oseen3
typedef struct T T;
typedef struct He He;
int oseen3_ini(He*, real, T**);
int oseen3_fin(T*);
int oseen3_apply(T*, He*, const real*, const real*, const real*, real *xx, real *xy, real *xz, real *yy, real *yz, real *zz);
int oseen3_velocity(T*, He*, real mu, const real*, const real*, const real*,
	const real *fx, const real *fy, const real *fz, const real r[3], real[3]);
int oseen3_stresslet(T*, He*, const real*, const real*, const real*, real *xx, real *xy, real *xz, real *yy, real *yz, real *zz);
#undef T
