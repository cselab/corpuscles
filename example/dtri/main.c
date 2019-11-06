#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <real.h>
#include <co/err.h>
#include <co/vec.h>
#include <co/ten.h>
#include <co/dtri.h>
#include <co/macro.h>
#include <co/util.h>

#define FMT_IN   CO_REAL_IN
static const char **argv;

static void vec(/**/ real a[3]) { vec_argv(&argv, a); }
static int scl(/**/ real *p) {
    if (*argv == NULL) ER("not enough args");
    if (sscanf(*argv, FMT_IN, p) != 1)
        ER("not a number '%s'", *argv);
    argv++;
    return CO_OK;
}
static int eq(const char *a, const char *b) { return util_eq(a, b); }

int main(__UNUSED int argc, const char **v0) {
    const char *op;
    real a[3], b[3], c[3], d[3], da[3], db[3], dc[3];
    real u[3], v[3], w[3], du[3], dv[3], dw[3];
    Ten Da, Db, Dc;
    real Ka, a3, a4, mu, b1, b2;

    argv = v0;
    argv++;
    if (*argv == NULL) ER("mssing OP");

    op = *argv++;
    if (eq(op, "angle")) {
        vec(a); vec(b); vec(c);
        dtri_angle(a, b, c, da, db, dc);
        vec_printf(da, "%g");
        vec_printf(db, "%g");
        vec_printf(dc, "%g");
    } else if (eq(op, "cot")) {
        vec(a); vec(b); vec(c);
        dtri_cot(a, b, c, da, db, dc);
        vec_printf(da, "%g");
        vec_printf(db, "%g");
        vec_printf(dc, "%g");
    } else if (eq(op, "area")) {
        vec(a); vec(b); vec(c);
        dtri_area(a, b, c, da, db, dc);
        vec_printf(da, "%g");
        vec_printf(db, "%g");
        vec_printf(dc, "%g");
    } else if (eq(op, "volume")) {
        vec(a); vec(b); vec(c);
        dtri_volume(a, b, c, da, db, dc);
        vec_printf(da, "%g");
        vec_printf(db, "%g");
        vec_printf(dc, "%g");
    } else if (eq(op, "normal")) {
        vec(a); vec(b); vec(c);
        dtri_normal(a, b, c, &Da, &Db, &Dc);
        ten_printf(&Da, "%g");
        ten_printf(&Db, "%g");
        ten_printf(&Dc, "%g");
    } else if (eq(op, "edg_area")) {
        vec(a); vec(b); vec(c);
        dtri_edg_area(a, b, c, da, db, dc);
        vec_printf(da, "%g");
        vec_printf(db, "%g");
        vec_printf(dc, "%g");
    } else if (eq(op, "alpha")) {
        vec(a); vec(b); vec(c);
        vec(u); vec(v); vec(w);
        dtri_alpha(a, b, c, u, v, w, /**/ du, dv, dw);
        vec_printf(du, "%g");
        vec_printf(dv, "%g");
        vec_printf(dw, "%g");
    } else if (eq(op, "beta")) {
        vec(a); vec(b); vec(c);
        vec(u); vec(v); vec(w);
        dtri_beta(a, b, c, u, v, w, /**/ du, dv, dw);
        vec_printf(du, "%g");
        vec_printf(dv, "%g");
        vec_printf(dw, "%g");
    } else if (eq(op, "lim_area")) {
        scl(&Ka); scl(&a3); scl(&a4);
        vec(a); vec(b); vec(c);
        vec(u); vec(v); vec(w);
        dtri_lim_area(Ka, a3, a4, a, b, c, u, v, w, du, dv, dw);
        vec_printf(du, "%g"); vec_printf(dv, "%g"); vec_printf(dw, "%g");
    } else if (eq(op, "lim_shear")) {
        scl(&mu); scl(&b1); scl(&b2);
        vec(a); vec(b); vec(c);
        vec(u); vec(v); vec(w);
        dtri_lim_shear(mu, b1, b2, a, b, c, u, v, w, du, dv, dw);
        vec_printf(du, "%g"); vec_printf(dv, "%g"); vec_printf(dw, "%g");
    } else if (eq(op, "lim")) {
        scl(&Ka); scl(&a3); scl(&a4);
        scl(&mu); scl(&b1); scl(&b2);
        vec(a); vec(b); vec(c);
        vec(u); vec(v); vec(w);
        dtri_lim(Ka, a3, a4, mu, b1, b2, a, b, c, u, v, w, du, dv, dw);
        vec_printf(du, "%g"); vec_printf(dv, "%g"); vec_printf(dw, "%g");
    } else
        ER("unknown operation '%s'", op);
    return 0;
}
