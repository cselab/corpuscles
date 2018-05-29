#include <stdio.h>

#include <he/err.h>
#include <he/punto.h>
#include <he/macro.h>

#define N (3)

static double XX[N];
static double YY[N];
static double ZZ[N];
static double *queue[] = {XX, YY, ZZ, NULL};

int main(__UNUSED int argc, const char *argv[]) {
    const char *i, *o;
    int n;
    if (argv[1] == NULL)
        ERR(HE_IO, "needs FILE.in");
    i = argv[1]; argv++;

    if (argv[1] == NULL)
        ERR(HE_IO, "needs FILE.out");
    o = argv[1]; argv++;
    
    punto_read(i, /**/ &n, queue);
    MSG("n = %d", n);
    MSG("writing: '%s'", o);
    punto_write(n, queue, o);
}
