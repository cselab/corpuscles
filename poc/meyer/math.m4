
function tri_cot(a, b, c) {
    y = 2 * tri_area(a, b, c)
    vec_minus(a, b,  u)
    vec_minus(c, b,  v)
    x = vec_dot(u, v)
    return x/y
}

function tri_area(a, b, c,   ab, ac, n) {
    vec_minus(b, a, ab)
    vec_minus(c, a, ac)
    vec_cross(ab, ac,   n)
    return vec_abs(n)/2
}

function vec_cross(a, b, c) {
    c[X] = a[Y]*b[Z]-b[Y]*a[Z]
    c[Y] = b[X]*a[Z]-a[X]*b[Z]
    c[Z] = a[X]*b[Y]-b[X]*a[Y]
}

function vec_abs(a) {
    return sqrt(a[X]*a[X] + a[Y]*a[Y] + a[Z]*a[Z])
}

function vec_minus(a, b,  ab) {
    ab[X] = a[X] - b[X]
    ab[Y] = a[Y] - b[Y]
    ab[Z] = a[Z] - b[Z]
}

function vec_dot(a, b) {
    return a[X]*b[X] + a[Y]*b[Y] + a[Z]*b[Z]
}

function vec_dot_i(i, a, b,   a0, b0) {
    vec_get(i, a, a0)
    vec_get(i, b, b0)
    return vec_dot(a0, b0)
}

function edg_sq(a, b,   u) {
    vec_minus(b, a, u)
    return vec_dot(u, u)
}

function tri_angle(a, b, c,   ba, bc) { # at "b"
    vec_minus(a, b, ba)
    vec_minus(c, b, bc)
    return vec_angle(ba, bc)
}

function vec_angle(a, b, n,   y, x) {
    vec_cross(a, b, n)
    y = vec_abs(n)
    x = vec_dot(a, b)
    return atan2(y, x)
}

function tri_normal(a, b, c, e,   n, ab, ac) {
    vec_minus(b, a,   ab)
    vec_minus(c, a,   ac)
    vec_cross(ab, ac,   n)
    vec_norm(n,   e)
}

function vec_norm(a, e,   r) {
    r = vec_abs(a)
    e[X] = a[X]/r; e[Y] = a[Y]/r; e[Z] = a[Z]/r
}

function vec_get(i, a, a0) {
    a0[X] = a[i, X]; a0[Y] = a[i, Y]; a0[Z] = a[i, Z]
}

function vec_set(i, a0, a) {
    a[i, X] = a0[X]; a[i, Y] = a0[Y]; a[i, Z] = a0[Z]
}

function vec_axpy(i, a, x, y) {
    y[i, X] += a*x[X]
    y[i, Y] += a*x[Y]
    y[i, Z] += a*x[Z]
}

function vec_cylindrical_r(a) {
    return sqrt(a[X]*a[X] + a[Y]*a[Y])
}
