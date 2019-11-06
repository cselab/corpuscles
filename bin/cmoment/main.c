#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <real.h>
#include <co/bending.h>
#include <co/punto.h>
#include <co/he.h>
#include <co/memory.h>
#include <co/err.h>
#include <co/util.h>
#include <co/macro.h>
#include <co/vec.h>
#include <co/y.h>

#define FMT_IN   CO_REAL_IN

static const char **argv;
static char name[4048];

static real *fx, *fy, *fz, *xx, *yy, *zz, *rr, *eng, *H, *area;
static int nv, nt;
static He *he;
static Bending *bending;
static BendingParam param;
static const char *me = "co.cmoment";

static void usg() {
    const char *list;
    list = bending_list();
    fprintf(stderr, "%s %s < OFF \n", me, list);
    fprintf(stderr, "print bending moments:\n");
    fprintf(stderr, "sum(a) sum(H*a) sum(H^a)\n");
    fprintf(stderr, "where `a' is a vertice area, `H' is a mean curvature\n");
    exit(2);
}

static int eq(const char *a, const char *b) { return util_eq(a, b); }
static int scl(/**/ real *p) {
    if (*argv == NULL) {
        usg();
        ER("not enough args");
    }
    if (sscanf(*argv, FMT_IN, p) != 1)
        ER("not a number '%s'", *argv);
    argv++;
    return CO_OK;
}
static int str(/**/ char *p) {
    if (*argv == NULL) {
        usg();
        ER("not enough args");
    }
    strncpy(p, *argv, 4048);
    argv++;
    return CO_OK;
}
static void arg() {
    if (*argv != NULL && eq(*argv, "-h")) {
        usg();
        exit(0);
    }
    str(name);
}

int main(int __UNUSED argc, const char *v[]) {

  int *tri;
  int i;
  real e;
  real H0, H1, H2;
  const char path[] = "/dev/stdin";
  argv = v; argv++;
  arg();

  y_ini(path, &he, &xx, &yy, &zz);
  
  nv = he_nv(he);
  CALLOC(nv, &fx); CALLOC(nv, &fy); CALLOC(nv, &fz);
  CALLOC(nv, &H); 
  CALLOC(nv, &area); 
  
  bending_ini(name, param, he,  &bending);
  e = bending_energy(bending, he, xx, yy, zz);
  
  bending_curva_mean_ver(bending, &H);
  bending_area_ver(bending, &area);

  H0 = H1 = H2 = 0;
  for (i = 0; i < nv; i++) {
    H0 += area[i];
    H1 += H[i]*area[i];
    H2 += H[i]*H[i]*area[i];
  }
  printf("%.16g %.16g %.16g\n", H0, H1, H2);
  
  FREE(fx); FREE(fy); FREE(fz);
  FREE(H);
  FREE(area);

  y_fin(he, xx, yy, zz);
}
