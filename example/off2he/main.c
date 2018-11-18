#include <stdio.h>

#include <real.h>
#include <he/err.h>
#include <he/read.h>
#include <he/off.h>

static HeRead *read;
static HeOff *off;

void ini() {
    int nv, nt, *tri;
    off_ini("/dev/stdin", &off);

    nv = off_nv(off);
    nt = off_nt(off);

    off_tri(off, &tri);
    he_read_tri_ini(nv, nt, tri, &read);
}

void fin() {
    he_read_fin(read);
    off_fin(off);
}

int main() {
    ini();
    fin();
    return HE_OK;
}
