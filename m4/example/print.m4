include(`he.m4')dnl
h_define(`f', `$1')dnl
dnl
h_define(`h_key', `h_do(
h_assert(`$#==1'),
h_map_sep(`h_echo', ` ', `$1'))')dnl
dnl
h_define(`_h_e', `%e')dnl
h_define(`h_e', `h_do(
h_assert(`$#==1'),
h_map_sep(`_$0', ` ', `$1'))')dnl
dnl
h_define(list, ``a, b, c, V/V0'')dnl
h_key("list")
print("h_e(list)\n", list)
