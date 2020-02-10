#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <real.h>
#include <co/argv.h>
#include <co/array.h>
#include <co/err.h>
#include <co/he.h>
#include <co/macro.h>
#include <co/force.h>
#include <co/povray.h>
#include <co/util.h>
#include <co/off.h>
#include <co/y.h>
#include <co/vtk.h>
#include <co/memory.h>

static const char *me = "co.xcolor";

#define IN CO_REAL_IN
#define OUT CO_REAL_OUT

static int
scl(char **v, /**/ real * p)
{
    if (*v == NULL)
        ER("%s: not enough args", me);
    if (sscanf(*v, IN, p) != 1)
        ER("not a number '%s'", *v);
    return CO_OK;
}

static int
filter(He * p, real * c)
{
    int i;
    int j;
    int k;
    int nt;
    int nv;
    int *rank;
    int t;
    real *c0;

    nv = he_nv(p);
    nt = he_nt(p);

    CALLOC(nv, &c0);
    CALLOC(nv, &rank);
    for (t = 0; t < nt; t++) {
        he_tri_ijk(p, t, &i, &j, &k);
        c0[i] += c[j];
        rank[i]++;
        c0[i] += c[k];
        rank[i]++;
        c0[j] += c[i];
        rank[j]++;
        c0[j] += c[k];
        rank[j]++;
        c0[k] += c[i];
        rank[k]++;
        c0[k] += c[j];
        rank[k]++;
    }
    for (i = 0; i < nv; i++) {
        if (rank[i] == 0) {
            fprintf(stderr, "%s: rank[%d] == 0\n", me, i);
            exit(2);
        }
        c[i] = c0[i] / rank[i];
    }
    FREE(rank);
    FREE(c0);
    return CO_OK;
}

static void
usg(void)
{
    fprintf(stderr,
            "%s [-a|t|p|n|b] [-f int] [-s float] [-o float] A.off lo hi B.off > C.off\n",
            me);
    fprintf(stderr,
            "color vertices in B acording to (x-x_min)/(x_max-x_min) in A.off\n");
    fprintf(stderr,
            "if -a is given color vertices in B acording to abs(x)/x_max in A\n");
    fprintf(stderr, "-t output ascii OFF (default is binary OFF)\n");
    fprintf(stderr, "-p output POVRay file\n");
    fprintf(stderr,
            "[-o float] sets opacity, should be in [0, 1], default is 0.5\n");
    exit(2);
}

int
main(int argc, char **a)
{
    enum { POV, TXT, VTK, OFF };
    enum { LIN, ABS, NABS, BIN };
    He *p, *q;
    int Filter;
    int i, n;
    int Map;
    int Output;
    int status;
    real hi;
    real lo;
    real min, max, d;
    real opacity;
    real *u, *v, *w;
    real *x, *y, *z, *c;
    real sharp;
    int Sharp;

    USED(argc);
    err_set_ignore();
    Output = OFF;
    Map = LIN;
    Filter = 0;
    opacity = 0.5;
    Sharp = 0;
    while (*++a != NULL && a[0][0] == '-')
        switch (a[0][1]) {
        case 'h':
            usg();
            break;
        case 'v':
            Output = VTK;
            break;
        case 'p':
            Output = POV;
            break;
        case 't':
            Output = TXT;
            break;
        case 'a':
            Map = ABS;
            break;
        case 'n':
            Map = NABS;
            break;
        case 'b':
            Map = BIN;
            break;
        case 'f':
            a++;
            if (*a == NULL) {
                fprintf(stderr, "%s: filter (-f) needs a value\n", me);
                exit(2);
            }
            argv_int(&a, &Filter);
            break;
        case 'o':
            a++;
            if (*a == NULL) {
                fprintf(stderr, "%s: opacity option (-o) needs a value\n",
                        me);
                exit(2);
            }
            scl(a, &opacity);
            if (opacity < 0 && opacity > 1) {
                fprintf(stderr, "%s: opaciy = " OUT " is not in [0, 1]\n",
                        me, opacity);
                exit(2);
            }
            break;
        case 's':
            a++;
            if (*a == NULL) {
                fprintf(stderr, "%s: sharp (-s) needs a value\n",
                        me);
                exit(2);
            }
            scl(a, &sharp);
            Sharp = 1;
            break;
        default:
            fprintf(stderr, "%s: unknown option '%s'\n", me, *a);
            exit(2);
        }
    status = y_ini(*a, &q, &u, &v, &w);
    if (status != CO_OK)
        ER("not an off file '%s'", a[0]);

    scl(++a, &lo);
    scl(++a, &hi);
    a++;
    if (*a == NULL) {
        status = y_inif(stdin, &p, &x, &y, &z);
    } else
        status = y_ini(*a, &p, &x, &y, &z);
    if (status != CO_OK) {
        fprintf(stderr, "%s: fail to read an OFF file\n", me);
        exit(1);
    }
    n = he_nv(q);
    CALLOC(n, &c);
    min = array_min(n, u);
    max = array_max(n, u);
    d = max - min;
    switch (Map) {
    case LIN:
        for (i = 0; i < n; i++)
            c[i] = (u[i] - min) / d;
        break;
    case ABS:
        for (i = 0; i < n; i++)
            c[i] = fabs(u[i]) / max;
        break;
    case NABS:
        for (i = 0; i < n; i++)
            c[i] = (max - fabs(u[i])) / max;
        break;
    case BIN:
        for (i = 0; i < n; i++) {
            if (fabs(u[i]) >= 0.6)
                c[i] = 0;
            else
                c[i] = 1;
        }
        break;
    }

    if (lo > hi) {
        hi = -hi;
        lo = -lo;
        array_neg(n, c);
    }
    if (Sharp) {
        for (i = 0; i < n; i++)
            c[i] = c[i] < sharp ? lo : hi;
    }
    for (i = 0; i < Filter; i++)
        filter(p, c);

    const real *scal[] = { c, NULL };
    const char *name[] = { "color", NULL };
    switch (Output) {
    case VTK:
        vtk_write(p, x, y, z, scal, name, stdout);
        break;
    case TXT:
        off_lh_ver_alpha_fwrite(p, x, y, z, lo, hi, opacity, c, stdout);
        break;
    case OFF:
        boff_lh_ver_alpha_fwrite(p, x, y, z, lo, hi, opacity, c, stdout);
        break;
    case POV:
        povray_lh_ver_mesh2(p, x, y, z, lo, hi, c, stdout);
        break;
    }
    FREE(c);
    y_fin(p, x, y, z);
    y_fin(q, u, v, w);
}
