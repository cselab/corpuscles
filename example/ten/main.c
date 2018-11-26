#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <real.h>
#include <he/err.h>
#include <he/vec.h>
#include <he/ten.h>
#include <he/macro.h>
#include <he/util.h>

#define FMT_IN   XE_REAL_IN
static const char **argv;

void vec(/**/ real a[3]) { vec_argv(&argv, a); }
void ten(/**/ Ten *T) { ten_argv(&argv, T); }
int scl(/**/ real *p) {
    if (*argv == NULL) ER("not enough args");
    if (sscanf(*argv, FMT_IN, p) != 1)
        ER("not a number '%s'", *argv);
    argv++;
    return HE_OK;
}

int eq(const char *a, const char *b) { return util_eq(a, b); }
int main(__UNUSED int argc, const char **v) {
    const char *op;
    real s, a[3], b[3], c[3];
    Ten T, R, P;
    argv = v;
    argv++;
    if (*argv == NULL) ER("mssing OP");

    op = *argv++;
    if (eq(op, "row_ini")) {
        vec(a); vec(b); vec(c);
        ten_row_ini(a, b, c, &T);
        ten_printf(&T, "%g");
    } else if (eq(op, "col_ini")) {
        vec(a); vec(b); vec(c);
        ten_col_ini(a, b, c, &T);
        ten_printf(&T, "%g");
    } else if (eq(op, "dyadic")) {
        vec(a);
        vec(b);
        ten_dyadic(a, b, &T);
        ten_printf(&T, "%g");
    } else if (eq(op, "plus")) {
        ten(&T); ten(&R);
        ten_plus(&T, &R, &P);
        ten_printf(&P, "%g");
    } else if (eq(op, "minus")) {
        ten(&T); ten(&R);
        ten_minus(&T, &R, &P);
        ten_printf(&P, "%g");
    } else if (eq(op, "scalar")) {
        ten(&T); scl(&s);
        ten_scalar(&T, s, &R);
        ten_printf(&R, "%g");
    } else if (eq(op, "vec")) {
        ten(&T); vec(a);
        ten_vec(&T, a, b);
        vec_printf(b, "%g");
    } else if (eq(op, "vec_ten")) {
        vec(a); ten(&T);
        vec_ten(a, &T, b);
        vec_printf(b, "%g");
    } else if (eq(op, "add")) {
        ten(&T); ten(&R);
        ten_add(&R, &T);
        ten_printf(&T, "%g");
    } else if (eq(op, "sub")) {
        ten(&T); ten(&R);
        ten_sub(&R, &T);
        ten_printf(&T, "%g");
    } else if (eq(op, "mult_right")) {
        ten(&T); ten(&R);
        ten_mult_right(&R, &T);
        ten_printf(&T, "%g");
    } else if (eq(op, "mult_left")) {
        ten(&T); ten(&R);
        ten_mult_left(&R, &T);
        ten_printf(&T, "%g");
    } else if (eq(op, "trace")) {
        ten(&T);
        printf("%g\n", ten_trace(&T));
    } else if (eq(op, "determinant")) {
        ten(&T);
        printf("%g\n", ten_determinant(&T));
    } else if (eq(op, "scale")) {
        scl(&s); ten(&T);
        ten_scale(s, &T);
        ten_printf(&T, "%g");
    } else if (eq(op, "axpy")) {
        scl(&s); ten(&P); ten(&T);
        ten_axpy(s, &P, &T);
        ten_printf(&T, "%g");
    } else if (eq(op, "copy")) {
        ten(&T);
        ten_copy(&T, &P);
        ten_printf(&P, "%g");
    } else if (eq(op, "zero")) {
        ten_zero(&T);
        ten_printf(&T, "%g");
    } else if (eq(op, "one")) {
        ten_one(&T);
        ten_printf(&T, "%g");
    } else
        ER("unknown operation '%s'", op);
    return 0;
}
