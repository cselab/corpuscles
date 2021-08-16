divert(-1)dnl
h_define(`r_define',
`h_define($1, `h_changequote()h_changequote(`,')`'$2`'h_changequote(`<<',`>>')')')
h_define(`r_PREFIX', https://github.com/cselab/corpuscles/blob/master)
r_define(`r_header', `<a href="r_PREFIX/lib/co/$1">$1</a>')
r_define(`r_source', `<a href="r_PREFIX/$1">$1</a>')
r_define(`r_source2', `<a href="r_PREFIX/$1">$2</a>')
r_define(`r_file', `h_cmd(m4/cat $1)')
r_define(`r_cmd', `h_cmd($1)')
r_define(`r_dir', `[$1](r_PREFIX/$1/)')
r_define(`r_geomview', `h_syscmd(WX=400 WY=300 co.geomview -o $2 -i "convert %i %o" $1)dnl
h_syscmd(co.install "$2" docs)dnl
<p align="center"><img src="$2" alt="$3"/></p>')
h_define(`r_nav',
`<div class="nav">
[Home](index.html) / $1
</div>')dnl
h_changequote(`<<', `>>')
divert<<>>dnl
