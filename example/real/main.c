#include <stdio.h>

#include <real.h>
#include <co/err.h>
#include <co/real.h>

const int n = 5;
Real *he_real;

#define FMT CO_REAL_OUT

int main() {
    int i;
    real r[n], *r0;
    double d[n], *d0;
    for (i = 0; i < n; i++) {
        r[i] = i;
        d[i] = 10*i;
    }

    real_ini(n, &he_real);
    r0 = real_from(he_real, d);
    d0 = real_to(he_real, r);

    for (i = 0; i < n; i++)
        printf(FMT " %g\n", r0[i], d0[i]);

    real_fin(he_real);
}
