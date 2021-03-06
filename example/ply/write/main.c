#include <stdio.h>
#include <stdlib.h>

#include <real.h>
#include <co/macro.h>
#include <co/err.h>
#include <co/he.h>
#include <co/ply.h>
#include <co/memory.h>
#include <co/util.h>

static const char **argv;
static Ply *read;
static int nm, m;
static const char *me = "ply/read";
static real *x, *y, *z;
static He *he;

static void usg(void) {
    fprintf(stderr, "%s [CELL INDEX] < OFF\n", me);
    exit(2);
}

static int num(/**/ int *p) {
    if (*argv == NULL) ER("not enough args");
    if (sscanf(*argv, "%d", p) != 1)
        ER("not an integer '%s'", *argv);
    argv++;
    return CO_OK;
}

static void ini() {
    ply_fread(stdin, &read);
}
static void fin() { ply_fin(read); }

static int write() {
    int i, *ban;
    nm = ply_nm(read);
    MALLOC(nm, &ban);
    for (i = 0; i < nm; i++)
        ban[i] = (i == m);
    ply_fwrite(read, stdout, ban);
    FREE(ban);
    return CO_OK;
}

int main(__UNUSED int c, const char **v) {
    if (*++v != NULL && util_eq(*v, "-h")) usg();
    argv = v;
    num(&m);
    ini();
    write();
    fin();
}
