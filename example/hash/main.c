#include <stdio.h>

#include <he/err.h>
#include <he/hash.h>
#include <he/util.h>

static HeHash *hash;

static void ini(int n) { he_hash_ini(n, &hash); }
static void fin() { he_hash_fin(hash); }
static int eq(const char *a, const char *b) { return util_eq(a, b); }
static void set(int i, int j, int v) {
    printf("set %d %d %d\n", i, j, v);
    he_hash_set(hash, i, j, v);
}
static void get(int i, int j) {
    int v;
    v = he_hash_get(hash, i, j);
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
