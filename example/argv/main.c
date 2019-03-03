#include <stdio.h>

#include <real.h>
#include <co/err.h>
#include <co/macro.h>
#include <co/argv.h>

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
    printf("x = " CO_REAL_OUT "\n", x);

    return CO_OK;
}
