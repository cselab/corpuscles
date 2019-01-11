include(`he.m4')dnl
h_define(`f', `print(fmt, $@)')dnl
h_apply(`f', `a, b, c')
h_apply(`f', `a, b, c', `x, y, z')
