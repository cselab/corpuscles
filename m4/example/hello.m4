include(`co.m4')dnl
h_define(`N', 42)dnl
h_define(`f', `N')dnl
h_define(`bump', h_assert($# == 1, bump needs one argumen)`$1 = $1 + 1')dnl
bump(`x')
