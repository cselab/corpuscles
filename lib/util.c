#include <stdlib.h>
#include <stdio.h>

#include "he/err.h"
#include "he/util.h"

#define SIZE (4096)

void util_mkdir(const char *path) {
    char command[SIZE];
    snprintf(command, SIZE, "mkdir -p '%s'", path);
    if (system(command) != 0)
        ERR("comamnd <%s> faild", command);
}
