#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#include "he/macro.h"
#include "he/err.h"

#define SIZE (2048)

void he_err(__UNUSED int code, const char *file, int line, const char *fmt, ...) {
    char msg[SIZE];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(msg, SIZE, fmt, ap);
    va_end(ap);
    fprintf(stderr, "%s:%d: %s\n", file, line, msg);
    abort();
}

void he_msg(const char *file, int line, const char *fmt, ...) {
    char msg[SIZE];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(msg, SIZE, fmt, ap);
    va_end(ap);
    fprintf(stderr, "%s:%d: %s\n", file, line, msg);
}


