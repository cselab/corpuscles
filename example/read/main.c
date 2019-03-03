#include <stdio.h>
#include <co/err.h>
#include <co/macro.h>
#include <co/read.h>

int main() {
    HeRead *read;
    he_read_ini("/dev/stdin", &read);
    he_read_info(read, stdout);
    he_read_fin(read);
}
