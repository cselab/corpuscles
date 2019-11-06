#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <real.h>
#include <co/err.h>
#include <co/vec.h>
#include <co/macro.h>
#include <co/util.h>
#include <co/strain/2d.h>

#include <co/dih.h>
#include <co/ddih.h>

#define FMT_IN   CO_REAL_IN
static const char **argv;
static const real h = 1e-6;
static real a[2], b[2], c[2];
static real v[2], u[2], w[2];

enum {X, Y, Z};

static real  F(__UNUSED void *param, real I1, __UNUSED real I2)          { return I1; }
static real F1(__UNUSED void *param, __UNUSED real I1, __UNUSED real I2) { return 1;  }
static real F2(__UNUSED void *param, __UNUSED real I1, __UNUSED real I2) { return 0;  }

static int print2(const real a[2], const real u[2], const real p[2], const real q[2]) {
    enum {X, Y};
    printf("%.16g %.16g %.16g %.16g %.16g %.16g\n", a[X] + u[X], a[Y] + u[Y], p[X], p[Y], q[X], q[Y]);
    return CO_OK;
}

static real energy() {
    real I1, I2;
    strain_2d(NULL, F1, F2,
                       a[X], a[Y], b[X], b[Y], c[X], c[Y],
                       v[X], v[Y], u[X], u[Y], w[X], w[Y], /**/
                       NULL, NULL, NULL, NULL, NULL, NULL,
                       &I1, &I2, NULL);
    return F(NULL, I1, I2);
}

static real denergy() {
    real I1, I2, area;
    strain_2d(NULL, F1, F2,
                       a[X], a[Y], b[X], b[Y], c[X], c[Y],
                       v[X], v[Y], u[X], u[Y], w[X], w[Y], /**/
                       NULL, NULL, NULL, NULL, NULL, NULL,
                       &I1, &I2, &area);
    return F(NULL, I1, I2)*area;
}


static int scl(/**/ real *p) {
    if (*argv == NULL) ER("not enough args");
    if (sscanf(*argv, FMT_IN, p) != 1)
        ER("not a number '%s'", *argv);
    argv++;
    return CO_OK;
}

static int vec2(/**/ real v[2]) {
    scl(&v[X]);
    scl(&v[Y]);
    return CO_OK;
}

static real fd0(real *p) {
    real e, ep, em, v, t;
    v = *p;
    *p += h; ep  = energy(); *p = v;
    *p -= h; em  = energy(); *p = v;
    return (ep - em)/(2*h);
}

static int fd(real dv[2], real du[2], real dw[2]) {
    enum {X, Y};
    dv[X] = fd0(&v[X]); dv[Y] = fd0(&v[Y]);
    du[X] = fd0(&u[X]); du[Y] = fd0(&u[Y]);
    dw[X] = fd0(&w[X]); dw[Y] = fd0(&w[Y]);
    return CO_OK;
}

static int eq(const char *a, const char *b) { return util_eq(a, b); }
int main(__UNUSED int argc, const char **argv0) {
    const char *op;
    real da[2], db[2], dc[2];
    real ha[2], hb[2], hc[2];
    real I1, I2, deng, area;
    argv = argv0;
    argv++;
    if (*argv == NULL) ER("mssing OP");

    op = *argv++;
    if (eq(op, "all")) {
        vec2(a); vec2(b); vec2(c);
        vec2(v); vec2(u); vec2(w);
        strain_2d(NULL, F1, F2,
                           a[X], a[Y], b[X], b[Y], c[X], c[Y],
                           v[X], v[Y], u[X], u[Y], w[X], w[Y], /**/
                           &da[X], &da[Y], &db[X], &db[Y], &dc[X], &dc[Y],
                           NULL, NULL, NULL);
        fd(ha, hb, hc);
        puts("x y fx fy hx hy");
        print2(a, v, da, ha);
        print2(b, u, db, hb);
        print2(c, w, dc, hc);
    } else if (eq(op, "energy")) {
        vec2(a); vec2(b); vec2(c);
        vec2(v); vec2(u); vec2(w);
        printf("%.16g\n", denergy());
    } else if (eq(op, "denergy")) {
        vec2(a); vec2(b); vec2(c);
        vec2(v); vec2(u); vec2(w);
        printf("%.16g\n", energy());
    } else if (eq(op, "force")) {
        vec2(a); vec2(b); vec2(c);
        vec2(v); vec2(u); vec2(w);
        strain_2d(NULL, F1, F2,
                           a[X], a[Y], b[X], b[Y], c[X], c[Y],
                           v[X], v[Y], u[X], u[Y], w[X], w[Y], /**/
                           &da[X], &da[Y], &db[X], &db[Y], &dc[X], &dc[Y],
                           NULL, NULL, &area);
        printf("%.16g %.16g %.16g %.16g %.16g %.16g\n",
               da[X]*area, da[Y]*area, db[X]*area, db[Y]*area, dc[X]*area, dc[Y]*area);
    } else if (eq(op, "dforce")) {
        vec2(a); vec2(b); vec2(c);
        vec2(v); vec2(u); vec2(w);
        strain_2d(NULL, F1, F2,
                           a[X], a[Y], b[X], b[Y], c[X], c[Y],
                           v[X], v[Y], u[X], u[Y], w[X], w[Y], /**/
                           &da[X], &da[Y], &db[X], &db[Y], &dc[X], &dc[Y],
                           NULL, NULL, &area);
        printf("%.16g %.16g %.16g %.16g %.16g %.16g\n",
               da[X], da[Y], db[X], db[Y], dc[X], dc[Y]);        
    } else
      ER("unknown operation '%s'", op);
    return 0;
}
