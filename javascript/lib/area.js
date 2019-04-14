function area_surface(he, ver)
{
	var nt, t, tri,a, b, c, A
	nt = he_nt(he)
	A = 0
	for (t = 0; t < nt; t++) {
		tri = he_tri_ijk(he, t)
		a = ver[tri[0]]
		b = ver[tri[1]]
		c = ver[tri[2]]
		A += tri_area(a, b, c)
	}
	return A
}

function area_tri(he, ver,   area)
{
	var nt, t, tri,a, b, c, A
	nt = he_nt(he)
	for (t = 0; t < nt; t++) {
		tri = he_tri_ijk(he, t)
		a = ver[tri[0]]
		b = ver[tri[1]]
		c = ver[tri[2]]
		area[t] = tri_area(a, b, c)
	}
}

function area_ver(he, ver,   area)
{
	var nt, nv, t, tri,a, b, c, i, j, k, A
	nt = he_nt(he)
	nv = he_nv(he)
	for (i = 0; i < nv; i++)
		area[i] = 0
	for (t = 0; t < nt; t++) {
		tri = he_tri_ijk(he, t)
		i = tri[0]
		j = tri[1]
		k = tri[2]
		a = ver[i]
		b = ver[j]
		c = ver[k]
		A = tri_area(a, b, c)/3
		area[i] += A
		area[j] += A
		area[k] += A
	}
}
