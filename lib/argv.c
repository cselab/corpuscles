#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "real.h"
#include "he/err.h"

#include "inc/def.h"
#include "he/argv.h"

#define  SIZE (MAX_STRING_SIZE)

int argv_real(char ***pargv, real *p) {
    char **argv;
    argv = *pargv;

    if (*argv == NULL)
        ERR(HE_IO, "not enough arguments");

    if (sscanf(*argv, HE_REAL_IN, p) != 1)
        ERR(HE_IO, "not a number '%s'",*argv);

    argv++;
    *pargv = argv;
    return HE_OK;
}

int argv_str(char ***pargv, char *p) {
    char **argv;
    argv = *pargv;

    if (*argv == NULL)
        ERR(HE_IO, "not enough arguments");
    strncpy(p, *argv, SIZE - 1);

    argv++;
    *pargv = argv;
    return HE_OK;
}
