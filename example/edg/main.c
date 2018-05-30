#include <stdio.h>

#include <he/err.h>
#include <he/edg.h>
#include <he/util.h>

static HeEdg *edg;

static void ini(int n) { he_edg_ini(n, &edg); }
static void fin() { he_edg_fin(edg); }
static int eq(const char *a, const char *b) { return util_eq(a, b); }
static void set(int i, int j, int v) {
    printf("set %d %d %d\n", i, j, v);
    he_edg_set(edg, i, j, v);
}
static void get(int i, int j) {
    int v;
    v = he_edg_get(edg, i, j);
    printf("get %d %d %d\n", i, j, v);
}

int main() {
    const char *path = "/dev/stdin";
    FILE *f;
    char cmd[2048], line[2048];
    int i, j, v, c, n;
    f = fopen(path, "r");
    if (util_fgets(line, f) == NULL)
        ER("fail to read file '%s'", path);
    if (sscanf(line, "%d", &n) != 1)
        ER("wrong first line: '%s'", line);
    ini(n);
    while (util_fgets(line, f) != NULL) {
        c = sscanf(line, "%s %d %d %d", cmd, &i, &j, &v);
        if (eq(cmd, "set") && c == 4) set(i, j, v);
        else if (eq(cmd, "get") && c == 3) get(i, j);
        else ER("wrong line: '%s'", line);
    }
    fclose(f);
    fin();
}
