include(`he.m4')dnl
h_define(`r_define',
`h_define($1,
`h_changequote()h_changequote(`,')$2h_changequote(`<<',`>>')')')dnl
r_define(`r_cmd', `h_cmd(`$1')')dnl
h_changequote(`<<',`>>')dnl
r_cmd(seq 5)dnl
