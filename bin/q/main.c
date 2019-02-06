#include <stdio.h>
#include <stdlib.h>

#include <real.h>
#include <he/argv.h>
#include <he/area.h>
#include <he/err.h>
#include <he/off.h>
#include <he/he.h>
#include <he/macro.h>
#include <he/memory.h>
#include <he/util.h>

#include <he/y.h>

static const char *me = "he.q";
static void usg(void) {
    fprintf(stderr, "%s query [ARGS..] < IN.off > OUT.off\n", me);
    fputs("color off file\n", stderr);
    exit(2);
}

static char **argv;
static real *x, *y, *z;
static He *he;
static int Lim; /* are limits set? */
static real lo, hi;

static int q_x(void) {
    boff_ver_fwrite(he, x, y, z, z, stdout);
    return HE_OK;
}

static int q_area(void) {
    return HE_OK;
}

static const char *Name[] = {"x", "area"};
static int (*Func[])(void) = {q_x, q_area};

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
    y_fin(he, x, y, z);
    return 0;
}
