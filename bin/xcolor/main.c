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
    fprintf(stderr, "%s [-n] -v A.off lo hi B.off > C.vtk\n", me);
    fprintf(stderr, "%s [-b] -v A.off lo hi B.off > C.vtk\n", me);
    fprintf(stderr, "%s [-x] -v A.off lo hi B.off > C.vtk\n", me);
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
    enum {LIN, ABS, NABS, BIN, AXIS};
    int status, Output, Map;
    real *x, *y, *z, *c;
    real *u, *v, *w;
    He *p, *q;
    int i, n;
    real min, max, d;
    real lo, hi;

    err_set_ignore();
    Output = OFF;
    Map = LIN;
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
	    Map=ABS;
            break;
	case 'n':
	    Map=NABS;
	    break;
	case 'b':
	    Map=BIN;
	    break;
	case 'x':
	    Map=AXIS;
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
    switch (Map)
      {
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
	  c[i] = (max-fabs(u[i])) / max;	
	break;
      case BIN:
	for (i = 0; i < n; i++) {
	  if ( fabs(u[i]) >= 0.6 )
	    c[i] = 0;
	  else
	    c[i] = 1;
	}
	break;
      case AXIS:
	max=0;
	real t;
	for (i = 0; i < n; i++) {
	  t=sqrt(v[i]*v[i]+w[i]*w[i]);
	  if (t<0.3)
	    c[i] = 0;
	  else
	    c[i] = 1;	  
	}
	break;

      }
    const real *scal[] = { c, NULL };
    const char *name[] = { "color", NULL };
    switch (Output) {
    case VTK:
        vtk_fwrite(p, x, y, z, scal, name, stdout);	
	break;
    case OFF:
	boff_lh_ver_fwrite(p, x, y, z, lo, hi, c, stdout);
	break;
    case POV:
	povray_lh_ver_mesh2(p, x, y, z, lo, hi, c, stdout);
	break;
    }
    FREE(c);
    y_fin(p, x, y, z);
    y_fin(q, u, v, w);
}
