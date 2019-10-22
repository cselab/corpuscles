#include <stdio.h>

void
cubtri2(void*, double*, double, int, double*, double*, int*, double*, int, int*);

#define NW (5000)

struct Function
{
    double (*function)(double, double, void*);
    void *params;
};

static
double F(double x, double y, void *p)
{
    return x*y + 1;
}

int
main()
{
    double w[NW];
    double T[2*3];
    double eps, ans, err;
    int mcalls, ncalls, i, ier;
    enum {X, Y};
    double a[] = {0, 0};
    double b[] = {1, 0};
    double c[] = {0, 1};
    double alpha;
    struct Function function;

    i = 0;
    T[i++] = a[X];
    T[i++] = a[Y];
    T[i++] = b[X];
    T[i++] = b[Y];
    T[i++] = c[X];
    T[i++] = c[Y];
    mcalls = 100;
    eps = 1e-4;
    alpha = 42.0;
    function.function = F;
    function.params = NULL;
    
    cubtri2(&function, T, eps, mcalls, &ans, &err, &ncalls, w, NW, &ier);
    printf("ans: %g\n", ans);
    printf("err: %g\n", err);
    printf("ncalls: %d\n", ncalls);
}
