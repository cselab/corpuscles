#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <real.h>
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

static void
usg(void)
{
    fprintf(stderr,
            "%s [-a|t|p|n|b|x] [-o float] A.off lo hi B.off > C.off\n",
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
    enum { LIN, ABS, NABS, BIN, AXIS };
    int status, Output, Map;
    real *x, *y, *z, *c;
    real *u, *v, *w;
    He *p, *q;
    int i, n;
    real min, max, d;
    real lo, hi;
    real opacity;
    real t;

    USED(argc);
    err_set_ignore();
    Output = OFF;
    Map = LIN;
    opacity = 0.5;
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
        case 'x':
            Map = AXIS;
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
    case AXIS:
        max = 0;
        for (i = 0; i < n; i++) {
            t = sqrt(v[i] * v[i] + w[i] * w[i]);
            if (t < 0.3)
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
