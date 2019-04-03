#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

#include <real.h>
#include <co/err.h>
#include <co/vec.h>
#include <co/tri.h>
#include <co/macro.h>
#include <co/util.h>

#define FMT_IN   CO_REAL_IN
static const char **argv;

static void
vec(/**/ real a[3])
{
	vec_argv(&argv, a);
}

static int
scl(/**/ real *p)
{
	if (*argv == NULL) ER("not enough args");
	if (sscanf(*argv, FMT_IN, p) != 1)
		ER("not a number '%s'", *argv);
	argv++;
	return CO_OK;
}

static int
eq(const char *a, const char *b)
{
	return util_eq(a, b);
}

int
main(__UNUSED int argc, const char **v0)
{
	const char *op;
	real a[3], b[3], c[3], n[3], ex[3], ey[3];
	real u[3], v[3], w[3];
	real al, be;
	real ax, bx, by;
	real ux, vx, vy;
	real Ka, a3, a4, mu, b1, b2;
	real a0, b0, c0;

	argv = v0;
	argv++;
	if (*argv == NULL) ER("mssing OP");

	op = *argv++;
	if (eq(op, "angle")) {
		vec(a);
		vec(b);
		vec(c);
		printf("%.16g\n", tri_angle(a, b, c));
	} else if (eq(op, "area")) {
		vec(a);
		vec(b);
		vec(c);
		printf("%.16g\n", tri_area(a, b, c));
	} else if (eq(op, "cot")) {
		vec(a);
		vec(b);
		vec(c);
		printf("%.16g\n", tri_cot(a, b, c));
	} else if (eq(op, "volume")) {
		vec(a);
		vec(b);
		vec(c);
		printf("%.16g\n", tri_volume(a, b, c));
	} else if (eq(op, "normal")) {
		vec(a);
		vec(b);
		vec(c);
		tri_normal(a, b, c, /**/ n);
		vec_printf(n, "%g");
	} else if (eq(op, "center")) {
		vec(a);
		vec(b);
		vec(c);
		tri_center(a, b, c, /**/ n);
		vec_printf(n, "%g");
	} else if (eq(op, "3to2")) {
		vec(a);
		vec(b);
		vec(c);
		tri_3to2(a, b, c, /**/ &ux, &vx, &vy);
		printf("%.16g %.16g %.16g\n", ux, vx, vy);
	} else if (eq(op, "2to3")) {
		vec(a);
		vec(b);
		vec(c);
		tri_2to3(a, b, c, /**/ ex, ey);
		vec_printf(ex, "%.16g");
		vec_printf(ey, "%.16g");
	} else if (eq(op, "2d_invariants")) {
		scl(&ax);
		scl(&bx);
		scl(&by);
		scl(&ux);
		scl(&vx);
		scl(&vy);
		tri_2d_invariants(ax, bx, by, ux, vx, vy, /**/ &al, &be);
		printf("%g %g\n", al, be);
	} else if (eq(op, "al")) {
		vec(a);
		vec(b);
		vec(c);
		vec(u);
		vec(v);
		vec(w);
		tri_3d_invariants(a, b, c, u, v, w, /**/ &al, NULL);
		printf("%g\n", al);
	} else if (eq(op, "be")) {
		vec(a);
		vec(b);
		vec(c);
		vec(u);
		vec(v);
		vec(w);
		tri_3d_invariants(a, b, c, u, v, w, /**/ NULL, &be);
		printf("%g\n", be);
	} else if (eq(op, "3d_invariants")) {
		vec(a);
		vec(b);
		vec(c);
		vec(u);
		vec(v);
		vec(w);
		tri_3d_invariants(a, b, c, u, v, w, /**/ &al, &be);
		printf("%g %g\n", al, be);
	} else if (eq(op, "edg_area")) {
		vec(a);
		vec(b);
		vec(c);
		printf("%g\n", tri_edg_area(a, b, c));
	} else if (eq(op, "alpha")) {
		vec(a);
		vec(b);
		vec(c);
		vec(u);
		vec(v);
		vec(w);
		printf("%g\n",tri_alpha(a, b, c, u, v, w));
	} else if (eq(op, "beta")) {
		vec(a);
		vec(b);
		vec(c);
		vec(u);
		vec(v);
		vec(w);
		printf("%g\n",tri_beta(a, b, c, u, v, w));
	} else if (eq(op, "lim_area")) {
		scl(&Ka);
		scl(&a3);
		scl(&a4);
		vec(a);
		vec(b);
		vec(c);
		vec(u);
		vec(v);
		vec(w);
		printf("%g\n",tri_lim_area(Ka, a3, a4, a, b, c, u, v, w));
	} else if (eq(op, "lim_shear")) {
		scl(&mu);
		scl(&b1);
		scl(&b2);
		vec(a);
		vec(b);
		vec(c);
		vec(u);
		vec(v);
		vec(w);
		printf("%g\n",tri_lim_shear(mu, b1, b2, a, b, c, u, v, w));
	} else if (eq(op, "lim")) {
		scl(&Ka);
		scl(&a3);
		scl(&a4);
		scl(&mu);
		scl(&b1);
		scl(&b2);
		vec(a);
		vec(b);
		vec(c);
		vec(u);
		vec(v);
		vec(w);
		printf("%g\n",tri_lim(Ka, a3, a4, mu, b1, b2, a, b, c, u, v, w));
	} else if (eq(op, "abc")) {
		vec(a);
		vec(b);
		vec(c);
		vec(u);
		vec(v);
		vec(w);
		tri_abc(a, b, c, u, v, w, &a0, &b0, &c0);
		printf("%g %g %g\n", a0, b0, c0);
	} else
		ER("unknown operation '%s'", op);
	return 0;
}
