#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <real.h>
#include <co/array.h>
#include <co/err.h>
#include <co/he.h>
#include <co/macro.h>
#include <co/transform.h>
#include <co/util.h>
#include <co/off.h>
#include <co/vec.h>
#include <co/tri.h>
#include <co/y.h>
#include <co/memory.h>

static const char *me = "co.bead";

#define FMT_IN CO_REAL_IN
enum { X, Y, Z };

static void
usg(void)
{
    fprintf(stderr,
            "%s [-x|-y|-z] -b bead.off -r reference.off > C.off\n",
            me);
    exit(2);
}

static int tri(int, He *, const real *, const real *, const real *,
               /**/ real[3]);

int
main(int argc, char **argv)
{
    enum { MAX_X, MAX_Y, MAX_Z };
    He *bead;
    He *current;
    He *reference;
    real *p;
    real *q;
    real *r;
    real *u;
    real *v;
    real *w;
    real *u0;
    real *v0;
    real *w0;
    real *x;
    real *y;
    real *z;
    const char *Bead;
    const char *Reference;
    int i;
    int j;
    int n;
    int Type;
    real pos[3];
    real mpos[3];
    int Cond;

    Bead = Reference = NULL;
    Type = MAX_X;
    USED(argc);
    while (*++argv != NULL && argv[0][0] == '-')
        switch (argv[0][1]) {
        case 'h':
            usg();
            break;
        case 'b':
            argv++;
            if ((Bead = *argv) == NULL) {
                fprintf(stderr, "%s: -b needs an argument\n", me);
                exit(2);
            }
            break;
        case 'x':
            Type = MAX_X;
            break;
        case 'y':
            Type = MAX_Y;
            break;
        case 'z':
            Type = MAX_Z;
            break;
        case 'r':
            argv++;
            if ((Reference = *argv) == NULL) {
                fprintf(stderr, "%s: -r needs an argument\n", me);
                exit(2);
            }
            break;
        default:
            fprintf(stderr, "%s: unknown option '%s'\n", me, *argv);
            exit(1);
        }
    if (Reference == NULL) {
        fprintf(stderr, "%s: reference file is not set (-r)\n", me);
        exit(2);
    }
    if (Bead == NULL) {
        fprintf(stderr, "%s: bead file is not set (-b)\n", me);
        exit(2);
    }
    if (y_ini(Reference, &reference, &p, &q, &r) != CO_OK) {
        fprintf(stderr, "%s: fail to read file '%s'\n", me, Reference);
        exit(2);
    }
    if (y_ini(Bead, &bead, &u0, &v0, &w0) != CO_OK) {
        fprintf(stderr, "%s: fail to read file '%s'\n", me, Bead);
        exit(2);
    }
    if (y_inif(stdin, &current, &x, &y, &z) != CO_OK) {
        fprintf(stderr, "%s: fail to read file from stdin\n", me);
        exit(2);
    }

    n = he_nv(bead);
    MALLOC3(n, &u, &v, &w);
    n = he_nt(reference);
    for (j = i = 0; i < n; i++) {
        tri(i, reference, p, q, r, pos);
        switch (Type) {
        case MAX_X:
            Cond = pos[X] > mpos[X];
            break;
        case MAX_Y:
            Cond = pos[Y] > mpos[Y];
            break;
        case MAX_Z:
            Cond = pos[Z] > mpos[Z];
            break;
        }
        if (i == 0 || Cond) {
            j = i;
            vec_copy(pos, mpos);
        }
    }
    tri(j, current, x, y, z, pos);
    n = he_nv(bead);
    array_copy3(n, u0, v0, w0, u, v, w);
    transform_tran(pos, n, u, v, w);
    if (off_he_xyz_fwrite(bead, u, v, w, stdout) != CO_OK) {
        fprintf(stderr, "%s: fail to write to stdout\n", me);
        exit(2);
    }
    y_fin(reference, p, q, r);
    y_fin(bead, u0, v0, w0);
    FREE3(u, v, w);
    y_fin(current, x, y, z);
}

static int
tri(int t, He * he, const real * x, const real * y, const real * z,
    /**/ real r[3])
{
    int i;
    int j;
    int k;
    real a[3];
    real b[3];
    real c[3];

    if (he_tri_ijk(he, t, &i, &j, &k) != CO_OK)
        ER("tri failed for t = %d, nv = %d", t, he_nv(he));
    vec_get3(i, j, k, x, y, z, a, b, c);
    tri_center(a, b, c, r);
    return CO_OK;
}
