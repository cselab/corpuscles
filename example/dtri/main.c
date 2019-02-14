#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

#include <real.h>
#include <he/err.h>
#include <he/vec.h>
#include <he/ten.h>
#include <he/dtri.h>
#include <he/macro.h>
#include <he/util.h>

static const char **argv;

void vec(/**/ real a[3]) { vec_argv(&argv, a); }

int eq(const char *a, const char *b) { return util_eq(a, b); }
int main(__UNUSED int argc, const char **v0) {
    const char *op;
    real a[3], b[3], c[3], d[3], da[3], db[3], dc[3];
    real u[3], v[3], w[3], du[3], dv[3], dw[3];
    Ten Da, Db, Dc;
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
    } else if (eq(op, "alpha")) {
        vec(a); vec(b); vec(c);
        vec(u); vec(v); vec(w);
        dtri_alpha(a, b, c, u, v, w, /**/ du, dv, dw);
        vec_printf(du, "%g");
        vec_printf(dv, "%g");
        vec_printf(dw, "%g");
    } else
        ER("unknown operation '%s'", op);
    return 0;
}
