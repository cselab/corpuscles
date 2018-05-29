typedef struct TriKernel TriKernel;

enum {KERNEL_QUINTIC, KERNEL_WENDLAND6, KERNEL_YANG};
enum {KERNEL_1D, KERNEL_2D, KERNEL_3D};

void he_kernel_ini(int type, int dim, TriKernel**);
void he_kernel_fin(TriKernel*);

double he_kernel_w(TriKernel*, double h, double x);
double he_kernel_dw(TriKernel*, double h, double x);
