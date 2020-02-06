include(`mh.m4')dnl
#!/usr/bin/env node
'use strict';
mh_include(vec.js)dnl
mh_include(tri.js)dnl

var a, b, c, d
process.argv.shift()
process.argv.shift()
a = vec_argv()
b = vec_argv()
c = vec_argv()

d = vec_ini()

tri_normal(a, b, c,    d)

console.error(d)

