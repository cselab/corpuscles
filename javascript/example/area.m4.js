include(`mh.m4')dnl
#!/usr/bin/env node
'use strict';
mh_include(off.js)dnl
mh_include(he.js)dnl
mh_include(vec.js)dnl
mh_include(tri.js)dnl
mh_include(area.js)dnl

var r, he, ver, nv, A, a
r = he_ver_ini("/dev/stdin")
he = r.he
ver = r.ver

nv = he_nv(he)
a = new Array(nv)
A = area_surface(he, ver)
area_ver(he, ver, a)

console.error(a)
