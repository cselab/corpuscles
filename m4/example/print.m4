include(`he.m4')dnl
h_define(`f', `$1')dnl
dnl
h_define(`KEY', `h_do(
h_assert(`$#==1'),
h_map_sep(`h_echo', ` ', `$1'))')dnl
dnl
h_define(`_E', `%e')dnl
h_define(`E', `h_do(
h_assert(`$#==1'),
h_map_sep(`_$0', ` ', `$1'))')dnl
dnl
h_define(list, ``a, b, c, V/V0'')dnl
KEY("list")
print("E(list)\n", list)
