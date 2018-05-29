typedef struct TriKernel TriKernel;

enum {KERNEL_QUINTIC, KERNEL_WENDLAND6, KERNEL_YANG};
enum {KERNEL_1D, KERNEL_2D, KERNEL_3D};

void tri_kernel_ini(int type, int dim, TriKernel**);
void tri_kernel_fin(TriKernel*);

double tri_kernel_w(TriKernel*, double h, double x);
double tri_kernel_dw(TriKernel*, double h, double x);
