include(`mh.m4')dnl
#!/usr/bin/env node
'use strict';
mh_include(off.js)dnl
mh_include(he.js)dnl

function rank(he, ans)
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

	var nt, t, u, v, w, i, j, k
	nt = he_nt(he)
	nv = he_nv(he)
	for (i = 0; i < nv; i++)
		ans[i] = 0

	for (t = 0; t < nt; t++) {
		u = hdg_tri(t)
		v = nxt(u)
		w = nxt(v)
		i = ver(u)
		j = ver(v)
		k = ver(w)
		ans[i] += 1
		ans[j] += 1
		ans[k] += 1
	}
}

function rank2(he, ans)
{
	function hdg_edg(e)
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

	var ne, e, u, v, i, j
	ne = he_ne(he)
	nv = he_nv(he)
	for (i = 0; i < nv; i++)
		ans[i] = 0

	for (e = 0; e < ne; e++) {
		u = hdg_edg(e)
		v = nxt(u)
		i = ver(u)
		j = ver(v)
		ans[i] += 1
		ans[j] += 1
	}
}

var he, r, nv, ne, i, r, ver
r = he_ver_ini("/dev/stdin")
he = r.he
ver = r.ver

nv = he_nv(he)

r = new Array(nv)
rank2(he, r)

for (i = 0; i < nv; i++)
	console.log(r[i])

ne = he_ne(he)
//console.log(he_edg_ijkl(he, ne - 2))

//console.log(ver)
