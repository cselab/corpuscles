#include <stdio.h>

#include <real.h>
#include <co/err.h>
#include <co/punto.h>
#include <co/macro.h>

#define N (3)

static real XX[N];
static real YY[N];
static real ZZ[N];
static const real *queue[] = {XX, YY, ZZ, NULL};

int main(__UNUSED int argc, const char *argv[]) {
    const char *i, *o;
    int n;
    if (argv[1] == NULL)
        ERR(CO_IO, "needs FILE.in");
    i = argv[1]; argv++;

    o = NULL;
    if (argv[1] != NULL) {
        o = argv[1]; argv++;
    }
    
    punto_read(i, /**/ &n, queue);
    MSG("n = %d", n);

    if (o == NULL) 
        punto_fwrite(n, queue, stdout);
    else
        punto_write(n, queue, o);
}
