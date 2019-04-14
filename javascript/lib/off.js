function off_empty(s)
{
	return s.length == 0
}

function off_uncomment(s)
{
	s = s.replace(/#.*/, '')
	s = s.replace(/^[ \t]*/, '')
	s = s.replace(/[ \t]*$/, '')
	return s
}

function off_read(f)
{
	var lines, NR, nr, l, a, u, nv, nt, i, j, k, t, x, y, z, ver, tri
	var X = 0, Y = 1, Z = 2

	u = off_uncomment
	lines = require('fs').readFileSync(f, 'ascii').split('\n').filter(Boolean)
	NR = lines.length
    	if (NR < 0)
		throw new Error("no lines in file " + f)
	nr = 0
	while (nr < NR && off_empty(l = u(lines[nr++]))) ;
	if (nr == NR || l !== 'OFF')
		throw new Error("not an off file " + f)
	while (nr < NR && off_empty(l = u(lines[nr++]))) ;
	if (nr == NR)
		throw new Error("unexpected EOF " + f)

	a = l.split(" ")
	if (a.length !== 3)
		throw new Error("expecting [nv nt ne], got " + l)

	nv = parseInt(a[0])
	nt = parseInt(a[1])
	ver = new Array(nv)
	tri = new Array(nt)
	for (i = 0; i < nv; i++) {
		while (nr < NR && off_empty(l = u(lines[nr++]))) ;
		if (nr == NR)
			throw new Error("unexpected EOF " + f)
		a = l.split(" ")
		x = parseFloat(a[X])
		y = parseFloat(a[Y])
		z = parseFloat(a[Z])
		ver[i] = [x, y, z]
	}
	
	for (t = 0; t < nt; t++) {
		if (nr == NR)
			throw new Error("unexpected EOF, t = " + t +  "  f = "+ f)
		while (nr < NR && off_empty(l = u(lines[nr++]))) ;
		a = l.split(" ")
		i = parseFloat(a[1])
		j = parseFloat(a[2])
		k = parseFloat(a[3])
		tri[t] = [i, j, k]
	}	
	return {ver : ver, tri : tri}
}

function off_write(f, off)
{
	var nv, nt, ne, s, v, ver, tri, i, t
	var X = 0, Y = 1, Z = 2
	
	if (!('ver' in off))
		throw new Error("not an off object")
	if (!('tri' in off))
		throw new Error("not an off object")

	s = require('fs').createWriteStream(f)

	ver = off.ver
	tri = off.tri

	nv = ver.length
	nt = tri.length
	ne = 0
	s.write("OFF\n")
	s.write(nv + " " + nt + " " + ne + "\n")

	for (i = 0; i < nv; i++) {
		x = ver[i][X]
		y = ver[i][Y]
		z = ver[i][Z]
		s.write(x + " " + y + " " + z + "\n")
	}

	for (t = 0; t < nt; t++) {
		i = tri[t][0]
		j = tri[t][1]
		k = tri[t][2]
		s.write(3 + " " + i + " " + j + " " + k + "\n")
	}
}

function off_nv(off)
{
	if (!('ver' in off))
		throw new Error("not an off object")
	return off.ver.length
}

function off_nt(off)
{
	if (!('tri' in off))
		throw new Error("not an off object")
	return off.tri.length
}

function off_tri(off)
{
	if (!('tri' in off))
		throw new Error("not an off object")
	return off.tri
}

function off_ver(off)
{
	if (!('ver' in off))
		throw new Error("not an off object")
	return off.ver
}