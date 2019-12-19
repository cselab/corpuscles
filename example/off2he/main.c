#include <stdio.h>

#include <real.h>
#include <co/err.h>
#include <co/read.h>
#include <co/off.h>

static HeRead *read;
static Off *off;

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
    return CO_OK;
}
