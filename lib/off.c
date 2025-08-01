#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "real.h"
#include "co/array.h"
#include "co/colormap.h"
#include "co/endian.h"
#include "co/err.h"
#include "co/he.h"
#include "co/memory.h"
#include "co/util.h"
#include "inc/def.h"

#include "co/off.h"

#define T Off
enum { SIZE = MAX_STRING_SIZE };

#define FWRITE(ptr, size)					     \
    do								     \
    if (size != (cnt = fwrite(ptr, sizeof((ptr)[0]), size, f)))	     \
	ERR(CO_IO, "fwrite failed: need = %d, got = %d", size, cnt); \
    while (0)
#define FREAD(n, p)					\
    do							\
    if ((int)fread(p, sizeof(*(p)), (n), (f)) != (n))	\
	ERR(CO_IO, "fread failed, n = %d", n);		\
    while (0)

#define FMT CO_REAL_IN
#define OUT CO_REAL_OUT

struct T {
    real *ver;                  /* x[0] y[0] z[0] ... */
    int *tri;                   /* t0[0] t1[0] t2[0] ... */
    int nv, nt;
};

#define NXT() if (util_comment_fgets(line, f) == NULL)  \
	ERR(CO_IO, "unexpected EOF")
static int
read_text(T * q, FILE * f)
{
    char line[SIZE];
    int i, nv, nt;
    int *t0, *t1, *t2, cnt, np;
    real *ver, *x, *y, *z;
    int *tri;

    NXT();
    cnt = sscanf(line, "%d %d %*d", &nv, &nt);
    if (cnt != 2)
        ERR(CO_IO, "fail to parse: '%s'", line);
    if (3 * nt < nv)
        ERR(CO_IO, "3*(nt=%d)   <   nv=%d", nt, nv);

    MALLOC(3 * nv, &q->ver);
    ver = q->ver;
    MALLOC(3 * nt, &q->tri);
    tri = q->tri;
    for (i = 0; i < nv; i++) {
        NXT();
        x = ver++;
        y = ver++;
        z = ver++;
        cnt = sscanf(line, FMT " " FMT " " FMT, x, y, z);
        if (cnt != 3)
            ERR(CO_IO, "wrong vertex line '%s'", line);
    }

    for (i = 0; i < nt; i++) {
        NXT();
        t0 = tri++;
        t1 = tri++;
        t2 = tri++;
        cnt = sscanf(line, "%d %d %d %d", &np, t0, t1, t2);
        if (cnt != 4)
            ERR(CO_IO, "wrong triangle line '%s'", line);
        if (np != 3)
            ERR(CO_IO, "not a triangle '%s'", line);
    }
    q->nv = nv;
    q->nt = nt;
    return CO_OK;
}

static int
read_binary(T * q, int Color, FILE * f)
{
    enum { X, Y, Z };
    int i, j, nv, nt, ibuf[1 + 3], np, nc;
    real *ver;
    int *tri;
    float fbuf[7];

    FREAD(3, ibuf);
    big_endian_int(3, ibuf);
    nv = ibuf[0];
    nt = ibuf[1];
    MALLOC(3 * nv, &ver);
    MALLOC(3 * nt, &tri);
    for (i = j = 0; i < nv; i++) {
        if (Color)
            FREAD(7, fbuf);
        else
            FREAD(3, fbuf);
        big_endian_flt(3, fbuf);
        ver[j++] = fbuf[X];
        ver[j++] = fbuf[Y];
        ver[j++] = fbuf[Z];
    }

    for (i = j = 0; i < nt; i++) {
        FREAD(1, &np);
        big_endian_int(1, &np);
	if (np != 3)
	  ERR(CO_IO, "np=%d != 3", np);
        FREAD(3, ibuf);
        big_endian_int(3, ibuf);
        tri[j++] = ibuf[0];
        tri[j++] = ibuf[1];
        tri[j++] = ibuf[2];
        FREAD(1, &nc);          /* skip colors */
        big_endian_int(1, &nc);
        if (nc > 0)
            FREAD(nc, fbuf);
    }

    q->tri = tri;
    q->ver = ver;
    q->nv = nv;
    q->nt = nt;
    return CO_OK;
}

int
off_inif(FILE * f, T ** pq)
{
    T *q;
    char line[SIZE];
    int status, Color;

    MALLOC(1, &q);
    if (f == NULL)
        ERR(CO_IO, "fail to read");
    NXT();
    if (util_eq(line, "OFF"))
        status = read_text(q, f);
    else if (util_eq(line, "OFF BINARY"))
        status = read_binary(q, Color = 0, f);
    else if (util_eq(line, "COFF BINARY"))
        status = read_binary(q, Color = 1, f);
    else
        ERR(CO_IO, "expecting 'OFF' or 'COFF' got '%s'", line);
    if (status != CO_OK)
        ERR(CO_IO, "read_xt failed");
    *pq = q;
    return CO_OK;
}

#undef NXT
int
off_ini(const char *path, T ** pq)
{
    FILE *f;

    if ((f = fopen(path, "r")) == NULL)
        ERR(CO_IO, "fail to open '%s'", path);
    if (off_inif(f, pq) != CO_OK)
        ERR(CO_IO, "off_fini failed for '%s", path);
    if (fclose(f) != 0)
        ERR(CO_IO, "fail to close '%s'", path);
    return CO_OK;
}

int
off_fin(T * q)
{
    FREE(q->ver);
    FREE(q->tri);
    FREE(q);
    return CO_OK;
}

int
off_nt(T * q)
{
    return q->nt;
}

int
off_nv(T * q)
{
    return q->nv;
};

int
off_ver(T * q, real ** p)
{
    *p = q->ver;
    return CO_OK;
}

int
off_xyz(T * q, real * x, real * y, real * z)
{
    int i, nv;
    real *ver;

    ver = q->ver;
    nv = q->nv;
    for (i = 0; i < nv; i++) {
        x[i] = *ver++;
        y[i] = *ver++;
        z[i] = *ver++;
    }
    return CO_OK;
}

int
off_tri(T * q, int **p)
{
    *p = q->tri;
    return CO_OK;
}

int
off_fwrite(T * q, const real * x, const real * y, const real * z,
           /**/ FILE * f)
{
    int nv, nt, ne, npv, *tri, m, i, j, k;

    if (fputs("OFF\n", f) == EOF)
        ERR(CO_IO, "fail to write");
    nv = q->nv;
    nt = q->nt;
    ne = 0;
    npv = 3;
    tri = q->tri;

    fprintf(f, "%d %d %d\n", nv, nt, ne);
    for (m = 0; m < nv; m++)
        fprintf(f, OUT " " OUT " " OUT "\n", x[m], y[m], z[m]);

    for (m = 0; m < nt; m++) {
        i = *tri++;
        j = *tri++;
        k = *tri++;
        fprintf(f, "%d %d %d %d\n", npv, i, j, k);
    }
    return CO_OK;
}

int
off_write0(T * q, const real * x, const real * y, const real * z,
           /**/ const char *path)
{
    FILE *f;

    if ((f = fopen(path, "w")) == NULL)
        ERR(CO_IO, "fail to open '%s'", path);
    if (off_fwrite(q, x, y, z, f) != CO_OK)
        ERR(CO_IO, "fail to write to '%s", path);
    if (fclose(f) != 0)
        ERR(CO_IO, "fail to close '%s'", path);
    return CO_OK;
}

int
off_tri_fwrite(T * q, const int *tri, /**/ FILE * f)
{
    int nv, nt, ne, npv, m, i, j, k;
    real x, y, z;
    const real *ver;

    ver = q->ver;
    if (fputs("OFF\n", f) == EOF)
        ERR(CO_IO, "fail to write");
    nv = q->nv;
    nt = q->nt;
    ne = 0;
    npv = 3;

    fprintf(f, "%d %d %d\n", nv, nt, ne);
    for (i = m = 0; m < nv; m++) {
        x = ver[i++];
        y = ver[i++];
        z = ver[i++];
        fprintf(f, OUT " " OUT " " OUT "\n", x, y, z);
    }

    for (m = 0; m < nt; m++) {
        i = *tri++;
        j = *tri++;
        k = *tri++;
        fprintf(f, "%d %d %d %d\n", npv, i, j, k);
    }
    return CO_OK;
}

int
off_tri_write(T * q, const int *tri, /**/ const char *path)
{
    FILE *f;

    if ((f = fopen(path, "w")) == NULL)
        ERR(CO_IO, "fail to open '%s'", path);
    if (off_tri_fwrite(q, tri, f) != CO_OK)
        ERR(CO_IO, "fail to write to '%s", path);
    if (fclose(f) != 0)
        ERR(CO_IO, "fail to close '%s'", path);
    return CO_OK;
}

int
off_he_fwrite(T * q, He * he, /**/ FILE * f)
{
    int nv, nt, ne, npv, m, h, n, nn, i, j, k;
    real x, y, z;
    const real *ver;

    ver = q->ver;
    if (fputs("OFF\n", f) == EOF)
        ERR(CO_IO, "fail to write");
    nv = he_nv(he);
    nt = he_nt(he);
    ne = 0;
    npv = 3;
    if (q->nv != nv)
        ERR(CO_INDEX, "q->nv=%d   !=   nv=%d", q->nv, nv);

    fprintf(f, "%d %d %d\n", nv, nt, ne);
    for (i = m = 0; m < nv; m++) {
        x = ver[i++];
        y = ver[i++];
        z = ver[i++];
        fprintf(f, OUT " " OUT " " OUT "\n", x, y, z);
    }

    for (m = 0; m < nt; m++) {
        h = he_hdg_tri(he, m);
        n = he_nxt(he, h);
        nn = he_nxt(he, n);
        i = he_ver(he, h);
        j = he_ver(he, n);
        k = he_ver(he, nn);
        fprintf(f, "%d %d %d %d\n", npv, i, j, k);
    }
    return CO_OK;
}

int
off_he_write(T * q, He * he, /**/ const char *path)
{
    FILE *f;

    if ((f = fopen(path, "w")) == NULL)
        ERR(CO_IO, "fail to open '%s'", path);
    if (off_he_fwrite(q, he, f) != CO_OK)
        ERR(CO_IO, "fail to write to '%s", path);
    if (fclose(f) != 0)
        ERR(CO_IO, "fail to close '%s'", path);
    return CO_OK;
}

int
off_xyz_tri_fwrite(int nv, const real * xyz, int nt, const int *tri,
                   FILE * f)
{
    int ne, npv, i, j, k;
    real x, y, z;

    if (fputs("OFF\n", f) == EOF)
        ERR(CO_IO, "fail to write");
    ne = 0;
    npv = 3;
    fprintf(f, "%d %d %d\n", nv, nt, ne);
    while (nv--) {
        x = *xyz++;
        y = *xyz++;
        z = *xyz++;
        fprintf(f, OUT " " OUT " " OUT "\n", x, y, z);
    }
    while (nt--) {
        i = *tri++;
        j = *tri++;
        k = *tri++;
        fprintf(f, "%d %d %d %d\n", npv, i, j, k);
    }
    return CO_OK;
}

int
off_he_xyz_fwrite(He * he, const real * x, const real * y, const real * z,
                  /**/ FILE * f)
{
    int nv, nt, ne, npv, m, i, j, k;

    if (fputs("OFF\n", f) == EOF)
        ERR(CO_IO, "fail to write");
    nv = he_nv(he);
    nt = he_nt(he);
    ne = 0;
    npv = 3;
    fprintf(f, "%d %d %d\n", nv, nt, ne);
    for (m = 0; m < nv; m++)
        fprintf(f, OUT " " OUT " " OUT "\n", x[m], y[m], z[m]);
    for (m = 0; m < nt; m++) {
        he_tri_ijk(he, m, &i, &j, &k);
        fprintf(f, "%d %d %d %d\n", npv, i, j, k);
    }
    return CO_OK;
}

int
off_he_xyz_write(He * he, const real * x, const real * y, const real * z,
                 /**/ const char *path)
{
    FILE *f;

    if ((f = fopen(path, "w")) == NULL)
        ERR(CO_IO, "fail to open '%s'", path);
    if (off_he_xyz_fwrite(he, x, y, z, f) != CO_OK)
        ERR(CO_IO, "fail to write to '%s", path);
    if (fclose(f) != 0)
        ERR(CO_IO, "fail to close '%s'", path);
    return CO_OK;
}

int
boff_fwrite(He * he, const real * x, const real * y, const real * z,
            /**/ FILE * f)
{
    int nv, nt, ne, npv, nc, m, i, j, k;
    int ib[5], n, cnt;
    float db[3];

    if (fputs("OFF BINARY\n", f) == EOF)
        ERR(CO_IO, "fail to write");
    nv = he_nv(he);
    nt = he_nt(he);
    ne = 0;
    npv = 3;
    nc = 0;

    n = 0;
    ib[n++] = nv;
    ib[n++] = nt;
    ib[n++] = ne;
    big_endian_int(n, ib);
    FWRITE(ib, n);
    for (m = 0; m < nv; m++) {
        n = 0;
        db[n++] = x[m];
        db[n++] = y[m];
        db[n++] = z[m];
        big_endian_flt(n, db);
        FWRITE(db, n);
    }

    for (m = 0; m < nt; m++) {
        he_tri_ijk(he, m, &i, &j, &k);
        n = 0;
        ib[n++] = npv;
        ib[n++] = i;
        ib[n++] = j;
        ib[n++] = k;
        ib[n++] = nc;
        big_endian_int(n, ib);
        FWRITE(ib, n);
    }
    return CO_OK;
}

int
boff_lh_tri_fwrite(He * he, const real * x, const real * y, const real * z,
                   real lo, real hi, const real * a, /**/ FILE * f)
{
    int nv, nt, ne, npv, nc, m, i, j, k;
    int ib[5], n, cnt;
    float db[4];
    float red, blue, green, alpha;

    if (fputs("OFF BINARY\n", f) == EOF)
        ERR(CO_IO, "fail to write");
    nv = he_nv(he);
    nt = he_nt(he);
    ne = 0;
    npv = 3;
    nc = 4;
    alpha = 0.5;

    n = 0;
    ib[n++] = nv;
    ib[n++] = nt;
    ib[n++] = ne;
    big_endian_int(n, ib);
    FWRITE(ib, n);
    for (m = 0; m < nv; m++) {
        n = 0;
        db[n++] = x[m];
        db[n++] = y[m];
        db[n++] = z[m];
        big_endian_flt(n, db);
        FWRITE(db, n);
    }

    for (m = 0; m < nt; m++) {
        he_tri_ijk(he, m, &i, &j, &k);
        n = 0;
        ib[n++] = npv;
        ib[n++] = i;
        ib[n++] = j;
        ib[n++] = k;
        ib[n++] = nc;
        big_endian_int(n, ib);
        FWRITE(ib, n);

        if (colormap(a[m], lo, hi, &red, &green, &blue) != CO_OK)
            ERR(CO_IO, "colormap failed");
        n = 0;
        db[n++] = red;
        db[n++] = green;
        db[n++] = blue;
        db[n++] = alpha;
        big_endian_flt(n, db);
        FWRITE(db, n);
    }
    return CO_OK;
}

int
boff_tri_fwrite(He * he, const real * x, const real * y, const real * z,
                const real * a, /**/ FILE * f)
{
    int nt;
    real l, h;

    nt = he_nt(he);
    l = array_min(nt, a);
    h = array_max(nt, a);

    return boff_lh_tri_fwrite(he, x, y, z, l, h, a, f);
}

int
boff_lh_ver_fwrite(He * he, const real * x, const real * y, const real * z,
                   real lo, real hi, const real * a, /**/ FILE * f)
{
    real alpha;

    alpha = 0.5;
    return boff_lh_ver_alpha_fwrite(he, x, y, z, lo, hi, alpha, a, f);
}

int
boff_lh_ver_alpha_fwrite(He * he, const real * x, const real * y,
                         const real * z, real lo, real hi, real alpha,
                         const real * a, /**/ FILE * f)
{
    int nv, nt, ne, npv, nc, m, i, j, k;
    int ib[5], n, cnt;
    float db[7];
    float red, green, blue;

    if (fputs("COFF BINARY\n", f) == EOF)
        ERR(CO_IO, "fail to write");
    nv = he_nv(he);
    nt = he_nt(he);
    ne = 0;
    npv = 3;
    nc = 0;

    n = 0;
    ib[n++] = nv;
    ib[n++] = nt;
    ib[n++] = ne;
    big_endian_int(n, ib);
    FWRITE(ib, n);
    for (m = 0; m < nv; m++) {
        if (colormap(a[m], lo, hi, &red, &green, &blue) != CO_OK)
            ERR(CO_IO, "colormap failed");
        n = 0;
        db[n++] = x[m];
        db[n++] = y[m];
        db[n++] = z[m];
        db[n++] = red;
        db[n++] = green;
        db[n++] = blue;
        db[n++] = alpha;
        big_endian_flt(n, db);
        FWRITE(db, n);
    }

    for (m = 0; m < nt; m++) {
        he_tri_ijk(he, m, &i, &j, &k);
        n = 0;
        ib[n++] = npv;
        ib[n++] = i;
        ib[n++] = j;
        ib[n++] = k;
        ib[n++] = nc;
        big_endian_int(n, ib);
        FWRITE(ib, n);
    }
    return CO_OK;
}

int
off_lh_ver_alpha_fwrite(He * he, const real * x, const real * y,
                        const real * z, real lo, real hi, real alpha,
                        const real * a, /**/ FILE * f)
{
    int nv, nt, ne, npv, m, i, j, k;
    float red, green, blue;

    if (alpha < 0 && alpha > 1)
        ERR(CO_IO, "alpha = " OUT " is not in [0, 1]", alpha);
    if (fputs("COFF\n", f) == EOF)
        ERR(CO_IO, "fail to write");
    nv = he_nv(he);
    nt = he_nt(he);
    ne = 0;
    npv = 3;

    fprintf(f, "%d %d %d\n", nv, nt, ne);
    for (m = 0; m < nv; m++) {
        if (colormap(a[m], lo, hi, &red, &green, &blue) != CO_OK)
            ERR(CO_IO, "colormap failed");
        fprintf(f, FMT " " FMT " " FMT " %.16g %.16g %.16g " FMT "\n",
                x[m], y[m], z[m], red, green, blue, alpha);
    }
    for (m = 0; m < nt; m++) {
        if (he_tri_ijk(he, m, &i, &j, &k) != CO_OK)
            ERR(CO_OK, "hi_tri_ikj failed for t = %d", m);
        fprintf(f, "%d %d %d %d\n", npv, i, j, k);
    }
    return CO_OK;
}

int
off_lh_ver_fwrite(He * he, const real * x, const real * y, const real * z,
                  real lo, real hi, const real * a, /**/ FILE * f)
{
    real alpha;

    alpha = 0.5;
    return off_lh_ver_alpha_fwrite(he, x, y, z, lo, hi, alpha, a, f);
}

int
boff_ver_fwrite(He * he, const real * x, const real * y, const real * z,
                const real * a, /**/ FILE * f)
{
    int n;
    real l, h;

    n = he_nv(he);
    l = array_min(n, a);
    h = array_max(n, a);
    return boff_lh_ver_fwrite(he, x, y, z, l, h, a, f);
}

int
boff_vect_fwrite(He * he, const real * xx, const real * yy,
                 const real * zz, const real * uu, const real * vv,
                 const real * ww, /**/ FILE * f)
{
#define P(x, y, z) do {                              \
	fb[m++] = (x); fb[m++] = (y); fb[m++] = (z);    \
    } while (0)

    int n, nv, nc, np, i, cnt, m;
    real x, y, z, u, v, w;

    short *sb;
    int ib[3];
    float *fb;

    n = he_nv(he);
    nc = 0;                     /* number of colors */
    if (fputs("VECT BINARY\n", f) == EOF)
        ERR(CO_IO, "fail to write");

    np = n + n;
    nv = 2 * n + 3 * n;

    MALLOC(np, &sb);
    MALLOC(3 * nv, &fb);

    m = 0;
    ib[m++] = np;
    ib[m++] = nv;
    ib[m++] = nc;
    big_endian_int(m, ib);
    FWRITE(ib, m);

    m = 0;
    for (i = 0; i < n; i++)
        sb[m++] = 2;
    for (i = 0; i < n; i++)
        sb[m++] = 3;
    big_endian_short(m, sb);
    FWRITE(sb, m);

    m = 0;
    for (i = 0; i < np; i++)
        sb[m++] = 0;
    big_endian_short(m, sb);
    FWRITE(sb, m);

    m = 0;
    for (i = 0; i < n; i++) {
        x = xx[i];
        y = yy[i];
        z = zz[i];
        u = uu[i];
        v = vv[i];
        w = ww[i];
        P(x, y, z);
        P(x + u, y + v, z + w);
    }
    for (i = 0; i < n; i++) {
        u = uu[i];
        v = vv[i];
        w = ww[i];
        x = xx[i] + u;
        y = yy[i] + v;
        z = zz[i] + w;
        P(x - (u - v / 2) / 5, y - (u / 2 + v) / 5, z);
        P(x, y, z);
        P(x - (u + v / 2) / 5, y + (u / 2 - v) / 5, z - 2 * w / 5);
    }
    big_endian_flt(m, fb);
    FWRITE(fb, m);

    FREE(sb);
    FREE(fb);
    return CO_OK;
#undef P
}

int
boff_lh_point_fwrite(He * he, const real * x, const real * y,
                     const real * z, real lo, real hi, const real * a,
                     /**/ FILE * f)
{
    int n, m, i, cnt;
    float red, blue, green, alpha;
    int ib[3];
    short *sb;
    float *fb;

    n = he_nv(he);
    alpha = 0.5;
    if (fputs("VECT BINARY\n", f) == EOF)
        ERR(CO_IO, "fail to write");
    MALLOC(2 * n, &sb);
    MALLOC(4 * n, &fb);

    m = 0;
    ib[m++] = n;
    ib[m++] = n;
    ib[m++] = n;
    big_endian_int(m, ib);
    FWRITE(ib, m);

    m = 0;
    for (i = 0; i < n; i++)
        sb[m++] = 1;
    for (i = 0; i < n; i++)
        sb[m++] = 1;
    big_endian_short(m, sb);
    FWRITE(sb, m);

    m = 0;
    for (i = 0; i < n; i++) {
        fb[m++] = x[i];
        fb[m++] = y[i];
        fb[m++] = z[i];
    }
    big_endian_flt(m, fb);
    FWRITE(fb, m);

    m = 0;
    for (i = 0; i < n; i++) {
        if (colormap(a[i], lo, hi, &red, &green, &blue) != CO_OK)
            ERR(CO_IO, "colormap failed");
        fb[m++] = red;
        fb[m++] = green;
        fb[m++] = blue;
        fb[m++] = alpha;
    }
    big_endian_flt(m, fb);
    FWRITE(fb, m);

    FREE(sb);
    FREE(fb);
    return CO_OK;
}

int
boff_point_fwrite(He * he, const real * x, const real * y, const real * z,
                  const real * a, /**/ FILE * f)
{
    int nt;
    real l, h;

    nt = he_nt(he);
    l = array_min(nt, a);
    h = array_max(nt, a);
    return boff_lh_point_fwrite(he, x, y, z, l, h, a, f);
}
