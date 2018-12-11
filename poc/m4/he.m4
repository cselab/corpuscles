divert(-1)
changequote()
changequote(`, ')

define(`h_define',   defn(`define'))
define(`h_defn',     defn(`defn'))
define(`h_undefine', defn(`undefine'))
define(`h_errprint', defn(`errprint'))
define(`h_shift', defn(`shift'))
define(`h_ifelse', defn(`ifelse'))
define(`h_pushdef', defn(`pushdef'))
define(`h_popdef', defn(`popdef'))

h_undefine(`define')
h_undefine(`defn')
h_undefine(`undefine')
h_undefine(`errprint')
h_undefine(`shift')
h_undefine(`ifelse')
h_undefine(`pushdef')
h_undefine(`popdef')

h_define(`h_location',
`__file__:__line__')

h_define(`h_errprintn',
`h_errprint(`$1
')')

h_define(`h_warning',
`h_errprint(h_location`: warning: $1')')

h_define(`h_shift3', `h_shift(h_shift(h_shift($@)))')

h_define(`h_shift2', `h_shift(h_shift($@))')

h_define(`h_foreach',
`h_ifelse(`$2', `', `',
       `h_pushdef(`$1')_$0(`h_define(`$1',', `)$3', `',
  $2)h_popdef(`$1')')')

h_define(`_h_foreach',
`h_ifelse(`$#', `3', `',
       `$1`$4'$2`'$0(`$1', `$2', h_shift3($@))')')

h_define(`_h_apply',
`h_ifelse(`$2', `', `', `$1($2)`'')')

h_define(`h_map',
`_h_foreach(`_h_apply(`$1',', `)', `', $2)')

h_define(`h_unquote', `$*')

h_define(`h_map_sep',
`h_pushdef(`Sep', `h_define(`Sep', h_defn(`h_unquote'))')'dnl
`_h_foreach(`_h_apply(`Sep(`$2')`'$1',', `)', `', $3)h_popdef(`Sep')')

h_define(`h_join',
`h_ifelse(`$#', `1', `',
       `$#', `2', ``$2'',
       `h_ifelse(`$2', `', `', ``$2'_')$0(`$1', h_shift2($@))')')
h_define(`_h_join',
`h_ifelse(`$#$2', `2', `',
       `h_ifelse(`$2', `', `', ``$1$2'')$0(`$1', h_shift2($@))')')

divert`'dnl
