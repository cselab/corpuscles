function tri_area(a, b, c)
{
	function sq(x)
	{
		return x*x
	}
	var X = 0, Y = 1, Z = 2
	var x, y, z, u, v, w
	x = b[X] - a[X]
	y = b[Y] - a[Y]
	z = b[Z] - a[Z]
	u = c[X] - a[X]
	v = c[Y] - a[Y]
	w = c[Z] - a[Z]
	return Math.sqrt(sq(y*w-z*v)+sq(z*u-x*w)+sq(x*v-y*u))/2
}

function tri_dir(a, b, c,   d)
{
	var X = 0, Y = 1, Z = 2
	var x, y, z, u, v, w
	x = b[X] - a[X]
	y = b[Y] - a[Y]
	z = b[Z] - a[Z]
	u = c[X] - a[X]
	v = c[Y] - a[Y]
	w = c[Z] - a[Z]
	d[X] = w*y-v*z
	d[Y] = u*z-w*x
	d[Z] = v*x - u*y
}

function tri_normal(a, b, c,   n)
{
	tri_dir(a, b, c,   n)
	vec_normalize(n)
}
