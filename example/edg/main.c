#include <stdio.h>

#include <he/err.h>
#include <he/edg.h>

static HeEdg *edg;

static void ini(int n) { he_edg_ini(n, &edg); }
static void fin() { he_edg_fin(edg); }

int main() {
    int n;
    ini(n = 10);
    fin();
}
