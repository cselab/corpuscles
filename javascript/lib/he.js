function he_ini(f)
{
	var off, nv, nt, tt
	off = off_read(f)
	nv = off_nv(off)
	nt = off_nt(off)
	tt  = off_tri(off)
	return he_ini0(nv, nt, tt)
}

function he_ver_ini(f)
{
	var off, nv, nt, tt, he, ver

	off = off_read(f)
	nv = off_nv(off)
	nt = off_nt(off)
	tt  = off_tri(off)
	ver = off_ver(off)

	he = he_ini0(nv, nt, tt)

	return { 'he': he, 'ver': ver}
}

function he_ini0(nv, nt, tt)
{
	var ne, nh
	var hdg
	var hdg_ver, hdg_edg, hdg_tri, nxt, flp, ver, tri, edg
	var t, e, i, j, k, u, v, w, h, f

	function g(i, j)
	{
		return i + " " + j
	}

	function setup_hdg(h, t, i, j)
	{
		hdg[g(i,j)] = h
		hdg_tri[t] = hdg_ver[i] = h
		ver[h] = i
		tri[h] = t
	}

	function setup_flp(i, j)
	{
		var h, f
		h = hdg[g(i,j)]
		if (g(j,i) in hdg) {
			f = hdg[g(j,i)]
			flp[h] = f
		} else
			flp[h] = -1
	}

	nh = 3*nt
	hdg_ver = new Array(nv)
	hdg_tri = new Array(nt)

	nxt = new Array(nh)
	flp = new Array(nh)

	ver = new Array(nh)
	tri = new Array(nh)
	edg = new Array(nh)

	hdg = { }
	for (t = h = 0; t < nt; t++) {
		i = tt[t][0]
		j = tt[t][1]
		k = tt[t][2]
		setup_hdg(u = h++, t, i, j)
		setup_hdg(v = h++, t, j, k)
		setup_hdg(w = h++, t, k, i)
		nxt[u] = v
		nxt[v] = w
		nxt[w] = u
	}

	for (t = e = 0; t < nt; t++) {
		i = tt[t][0]
		j = tt[t][1]
		k = tt[t][2]
		setup_flp(i, j)
		setup_flp(j, k)
		setup_flp(k, i)
	}

	for (h = e = 0; h < nh; h++) {
		f = flp[h]
		if (f in edg) edg[h] = edg[f]
		else edg[h] = e++
	}
	ne = e

	hdg_edg = new Array(ne)
	for (h = 0; h < nh; h++) {
		e = edg[h]
		hdg_edg[e] = h
	}

	return {'nxt': nxt, 'flp': flp, 'ver' : ver, 'tri': tri, 'edg': edg,
		    'hdg_ver': hdg_ver, 'hdg_edg': hdg_edg, 'hdg_tri': hdg_tri}
}

function he_nv(he)
{
	if (!('hdg_ver' in he))
		throw new Error("not a he object")
	return he.hdg_ver.length
}

function he_nt(he)
{
	if (!('hdg_tri' in he))
		throw new Error("not a he object")
	return he.hdg_tri.length
}

function he_ne(he)
{
	if (!('hdg_edg' in he))
		throw new Error("not a he object")
	return he.hdg_edg.length
}

function he_nh(he)
{
	if (!('ver' in he))
		throw new Error("not a he object")
	return he.ver.length
}

function he_tri(he, h)
{
	var nh
	if (!('tri' in he))
		throw new Error("not a he object")
	nh = he_nh(he)
	if (!(h in he.tri))
		throw new Error("h = " + h+ " nh = " + nh)
	return he.tri[h]
}

function he_hdg_tri(he, t)
{
	var nt
	if (!('hdg_tri' in he))
		throw new Error("not a he object")
	nt = he_nt(he)
	if (!(t in he.hdg_tri))
		throw new Error("t = " + t + " nt = " + nt)
	return he.hdg_tri[t]
}

function he_hdg_edg(he, e)
{
	var ne
	if (!('hdg_edg' in he))
		throw new Error("not a he object")
	ne = he_ne(he)
	if (!(e in he.hdg_edg))
		throw new Error("e = " + e + " ne = " + ne)
	return he.hdg_edg[e]
}

function he_nxt(he, h)
{
	var nh
	if (!('nxt' in he))
		throw new Error("not a he object")
	nh = he_nh(he)
	if (!(h in he.nxt))
		throw new Error("h = " + h + " nh = " + nh)
	return he.nxt[h]
}

function he_flp(he, h)
{
	var nh
	if (!('flp' in he))
		throw new Error("not a he object")
	nh = he_nh(he)
	if (!(h in he.flp))
		throw new Error("h = " + h + " nh = " + nh)
	return he.flp[h]
}

function he_ver(he, h)
{
	var nh
	if (!('ver' in he))
		throw new Error("not a he object")
	nh = he_nh(he)
	if (!(h in he.ver))
		throw new Error("h = " + h + " nh = " + nh)
	return he.ver[h]
}

function he_edg(he, h)
{
	var nh
	if (!('edg' in he))
		throw new Error("not a he object")
	nh = he_nh(he)
	if (!(h in he.edg))
		throw new Error("h = " + h + " nh = " + nh)
	return he.edg[h]
}

function he_tri_ijk(he, t)
{
	function hdg_tri(t)
	{
		return he_hdg_tri(he, t)
	}
	function nxt(h)
	{
		return he_nxt(he, h)
	}
	function ver(h)
	{
		return he_ver(he, h)
	}
	var u, v, w, i, j, k
	u = hdg_tri(t)
	v = nxt(u)
	w = nxt(v)
	i = ver(u)
	j = ver(v)
	k = ver(w)
	return [i, j, k]
}

function he_edg_ijkl(he, e)
{
	function edg(e)
	{
		return he_hdg_edg(he, e)
	}
	function nxt(h)
	{
		return he_nxt(he, h)
	}
	function ver(h)
	{
		return he_ver(he, h)
	}
	function bnd(h)
	{
		return he_bnd(he, h)
	}
	function flp(h)
	{
		return he_flp(he, h)
	}

	var i, j, k, l
	var u, v, w, y

	v = edg(e)
	if (bnd(v))
		return [-1, -1, -1, -1]
	w = nxt(v)
	u = nxt(w)
	y = nxt(nxt(flp(v)))

	i = ver(u)
	j = ver(v)
	k = ver(w)
	l = ver(y)
	return [i, j, k, l]
}

function he_bnd(he, h)
{
	var nh
	nh = he_nh(he)
	if (!(h in he.flp))
		throw new Error("h = " + h + " nh = " + nh)
	return he.flp[h] == -1
}

