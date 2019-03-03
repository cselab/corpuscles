#include <stdio.h>

#include <real.h>
#include <co/err.h>
#include <co/x.h>

int main() {
    ini("/dev/stdin");
    printf("%g\n", area());
    fin();
    return CO_OK;
}
