#include <stdio.h>
#include <stdlib.h>

#include <real.h>
#include <he/macro.h>
#include <he/err.h>
#include <he/he.h>
#include <he/ply.h>
#include <he/memory.h>
#include <he/util.h>

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
    return HE_OK;
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
    return HE_OK;
}

int main(__UNUSED int c, const char **v) {
    if (*++v != NULL && util_eq(*v, "-h")) usg();
    argv = v;
    num(&m);
    ini();
    write();
    fin();
}
