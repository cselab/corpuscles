typedef double (*Fun)(double, double);
static Fun   w[][3] = {  {NULL,         quintic_w2,   quintic_w3},
                         {wendland6_w1,  wendland6_w2, wendland6_w3},
                         {yang_w1,  yang_w2, yang_w3} };

static Fun  dw[][3] = {  {        NULL,  quintic_dw2,    quintic_dw3},
                         {wendland6_dw1, wendland6_dw2, wendland6_dw3},
                         {yang_dw1, yang_dw2, yang_dw3} };

struct TriKernel {
    int dim;
    Fun w, dw;
};

void tri_kernel_ini(int type, int dim, TriKernel **pq) {
    TriKernel *q;
    MALLOC(1, &q);
    switch (type) {
    case (KERNEL_QUINTIC): break;
    case (KERNEL_WENDLAND6): break;
    case (KERNEL_YANG):      break;        
    default:
        ERR("unknown type, possible values are KERNEL_QUINTIC, KERNEL_WENDLAND6, KERNEL_YANG");
    }

    switch (dim) {
    case(KERNEL_1D): break;        
    case(KERNEL_2D): break;
    case(KERNEL_3D): break;
    default:
        ERR("unknown dimenshon, possible values are KERNEL_1D, KERNEL_2D, KERNEL_3D");
    }
    q->dim = dim;
    q->w = w[type][dim]; q->dw = dw[type][dim];
    *pq = q;
}

void tri_kernel_fin(TriKernel *q) { FREE(q); }

double tri_kernel_w(TriKernel *q, double c, double x) { return q->w(c, x); }
double tri_kernel_dw(TriKernel *q, double c, double x) { return q->dw(c, x); }
