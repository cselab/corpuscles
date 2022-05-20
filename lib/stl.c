#include <stdio.h>
#include <stdint.h>
#include "real.h"
#include "co/err.h"
#include "co/he.h"
#include "co/stl.h"
#include "co/vec.h"
#include "co/tri.h"

#define FWRITE(ptr, size)					     \
    do								     \
    if (size != (cnt = fwrite(ptr, 1, size, f)))	     \
	ERR(CO_IO, "fwrite failed: need = %d, got = %d", size, cnt); \
    while (0)

int
stl_write(He * he, const real * x, const real * y, const real * z,
          /**/ FILE * f)
{
    enum { X, Y, Z };
    float r[3];
    int cnt;
    int i;
    int j;
    int k;
    int t;
    real a[3];
    real b[3];
    real c[3];
    real normal[3];
    uint16_t count;
    uint32_t nt;
    uint8_t header[80] = { 0 };

    FWRITE(header, sizeof header);
    nt = he_nt(he);
    FWRITE(&nt, sizeof nt);
    for (t = 0; t < nt; t++) {
        he_tri_ijk(he, t, &i, &j, &k);
        vec_get(i, x, y, z, a);
        vec_get(j, x, y, z, b);
        vec_get(k, x, y, z, c);
        tri_normal(a, b, c, normal);

        r[X] = normal[X];
        r[Y] = normal[Y];
        r[Z] = normal[Z];
        FWRITE(r, sizeof r);

        r[X] = a[X];
        r[Y] = a[Y];
        r[Z] = a[Z];
        FWRITE(r, sizeof r);

        r[X] = b[X];
        r[Y] = b[Y];
        r[Z] = b[Z];
        FWRITE(r, sizeof r);

        r[X] = c[X];
        r[Y] = c[Y];
        r[Z] = c[Z];
        FWRITE(r, sizeof r);

        FWRITE(&count, sizeof count);
    }

    return CO_OK;
}
