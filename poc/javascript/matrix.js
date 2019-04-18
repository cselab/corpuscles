function matrix_perspective(out, fovy, aspect, near, far)
{
	var f, nf
	f = 1.0 / Math.tan(fovy / 2)
	nf = 1 / (near - far)
	out[0] = f / aspect
	out[1] = 0
	out[2] = 0
	out[3] = 0
	out[4] = 0
	out[5] = f
	out[6] = 0
	out[7] = 0
	out[8] = 0
	out[9] = 0
	out[10] = (far + near) * nf
	out[11] = -1
	out[12] = 0
	out[13] = 0
	out[14] = 2 * far * near * nf
	out[15] = 0
	return out
}

function matrix_create()
{
	var out
	out = new Array(16)
	out[0] = 1
	out[1] = 0
	out[2] = 0
	out[3] = 0
	out[4] = 0
	out[5] = 1
	out[6] = 0
	out[7] = 0
	out[8] = 0
	out[9] = 0
	out[10] = 1
	out[11] = 0
	out[12] = 0
	out[13] = 0
	out[14] = 0
	out[15] = 1
	return out
}

function matrix_translate(out, a, v) {
	var x = v[0], y = v[1], z = v[2]

	out[12] = a[0]*x + a[4]*y + a[8]*z + a[12]
	out[13] = a[1]*x + a[5]*y + a[9]*z + a[13]
	out[14] = a[2]*x + a[6]*y + a[10]*z + a[14]
	out[15] = a[3]*x + a[7]*y + a[11]*z + a[15]
	return out
}