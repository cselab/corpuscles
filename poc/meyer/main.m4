divert(-1)
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
include(`loop.m4')
include(`math.m4')
include(`off.m4')
BEGIN {
    math_ini()
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

function msg(s) { print s | "cat >&2" }
'
