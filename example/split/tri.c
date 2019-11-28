#include <stdio.h>
#include <stdlib.h>
#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/he/invariant.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/off.h>
#include <co/vec.h>
#include <co/tri.h>
#include <co/y.h>

const char *me = "split/tri";
static void
usg()
{
    fprintf(stderr, "%s: [tirangle number] < OFF\n", me);
    exit(2);
}

int
main(int __UNUSED argc, const char **argv)
{
    He *he;
    real *x, *y, *z, *color;
    real a[3], b[3], c[3];
    real u[3], v[3], w[3];
    const char *arg;
    int t, i, j, k, nt, nv, Tflag, status;

    Tflag = 0;
    while (*++argv != NULL && argv[0][0] == '-')
	switch (argv[0][1]) {
	case 'h':
	    usg();
	    break;
	case 't':
	    argv++;
	    if ((arg = *argv) == NULL) {
		fprintf(stderr, "%s: -t needs an argument\n", me);
		exit(2);
	    }
	    Tflag = 1;
	    t = atoi(*argv);
	    break;
	default:
	    fprintf(stderr, "%s: unknown option '%s'\n", me, argv[0]);
	    exit(2);
	}
    y_inif(stdin, &he, &x, &y, &z);
    nt = he_nt(he);
    if (!Tflag) {
	fprintf(stderr, "%s: triangle (-t) is not set\n", me);
	exit(2);
    }
    if (t >= nt) {
	fprintf(stderr, "%s: t=%d >= nt=%d\n", me, t, nt);
	exit(2);
    }
    MSG("he_nv: %d", he_nv(he));
    MSG("he_nt: %d", he_nt(he));
    he_tri_ijk(he, t, &i, &j, &k);

    int h, n, nn;
    h = he_hdg_tri(he, t);
    n = he_nxt(he, h);
    nn = he_nxt(he, n);
    h = he_tri(he, he_flp(he, h));
    n = he_tri(he, he_flp(he, n));
    nn = he_tri(he, he_flp(he, nn));

    vec_get3(i, j, k, x, y, z, a, b, c);

    status = he_tri_split(he, t);
    if (status != CO_OK)
	ER("he_tri_split failed");
    status = he_invariant(he);
    if (status != CO_OK)
      ER("he_invariant failed");
    nv = he_nv(he);
    nt = he_nt(he);
    REALLOC(nv, &x);
    REALLOC(nv, &y);
    REALLOC(nv, &z);
    CALLOC(nt, &color);
    for (i = nt - 6; i < nt; i++)
      color[i] = 1;
    MSG("fnnn: %d %d %d", h, n, nn);
    color[t] = color[h] = color[n] = color[nn] = 1;
    tri_edg_center(a, b, c, u, v, w);
    vec_fprintf(a, stderr, "%g");
    vec_fprintf(b, stderr, "%g");
    vec_fprintf(c, stderr, "%g");
    //vec_zero(u);
    //vec_zero(v);
    vec_set(v, nv - 1, x, y, z);
    vec_set(u, nv - 2, x, y, z);
    vec_set(w, nv - 3, x, y, z);
    MSG("nv: %d", nv);
    MSG("nt: %d", nt);
    boff_tri_fwrite(he, x, y, z, color, stdout);
    FREE(color);
    y_fin(he, x, y, z);
}
