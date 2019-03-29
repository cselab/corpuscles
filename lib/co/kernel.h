#define T Kernel
typedef struct T T;

enum {KERNEL_QUINTIC, KERNEL_WENDLAND6, KERNEL_YANG};
enum {KERNEL_1D, KERNEL_2D, KERNEL_3D};

int kernel_ini(int type, int dim, T**);
int kernel_fin(T*);

real kernel_w(T*, real h, real x);
real kernel_dw(T*, real h, real x);

#undef T
