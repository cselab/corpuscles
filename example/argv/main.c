#include <stdio.h>

#include <real.h>
#include <he/err.h>
#include <he/macro.h>
#include <he/argv.h>

#define  SIZE (1024)

static char s[SIZE];

int main(__UNUSED int argc, char **argv) {
    real x, y;

    argv++;
    argv_str(&argv, s);
    argv_real(&argv, &x);
    argv_real(&argv, &y);

    printf("s = %s\n", s);
    printf("x = " HE_REAL_OUT "\n", x);
    printf("y = " HE_REAL_OUT "\n", y);

    return HE_OK;
}
