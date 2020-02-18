include(`co.m4')dnl
h_format(`%08d
', 42)dnl
h_define(`ind', `{`'h_translit(``$1'', `a-z', `A-Z')`'}')dnl
h_define(g, `medved')dnl
ind(preved)
ind(`preved, medved')
ind(``preved'')
ind(`g')
