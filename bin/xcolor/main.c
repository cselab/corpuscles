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

#define FMT_IN CO_REAL_IN

static int
scl(char **v, /**/ real * p)
{
    if (*v == NULL)
        ER("%s: not enough args", me);
    if (sscanf(*v, FMT_IN, p) != 1)
        ER("not a number '%s'", *v);
    return CO_OK;
}

static void
usg(void)
{
    fprintf(stderr, "%s A.off lo hi B.off > C.off\n", me);
    fprintf(stderr, "%s [-a] -v A.off lo hi B.off > C.vtk\n", me);
    fprintf(stderr, "%s [-a] -p A.off lo hi B.off > C.pov\n", me);
    fprintf(stderr, "%s [-a] [-v] A.off lo hi B.off > C.off\n", me);
    fprintf(stderr,
            "color vertices in B acording to (x-x_min)/(x_max-x_min) in A\n");
    fprintf(stderr,
            "if -a is given color vertices in B acording to abs(x)/x_max in A\n");
    exit(2);
}

int
main(int argc, char **a)
{
    enum {POV, VTK, OFF};
    int status, Output;
    real *x, *y, *z, *c;
    real *u, *v, *w;
    He *p, *q;
    int i, n, Abs, Vtk;
    real min, max, d;
    real lo, hi;

    err_set_ignore();
    Abs = 0;
    Output = OFF;
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
        case 'a':
            Abs = 1;
            break;
        default:
            fprintf(stderr, "%s: unknown option '%s'\n", me, a[0]);
            exit(1);
        }
    status = y_ini(*a, &q, &u, &v, &w);
    if (status != CO_OK)
        ER("not an off file '%s'", a[0]);
    scl(++a, &lo);
    scl(++a, &hi);
    a++;
    status = y_ini(*a, &p, &x, &y, &z);
    if (status != CO_OK)
        ER("not an off file '%s'", a[0]);
    n = he_nv(q);
    CALLOC(n, &c);
    min = array_min(n, u);
    max = array_max(n, u);
    d = max - min;
    if (Abs)
        for (i = 0; i < n; i++)
            c[i] = fabs(u[i]) / max;
    else
        for (i = 0; i < n; i++)
            c[i] = (u[i] - min) / d;

    const real *scal[] = { c, NULL };
    const char *name[] = { "color", NULL };
    switch (Output) {
    case OFF:
      boff_lh_ver_fwrite(p, x, y, z, lo, hi, c, stdout);
      break;
    case VTK:
      vtk_fwrite(p, x, y, z, scal, name, stdout);
      break;
    case POV:
	povray_ver_mesh2(p, x, y, z, c, stdout);
	break;
    }
    FREE(c);
    y_fin(p, x, y, z);
    y_fin(q, u, v, w);
}
