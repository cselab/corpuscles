include(`he.m4')dnl
h_define(list, ``a, b, c'')dnl
h_define(`narg', `$#')dnl
h_define(`show', `($*)')dnl
show(h_cdr(h_unquote(list)))dnl
