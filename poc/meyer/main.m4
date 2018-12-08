define(`BEGIN_T', `dnl
for (h = 0; h in ii; h++) {
    t = int(h/3)
    i = ii[h]; j = jj[h]; k = kk[h]
    get3(i, j, k, a, b, c)')dnl
define(`END_T', }
)dnl
define(`BEGIN_V', `dnl
for (i = 0; i < nv; i++) {
    get(i, a)')dnl
define(`END_V', }
)dnl
dnl
define(`LPL',`dnl
pushdef(`f', `$1')dnl
pushdef(`g', `$2')dnl
BEGIN_T
    g(i) += tb[h]*(f(i) - f(k)) + tc[h]*(f(i) - f(j))
END_T
BEGIN_V
    g(i) /= 2*area[i]
END_V
popdef(`f')
popdef(`g')')dnl
"${AWK=awk}" '
BEGIN {
    ini()
    read()

    BEGIN_T
	tb[h] = tri_cot(a, b, c)
	tc[h] = tri_cot(b, c, a)
	ang[h] = tri_angle(c, a, b)

	sb = edg_sq(a, b)
	sc = edg_sq(a, c)
	area[i] += (tb[h]*sc + tc[h]*sb)/8
	K[i] += ang[h]
    END_T

    BEGIN_V
        K[i] = (2*pi - K[i])/area[i]
    END_V

    LPL(`r[$1, X]', `lp[$1, X]')
    LPL(`r[$1, Y]', `lp[$1, Y]')
    LPL(`r[$1, Z]', `lp[$1, Z]')

    print lp[0, X], lp[0, X], lp[0, Z]
}

function read(   v, t, h, i, j, k) {
    getline; getline
    nv = $1; nt = $2
    for (v = 0; v < nv; v++) {
	getline
	r[v,X] = $1; r[v,Y] = $2; r[v,Z] = $3
    }
    for (h = t = 0; t <  nt; t++) {
	getline
	i = $2; j = $3; k = $4
	ii[h] = i; jj[h] = j; kk[h++] = k
	jj[h] = i; kk[h] = j; ii[h++] = k
	kk[h] = i; ii[h] = j; jj[h++] = k
    }
}

function ini() {
    pi = 3.141592653589793115997964
    X = 0; Y = 1; Z = 2
}
function get3(i, j, k, a, b, c) {
    get(i, a); get(j, b); get(k, c)
}
function get(i, a) {
    a[X] = r[i, X]
    a[Y] = r[i, Y]
    a[Z] = r[i, Z]
}

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

'
