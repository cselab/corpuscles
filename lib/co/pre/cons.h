#define T PreCons
typedef struct T T;
typedef struct Kernel Kernal;

/* 3D and 2D */
int pre_cons_ini(real R, real(*)(real, void *), void *, T **);
int pre_cons2_ini(real R, real(*)(real, void *), void *, T **);

int pre_cons_kernel_ini(real R, Kernel *, T **);
int pre_cons2_kernel_ini(real R, Kernel *, T **);

int pre_cons_fin(T *);
int pre_cons_apply(T *, real r[3], real point[3], real n[3], /**/ real[3]);

#undef T
