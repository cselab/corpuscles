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
static HeOff *off;
static int nt;
static real *x, *y, *z;
static He *he;
static real lo, hi, *a;

static int q_x(void) {
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

int main(__UNUSED int c, char **v) {
    char q[1024];
    int n, i;

    argv = v;
    argv++;
    if (!argv[0] || util_eq(argv[0], "-h"))
        usg();
    argv_str(&argv, q);

    n = sizeof(Name)/sizeof(Name[0]);
    for (i = 0; i < n; i++) {
        if (util_eq(q, Name[i]))
            return Func[i]();
    }

    MSG("unknown query: '%s'", q);
    eputs("possible values are:");
    for (i = 0; i < n; i++)
        eputs(Name[i]);
    return 1;
}
