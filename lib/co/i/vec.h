static real
i_vec_dot(const real a[3], const real b[3])
{
    enum { X, Y, Z };

    return a[X] * b[X] + a[Y] * b[Y] + a[Z] * b[Z];
}

static real
i_vec_abs(const real a[3])
{
    return sqrt(i_vec_dot(a, a));
}

static int
i_vec_get(int i, const real x[], const real y[], const real z[],
          /**/ real a[3])
{
    enum { X, Y, Z };

    a[X] = x[i];
    a[Y] = y[i];
    a[Z] = z[i];
    return CO_OK;
}

static int
i_vec_get3(int i, int j, int k, const real x[], const real y[],
           const real z[], /**/ real a[3], real b[3], real c[3])
{
    i_vec_get(i, x, y, z, /**/ a);
    i_vec_get(j, x, y, z, /**/ b);
    i_vec_get(k, x, y, z, /**/ c);
    return CO_OK;
}

static int
i_vec_minus(const real a[3], const real b[3], /**/ real c[3])
{
    enum { X, Y, Z };

    c[X] = a[X] - b[X];
    c[Y] = a[Y] - b[Y];
    c[Z] = a[Z] - b[Z];
    return CO_OK;
}

static int
i_vec_plus(const real a[3], const real b[3], /**/ real c[3])
{
    enum { X, Y, Z };

    c[X] = a[X] + b[X];
    c[Y] = a[Y] + b[Y];
    c[Z] = a[Z] + b[Z];
    return CO_OK;
}

static real
i_vec_angle(const real a[3], const real b[3])
{
    enum { X, Y, Z };
    real u, v, w;
    real x, y;

    u = a[Y] * b[Z] - b[Y] * a[Z];
    v = b[X] * a[Z] - a[X] * b[Z];
    w = a[X] * b[Y] - b[X] * a[Y];
    y = sqrt(u * u + v * v + w * w);
    x = a[X] * b[X] + a[Y] * b[Y] + a[Z] * b[Z];
    return atan2(y, x);
}
