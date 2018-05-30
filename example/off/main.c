#include <stdio.h>

#include <real.h>
#include <he/err.h>
#include <he/off.h>

static HeOff *read;
static void ini() {
    he_off_ini("/dev/stdin", &read);
}

static void fin() { he_off_fin(read); }

int main() {
    ini();
    fin();
}
