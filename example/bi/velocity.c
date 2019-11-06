#include <stdio.h>
#include <stdlib.h>
#include <real.h>
#include <co/area.h>
#include <co/bi/cortez_zero.h>
#include <co/err.h>
#include <co/he.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/vec.h>
#include <co/y.h>

#define FMT   CO_REAL_OUT

static char me[] = "vtk/off";
static void
usg()
{
  fprintf(stderr, "%s -b xl yl zl xh yh zh -n nx ny nz < OFF > VTK\n", me);
  exit(1);
}

int
main(int argc, char **argv)
{
    int i, n;
    BiCortezZero *cortez;
    He *he;    
    real *x, *y, *z, *fx, *fy, *fz, *area;

    USED(argc);
    while (*++argv != NULL && argv[0][0] == '-')
      switch (argv[0][1]) {
      case 'h':
	usg();
	break;
      }
    y_inif(stdin, &he, &x, &y, &z);
    bi_cortez_zero_ini(he, &cortez);
    n = he_nv(he);
    MALLOC3(n, &fx, &fy, &fz);
    MALLOC(n, &area);
    he_area_ver(he, x, y, z, area);    
    for (i = 0; i < n; i++) {
      fx[i] = area[i];
      fy[i] = 0;
      fz[i] = 0;
    }
    real r[] = {0, 0, -1.01};
    real v[3];
    bi_cortez_zero_single_velocity(cortez, he, x, y, z, fx, fy, fz, r, v);
    y_fin(he, x, y, z);
    vec_printf(v, FMT);
    FREE3(fx, fy, fz);
    bi_cortez_zero_fin(cortez);
}
