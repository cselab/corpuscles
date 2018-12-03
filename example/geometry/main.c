#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>
#include <string.h>

#include <real.h>
#include <he/bending.h>
#include <he/punto.h>
#include <he/he.h>
#include <he/off.h>
#include <he/memory.h>
#include <he/err.h>
#include <he/util.h>
#include <he/macro.h>
#include <he/vec.h>

#define FMT_IN   XE_REAL_IN

static const char **argv;
static char name[4048];

static real *fx, *fy, *fz, *xx, *yy, *zz, *rr, *eng, *H, *area;
static int nv, nt;
static He *he;
static Bending *bending;
static BendingParam param;
static const char *me = "bending";

static void usg() {
    const char *list;
    list = bending_list();
    fprintf(stderr, "%s %s < OFF \n", me, list);
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
    return HE_OK;
}
static int str(/**/ char *p) {
    if (*argv == NULL) {
        usg();
        ER("not enough args");
    }
    strncpy(p, *argv, 4048);
    argv++;
    return HE_OK;
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
  static HeOff *off;
  argv = v; argv++;
  arg();
  
  off_ini(path, &off);
  
  nv = off_nv(off);
  nt = off_nt(off);
  off_tri(off, &tri);
  he_tri_ini(nv, nt, tri, &he);
  
  MALLOC(nv, &xx); MALLOC(nv, &yy); MALLOC(nv, &zz);
  CALLOC(nv, &fx); CALLOC(nv, &fy); CALLOC(nv, &fz);
  CALLOC(nv, &H); 
  CALLOC(nv, &area); 
  
  off_xyz(off, xx, yy, zz);
  
  bending_ini(name, param, he,  &bending);
  e = bending_energy(bending, he, xx, yy, zz);
  
  bending_curva_mean_ver(bending, &H);
  bending_area_ver(bending, &area);

  H0=0;
  H1=0;
  H2=0;
  for (i=0; i<nv; i++) {
    //printf("H[%i]=%g\n", i, H[i]);
    H0+=area[i];
    H1+=H[i]*area[i];
    H2+=H[i]*H[i]*area[i];
  }
  //printf("#H0 H1 H2\n");
  printf("%f %f %f\n", H0, H1, H2);
  
  
  FREE(xx); FREE(yy); FREE(zz);
  FREE(fx); FREE(fy); FREE(fz);
  FREE(H);
  FREE(area);
  
  off_fin(off);
  he_fin(he);
}
