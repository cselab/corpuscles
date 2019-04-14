include(`mh.m4')dnl
#!/usr/bin/env node
mh_include(off.js)dnl
mh_include(he.js)dnl

function he_ini(f)
{
	var off, nv, nt, ne, nh, tt
	var hdg
	var hdg_ver, hdg_tri, nxt, flp, ver, tri, edg
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

	off = off_read(f)
	nv = off_nv(off)
	nt = off_nt(off)
	tt  = off_tri(off)

	nh = 3*nt
	hdg_ver = new Array(nv)
	hdg_tri = new Array(nt)

	nxt = new Array(nh)
	flp = new Array(nh)

	ver = new Array(nh)
	tri = new Array(nh)
	edg = new Array(nh)

	hdg = new Object()
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

var he
he = he_ini("/dev/stdin")
console.log(he)
