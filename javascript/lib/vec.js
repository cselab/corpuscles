 function vec_ini()
{
	return new Array(3)
}

function vec_copy(a,   b)
{
	var X = 0, Y = 1, Z = 2
	b[X] = a[X]
	b[Y] = a[Y]
	b[Z] = a[Z]
}

function vec_argv()
{
	var X = 0, Y = 1, Z = 2
	var x, y, z, a
	a = process.argv
	x = parseFloat(a[X])
	y = parseFloat(a[Y])
	z = parseFloat(a[Z])
	if (isNaN(x) || isNaN(y) || isNaN(z))
		throw new Error("fail to get vector: " + a)
	a.shift(); a.shift(); a.shift()
	return [x, y, z]
}

function vec_dot(a, b)
{
	var X = 0, Y = 1, Z = 2
	return a[X]*b[X] + a[Y]*b[Y] + a[Z]*b[Z]
}

function vec_abs(a)
{
	var X = 0, Y = 1, Z = 2
	return Math.sqrt(a[X]*a[X] + a[Y]*a[Y] + a[Z]*a[Z])
}

function vec_cross(a, b,   c)
{
	var X = 0, Y = 1, Z = 2
	c[X] = a[Y]*b[Z]-b[Y]*a[Z]
	c[Y] = b[X]*a[Z]-a[X]*b[Z]
	c[Z] = a[X]*b[Y]-b[X]*a[Y]
}

function vec_scalar(a, s,   b)
{
	var X = 0, Y = 1, Z = 2
	b[X] = s*a[X]
	b[Y] = s*a[Y]
	b[Z] = s*a[Z]
}

function vec_small(s) {
	var eps = 1e-32
	if      (s >  eps) return 0
	else if (s < -eps) return 0
	else               return 1
}

function vec_norm(a,   b)
{
	var s
	s = vec_abs(a)
	if (!vec_small(s)) vec_scalar(a, 1/s, b)
	else vec_copy(a, b)
}

function vec_normalize(a)
{
	var s
	s = vec_abs(a)
	if (!vec_small(s))
		vec_scale(1/s, a)
}

function vec_scale(s, a)
{
	var X = 0, Y = 1, Z = 2
	a[X] *= s
	a[Y] *= s
	a[Z] *= s
}
