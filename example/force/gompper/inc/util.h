static void get3(int i, int j, int k, /**/
          real a[3], real b[3], real c[3]) {
    vec_get(i, XX, YY, ZZ, a);
    vec_get(j, XX, YY, ZZ, b);
    vec_get(k, XX, YY, ZZ, c);
}

static void get_edg(int i, int j, real r[3]) {
    real a[3], b[3];
    vec_get(i, XX, YY, ZZ, a);
    vec_get(j, XX, YY, ZZ, b);
    vec_minus(a, b, r);
}

static real sum(int n, real *a) {
    int i;
    real s;
    for (i = 0, s = 0; i < n; i++) s += a[i];
    return s;
}

static real sum_sq(int n, real *a) {
    int i;
    real s;
    for (i = 0, s = 0; i < n; i++) s += a[i]*a[i];
    return s;
}

static void vabs(int n, real *x, real *y, real *z, /**/ real *r) {
    int i;
    for (i = 0; i < n; i++)
        r[i] = sqrt(x[i]*x[i] + y[i]*y[i] + z[i]*z[i]);
}
