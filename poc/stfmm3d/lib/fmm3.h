#define T FMM3
typedef struct T T;
int fmm3_ini(int n, T**);
int fmm3_single(T*,
		const real *x, const real *y, const real *z,
		const real *fx, const real *fy, const real *fz,
		/*io*/ real *vx, real *vy, real *vz);
int fmm3_double(T*,
		const real *x, const real *y, const real *z,
		const real *ux, const real *uy, const real *uz,
		const real *nx, const real *ny, const real *nz,
		/*io*/ real *vx, real *vy, real *vz);
int fmm3_fin(T*);
#undef T
