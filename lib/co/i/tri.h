static int
i_tri_area(const real a[3], const real b[3], const real c[3])
{
    enum {X, Y, Z};
    real bx, by, bz;
    real cx, cy, cz;

    bx = b[X] - a[X];
    by = b[Y] - a[Y];
    bz = b[Z] - a[Z];
    cx = c[X] - a[X];
    cy = c[Y] - a[Y];
    cz = c[Z] - a[Z];
    x = by*cz-bz*cy;
    y = bz*cx-bx*cz;
    z = bx*cy-by*cx;
    return sqrt(x*x + y*y + z*z);
}
