#include <stdio.h>

#include <real.h>
#include <he/err.h>
#include <he/macro.h>
#include <he/argv.h>

#define  SIZE (1024)

static char s[SIZE];

int main(__UNUSED int argc, char **argv) {
    int i;
    real x;

    argv++;
    argv_str(&argv, s);
    argv_int(&argv, &i);
    argv_real(&argv, &x);

    printf("s = %s\n", s);
    printf("i = %d\n", i);
    printf("x = " HE_REAL_OUT "\n", x);

    return HE_OK;
}