divert(-1)
define(`BEGIN_T', `
for (h = 0; h in ii; h++) {
    i = ii[h]; j = jj[h]; k = kk[h]
    get3(i, j, k, a, b, c)')
define(`END_T', }
)
define(`BEGIN_V', `
for (i = 0; i < nv; i++) {
    get(i, a)')
define(`END_V', }
)
define(`LPL',`
pushdef(`f', `$1')
pushdef(`g', `$2')
BEGIN_T
    g(i) -= tb[h]*(f(i) - f(k)) + tc[h]*(f(i) - f(j))
END_T
BEGIN_V
    g(i) /= 2*area[i]
END_V
popdef(`f')
popdef(`g')')
divert`'dnl
"${AWK=awk}" '
include(math.m4)
BEGIN {
    ini()
    read()

    BEGIN_T
	tb[h] = tri_cot(a, b, c)
	tc[h] = tri_cot(b, c, a)

	tri_normal(a, b, c,   u)

	sb = edg_sq(a, b)
	sc = edg_sq(a, c)
	ang = tri_angle(c, a, b)

	area[i] += (tb[h]*sc + tc[h]*sb)/8
	vec_axpy(i, ang, u,  n)
	K[i] += ang
    END_T

    BEGIN_V
	K[i] = (2*pi - K[i])/area[i]
	vec_get(i, n, n0)
	vec_norm(n0, n0)
	vec_set(i, n0, n)
    END_V

    LPL(`r[$1, X]', `lp[$1, X]')
    LPL(`r[$1, Y]', `lp[$1, Y]')
    LPL(`r[$1, Z]', `lp[$1, Z]')

    BEGIN_V
       vec_get(i, lp,   lp0)
       H[i] = vec_abs(lp0)/2
       if (vec_dot_i(i, n, lp) < 0)
	   H[i] = -H[i]
    END_V
    LPL(`H[$1]', `lpH[$1]')

    BEGIN_V
        lpH[i] = -lpH[i]
        fm[i] = 2*(lpH[i] + 2*H[i]*(H[i]^2 - K[i]))
    END_V

    print "r fm H K lpl area"
    BEGIN_V
        rr = vec_cylindrical_r(a)
        print rr, fm[i], H[i], K[i], lpH[i], area[i]
    END_V
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

function msg(s) { print s | "cat >&2" }
'
