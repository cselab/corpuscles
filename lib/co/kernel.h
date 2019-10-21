#define T Kernel
typedef struct T T;

enum { KERNEL_GAUSS, KERNEL_QUINTIC, KERNEL_WENDLAND6, KERNEL_YANG };
enum { KERNEL_1D, KERNEL_2D, KERNEL_3D };

int kernel_ini(int dim, int type, T **);
int kernel_argv(char ***, T **);
int kernel_fin(T *);

real kernel_w(T *, real h, real x);
real kernel_dw(T *, real, real);
real kernel_dwr(T *, real, real);

#undef T
