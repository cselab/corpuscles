#define T PreVisc
typedef struct T T;
typedef struct Kernel Kernal;

/* 3D and 2D */
int pre_visc_ini(real R, real beta, real(*)(real, void *), void *, T **);
int pre_visc2_ini(real R, real beta, real(*)(real, void *), void *, T **);

int pre_visc_kernel_ini(real R, real beta, Kernel *, T **);
int pre_visc2_kernel_ini(real R, real beta, Kernel *, T **);

int pre_visc_fin(T *);
int pre_visc_apply(T *, real r[3], real point[3], real n[3], /**/ real *);

#undef T
