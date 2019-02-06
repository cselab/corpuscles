#include <stdio.h>
#include <stdlib.h>

#include <real.h>
#include <he/argv.h>
#include <he/area.h>
#include <he/array.h>
#include <he/bending.h>
#include <he/err.h>
#include <he/off.h>
#include <he/he.h>
#include <he/macro.h>
#include <he/memory.h>
#include <he/util.h>

#include <he/y.h>

#define FMT   HE_REAL_OUT

static const char *me = "he.q";
static void usg(void) {
    fprintf(stderr, "%s query [ARGS..] < IN.off > OUT.off\n", me);
    fputs("color off file\n", stderr);
    exit(2);
}

static char **argv;
static real *x, *y, *z, *area;
static int nv, nt;
static He *he;
static int Lim; /* are limits set? */
static real lo, hi;

static int divide(int n, const real *a, const real *b, /**/ real *c) {
    int i;
    for (i = 0; i < n; i ++)
        c[i] = a[i] / b[i];
    return HE_OK;
}

static int ver(const real *a) {
    if (Lim)
        return boff_lh_ver_fwrite(he, x, y, z, lo, hi, a, stdout);
    else {
        fprintf(stderr, FMT " " FMT "\n",
                array_min(nv, a), array_max(nv, a));
        return boff_ver_fwrite(he, x, y, z, a, stdout);
    }
}

static int tri(const real *a) {
    if (Lim)
        return boff_lh_tri_fwrite(he, x, y, z, lo, hi, a, stdout);
    else {
        fprintf(stderr, FMT " " FMT "\n",
                array_min(nt, a), array_max(nt, a));
        return boff_tri_fwrite(he, x, y, z, a, stdout);
    }
}

static int q_x(void) { return ver(x); }
static int q_y(void) { return ver(y); }
static int q_z(void) { return ver(z); }

static int q_area(void) {
    real *a;
    MALLOC(nt, &a);
    he_area_tri(he, x, y, z, a);
    tri(a);
    FREE(a);
    return HE_OK;
}

static int q_bending(void) {
    Bending *b;
    BendingParam p;
    real *eng, *deng;

    MALLOC(nv, &deng);
    p.Kb = 1;
    p.C0 = p.Kad = p.DA0D = 0;
    
    bending_ini("juelicher_xin", p, he, &b);
    bending_energy(b, he, x, y, z);
    bending_energy_ver(b, &eng);
    he_area_ver(he, x, y, z, area);

    divide(nv, eng, area, deng);
    
    ver(deng);
    
    bending_fin(b);
    FREE(deng);
}

static const char *Name[] = {"x", "y", "z", "area", "bending"};
static int (*Func[])(void) = {q_x, q_y, q_z, q_area, q_bending};

int eputs(const char *s) {
    fputs(s, stderr);
    fputs("\n", stderr);
    return HE_OK;
}

static int nxt(const char *b) {
    return (argv[0] != NULL) && util_eq(argv[0], b);
};

int main(__UNUSED int c, char **v) {
    char q[1024];
    int n, i;

    argv = v;
    argv++;
    if (nxt("-h")) usg();

    if (nxt("-l")) {
        Lim = 1;
        argv++; argv_real(&argv, &lo);
        if (!nxt("-h"))
            ER("expecting -h, got '%s'", argv[0] ? argv[0] : "null");
        argv++; argv_real(&argv, &hi);
    }

    argv_str(&argv, q);
    y_ini("/dev/stdin", &he, &x, &y, &z);
    nv = he_nv(he);
    nt = he_nt(he);
    MALLOC(nv, &area);
    he_area_ver(he, x, y, z, area);

    n = sizeof(Name)/sizeof(Name[0]);
    for (i = 0; i < n; i++) {
        if (util_eq(q, Name[i])) {
            Func[i]();
            goto ok;
        }
    }

    MSG("unknown query: '%s'", q);
    eputs("possible values are:");
    for (i = 0; i < n; i++)
        eputs(Name[i]);
    return 1;
ok:
    FREE(area);
    y_fin(he, x, y, z);
    return 0;
}
