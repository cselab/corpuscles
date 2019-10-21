static real
i_tri_area(const real a[3], const real b[3], const real c[3])
{
    enum { X, Y, Z };
    real bx, by, bz;
    real cx, cy, cz;
    real x, y, z;

    bx = b[X] - a[X];
    by = b[Y] - a[Y];
    bz = b[Z] - a[Z];
    cx = c[X] - a[X];
    cy = c[Y] - a[Y];
    cz = c[Z] - a[Z];
    x = by * cz - bz * cy;
    y = bz * cx - bx * cz;
    z = bx * cy - by * cx;
    return sqrt(x * x + y * y + z * z) / 2;
}

static real
i_tri_angle(const real a[3], const real b[3], const real c[3])
{
    real u[3], v[3];

    i_vec_minus(a, b, u);
    i_vec_minus(c, b, v);
    return fabs(i_vec_angle(u, v));
}

static int
i_tri_normal(const real a[3], const real b[3], const real c[3],
             /**/ real e[3])
{
    enum { X, Y, Z };
    real ax, ay, az, bx, by, bz;
    real u, v, w, n;

    ax = b[X] - a[X];
    ay = b[Y] - a[Y];
    az = b[Z] - a[Z];
    bx = c[X] - a[X];
    by = c[Y] - a[Y];
    bz = c[Z] - a[Z];
    u = ay * bz - by * az;
    v = bx * az - ax * bz;
    w = ax * by - bx * ay;
    n = sqrt(u * u + v * v + w * w);
    e[X] = u / n;
    e[Y] = v / n;
    e[Z] = w / n;
    return CO_OK;
}
