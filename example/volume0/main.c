#include <stdio.h>

#include <real.h>
#include <co/err.h>
/* #include <co/x.h> */

int main() {
    ini("/dev/stdin");
    printf("%g\n", volume());
    fin();
    return CO_OK;
}
