include(`co.m4')dnl
h_define(`map2', `h_ifval(`$2',
`$1(`h_car($2)', `h_car($3)')$0(`$1', h_cdr($2), h_cdr($3))')')dnl
h_define(`show', `$1 = $2;
')dnl
h_define(`c', 42)dnl
map2(`show', `a, b, `c'', `10, 20, 30')dnl
