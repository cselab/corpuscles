#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#include "inc/def.h"
#include "co/macro.h"
#include "co/err.h"

#define SIZE (MAX_STRING_SIZE)

enum { ERR_ABORT, ERR_IGNORE };
static int Type = ERR_ABORT;

void
he_err(__UNUSED int code, const char *file, int line, const char *fmt, ...)
{
    char msg[SIZE];
    va_list ap;

    va_start(ap, fmt);
    vsnprintf(msg, SIZE, fmt, ap);
    va_end(ap);
    fprintf(stderr, "%s:%d: %s\n", file, line, msg);
    if (Type == ERR_ABORT)
        abort();
    else if (code == CO_USER)
        exit(2);
}

void
he_msg(const char *file, int line, const char *fmt, ...)
{
    char msg[SIZE];
    va_list ap;

    va_start(ap, fmt);
    vsnprintf(msg, SIZE, fmt, ap);
    va_end(ap);
    fprintf(stderr, "%s:%d: %s\n", file, line, msg);
}

int
err_set(int new)
{
    int old;

    old = Type;
    Type = new;
    return old;
}

int
err_set_abort(void)
{
    return err_set(ERR_ABORT);
}

int
err_set_ignore(void)
{
    return err_set(ERR_IGNORE);
}
