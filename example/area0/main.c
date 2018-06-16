#include <stdio.h>

#include <real.h>
#include <he/err.h>
#include <he/x.h>

int main() {
    ini("/dev/stdin");
    printf("%g\n", area());    
    fin();
    return HE_OK;
}
