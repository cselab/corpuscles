#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "real.h"
#include "he/err.h"
#include "he/util.h"

#include "inc/def.h"

#define  SIZE (MAX_STRING_SIZE)

int util_mkdir(const char *path) {
    char command[SIZE];
    snprintf(command, SIZE, "mkdir -p -- '%s'", path);
    if (system(command) != 0)
        ERR(HE_SYS, "comamnd <%s> failed", command);
    return HE_OK;
}

int util_eq(const char *a, const char *b) {
    return strncmp(a, b, SIZE) == 0;
}

char *util_fgets(char *s, FILE *stream) {
    char *c;
    if (fgets(s, SIZE, stream) == NULL)
        return NULL;
    if ((c = strchr(s, '\n')) != NULL)
        *c = '\0';
    return s;
}

enum {NO, YES};
static int commentp(const char *s) {
    while (*s != '\0') {
        if (*s == '#') return YES;
        if (!isblank(*s)) return NO;
        s++;
    }
    return YES;
}
char *util_comment_fgets(char *s, FILE *stream) {
    do if (util_fgets(s, stream) == NULL)
           return NULL;
    while (commentp(s) == YES);
    return s;
}

int util_real(char ***pargv, real *p) {
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

int util_str(char ***pargv, char *p) {
    char **argv;
    argv = *pargv;

    if (*argv == NULL)
        ERR(HE_IO, "not enough arguments");
    strncpy(p, *argv, SIZE - 1);

    argv++;
    *pargv = argv;
    return HE_OK;
}
