#define T PreDensity
typedef struct T T;
typedef struct Kernel Kernal;

/* 3D and 2D */
int pre_density_ini(real R, real(*)(real, void *), void *, T **);
int pre_density2_ini(real R, real(*)(real, void *), void *, T **);

int pre_density_kernel_ini(real R, Kernel *, T **);
int pre_density2_kernel_ini(real R, Kernel *, T **);

int pre_density_fin(T *);
int pre_density_apply(T *, real r[3], real point[3], real n[3],
                      /**/ real *);

#undef T
