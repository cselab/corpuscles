#include <stdio.h>

#include <real.h>
#include <co/err.h>
#include <co/off.h>
#include <co/read.h>
#include <co/he.h>

int main() {
    He      *he;
    Off *off;
    HeRead *read;
    int *tri;
    int nv, nt, nh;
    int h, f, i, j;
    const char path[] = "/dev/stdin";

    off_ini(path, &off);

    nv = off_nv(off);
    nt = off_nt(off);

    off_tri(off, &tri);
    he_read_tri_ini(nv, nt, tri, &read);
    he_ini(read, &he);

    nh = he_nh(he);
    for (h = 0; h < nh; h++) {
        if (he_bnd(he, h)) continue;
        f = he_flp(he, h);

        i = he_ver(he, h);
        j = he_ver(he, f);
        printf("%d %d\n", i, j);
    }

    he_read_fin(read);
    off_fin(off);
    he_fin(he);

    return CO_OK;
}
