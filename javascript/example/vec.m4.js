include(`mh.m4')dnl
#!/usr/bin/env node
'use strict';
mh_include(vec.js)dnl

var a, b, c
process.argv.shift()
process.argv.shift()
a = vec_argv()
b = vec_argv()
c = vec_ini()

vec_normalize(a)
vec_norm(a, c)

console.error(a)
console.error(c)
