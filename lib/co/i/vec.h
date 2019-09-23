static real
i_vec_dot(const real a[3], const real b[3])
{
    enum {X, Y, Z};
   return a[X]*b[X] + a[Y]*b[Y] + a[Z]*b[Z];
}

static real
i_vec_abs(const real a[3]) {
    return sqrt(vec_dot(a, a));
}
