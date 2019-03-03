h_define(`r_define',
`h_define($1, `h_changequote()h_changequote(`,')`'$2`'h_changequote(`<<',`>>')')')dnl
r_define(`r_header', `[$1](lib/co/$1)')dnl
r_define(`r_file', `h_cmd(cat $1)')dnl
r_define(`r_cmd', `h_cmd($1)')dnl
r_define(`r_dir', `[$1]($1/)')dnl
r_define(`r_geomview', `h_syscmd(WX=400 WY=300 co.geomview -o $2 -i "convert %i %o" $1)dnl
h_syscmd(co.install "$2" docs)
![$3]($2)')dnl
h_changequote(`<<', `>>')dnl
