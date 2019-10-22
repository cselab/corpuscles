#include <stdio.h>

struct Function
{
    double (*function)(double, double, void*);
    void *params;
};

double
fun(void *p, double x, double y)
{
    struct Function *function;
    function = p;
    return function->function(x, y, function->params);
}
