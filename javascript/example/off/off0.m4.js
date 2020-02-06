include(`mh.m4')dnl
#!/usr/bin/env node
mh_include(off.js)dnl
var off
off = off_read("/dev/stdin")
off_write("t.off", off)
console.log("end")
