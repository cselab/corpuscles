#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <real.h>
#include <co/err.h>
#include <co/edg.h>
#include <co/vec.h>
#include <co/tri.h>
#include <co/macro.h>

#define FMT CO_REAL_OUT
enum {
    X, Y, Z
};
static const char **argv;

static void
vec( /**/ real a[3])
{
    vec_argv(&argv, a);
}

static int
closest(const real a[3], const real b[3], const real c[3], const real p[3],
        real ans[3])
{
    int n, i, j;
    real u[3], v[3], q[3], d, t1, t2, m;

    n = 20000;
    m = 1e42;

    vec_minus(b, a, u);
    vec_minus(c, a, v);

    for (i = 0; i < n; i++) {
        t1 = (i + 0.0) / (n - 1);
        for (j = 0; j < n; j++) {
            t2 = (1 - t1) * j / (n - 1);
            q[X] = a[X] + t1 * u[X] + t2 * v[X];
            q[Y] = a[Y] + t1 * u[Y] + t2 * v[Y];
            q[Z] = a[Z] + t1 * u[Z] + t2 * v[Z];
            d = edg_sq(p, q);
            if (d < m) {
                m = d;
                vec_copy(q, ans);
            }
        }
    }
    return CO_OK;
}


int
main(__UNUSED int argc, const char **v0)
{
    real a[3], b[3], c[3];
    real p[3], q[3], r[3];
    real d, e, eps;

    eps = 1e-4;

    argv = v0;
    argv++;
    vec(a);
    vec(b);
    vec(c);
    vec(p);
    tri_point_closest(a, b, c, p, q);
    closest(a, b, c, p, r);
    d = tri_point_distance(a, b, c, p);
    e = edg_abs(p, q);

    if (fabs(d - e) > eps)
        ER("d = " FMT " e = " FMT, d, e);

    if (edg_abs(q, r) > eps) {
        vec_fprintf(q, stderr, FMT);
        vec_fprintf(r, stderr, FMT);
        ER("edg_abs > eps");
    }

    vec_fprintf(q, stderr, FMT);
    vec_fprintf(r, stderr, FMT);

    /*
       fputs("LIST\n", stdout);
       fputs("{\n", stdout);
       tri_off(a, b, c, stdout);
       edg_vect(p, q, stdout);
       edg_vect(p, r, stdout);
       fputs("}\n", stdout);
     */

    return 0;
}
