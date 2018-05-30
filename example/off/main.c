#include <stdio.h>

#include <real.h>
#include <he/err.h>
#include <he/off.h>

static HeOff *read;
static real *ver;
static int  nv, nt, *tri;

static void ini() {
    he_off_ini("/dev/stdin", &read);
    nv = he_off_nv(read);
    nt = he_off_nt(read);
    he_off_ver(read, &ver);
    he_off_tri(read, &tri);
}
static void fin() { he_off_fin(read); }

int main() {
    enum {X, Y, Z};
    int i;
    real x, y, z, *r;
    ini();
    for (i = 0, r = ver; i < nv; i++) {
        x = *r++; y = *r++; z = *r++;
        printf("%g %g %g\n", x, y, z);
    }
    
    fin();
}
