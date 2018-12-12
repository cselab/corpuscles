divert(-1)
changequote()
changequote(`, ')

define(`h_define',   defn(`define'))
define(`h_defn',     defn(`defn'))
define(`h_undefine', defn(`undefine'))
h_undefine(`define')
h_undefine(`defn')
h_undefine(`undefine')

h_define(`h_copy',
`h_define(`$2', h_defn(`$1'))')

h_define(`h_rename',
`h_copy(`$1', `$2')h_undefine(`$1')')

h_define(`h_rename_h',
`h_rename(`$1', `h_$1')')

h_rename_h(`errprint')
h_rename_h(`shift')
h_rename_h(`ifelse')
h_rename_h(`pushdef')
h_rename_h(`popdef')
h_copy(`m4exit', `h_exit')

h_define(`h_location',
`__file__:__line__')

h_define(`h_errprintn',
`h_errprint(`$1
')')

h_define(`h_warn',
`h_errprint(h_location`: warning: $1')')

h_define(`h_fatal',
`h_errprintn(h_location`: error: $1')h_exit(1)')

h_define(`h_assert',
`h_ifelse(h_eval(`$1'), 0,
       `h_fatal(`assert failed: $1', `$2')')')

h_define(`h_shift3', `h_shift(h_shift(h_shift($@)))')

h_define(`h_shift2', `h_shift(h_shift($@))')

h_define(`h_foreach',
`h_ifelse(`$2', `', `',
          `h_pushdef(`$1')_$0(`h_define(`$1',', `)$3', `',
  $2)h_popdef(`$1')')')

h_define(`_h_foreach',
`h_ifelse(`$#', `3', `',
          `$1`$4'$2`'$0(`$1', `$2', h_shift3($@))')')

h_define(`h_apply',
`h_ifelse(`$2', `', `$1', `$1($2)')`'')

h_define(`_h_apply',
`h_ifelse(`$2', `', `', `$1($2)`'')')

h_define(`h_map',
`_h_foreach(`_h_apply(`$1',', `)', `', $2)')

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

h_define(`h_echo', `$@')
h_define(`h_ignore')
h_define(`h_unquote', `$*')
h_define(`h_dquote', ``$@'')

h_define(`h_car', ``$1'')
h_define(`h_cdr',
`h_ifelse(`$#', 0, `h_warn(`$0: cannot be called without arguments')',
          `$#', 1, `',
          `h_dquote(h_shift($@))')')

h_define(`h_aux',
`h_pushdef(`Sep', `h_define(`Sep', h_defn(`h_unquote'))')dnl
_h_aux($@)dnl
h_popdef(`Sep')')

h_define(`_h_aux',
`h_ifelse(`$3', `', `',
`h_pushdef(`$1', `h_car($3)')dnl
Sep(`$2')$4`'dnl
h_popdef(`$1')_h_aux(`$1', `$2', h_cdr($3), `$4')')')

divert`'dnl
