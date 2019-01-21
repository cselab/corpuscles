#include <stdio.h>

#include <real.h>
#include <he/err.h>
#include <he/macro.h>
#include <he/util.h>

int main(__UNUSED int argc, const char **argv) {
    real x;
    util_real(&argv, x);
    
    return HE_OK;
}
