#include <stdio.h>
#include <he/err.h>
#include <he/macro.h>
#include <he/read.h>

int main() {
    HeRead *read;
    he_read_ini("/dev/stdin", &read);
    he_info(read, stdout);
    he_read_fin(read);
}
