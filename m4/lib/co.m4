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

h_rename_h(`changecom')
h_rename_h(`changequote')
h_rename_h(`decr')
h_rename_h(`dumpdef')
h_rename_h(`errprint')
h_rename_h(`esyscmd')
h_rename_h(`eval')
h_rename_h(`format')
h_rename_h(`ifdef')
h_rename_h(`ifelse')
h_rename_h(`include')
h_rename_h(`incr')
h_rename_h(`index')
h_rename_h(`indir')
h_rename_h(`len')
h_rename_h(`maketemp')
h_rename_h(`mkstemp')
h_rename_h(`patsubst')
h_rename_h(`popdef')
h_rename_h(`pushdef')
h_rename_h(`regexp')
h_rename_h(`shift')
h_rename_h(`sinclude')
h_rename_h(`substr')
h_rename_h(`syscmd')
h_rename_h(`sysval')
h_rename_h(`traceoff')
h_rename_h(`traceon')
h_rename_h(`translit')
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
       `h_fatal(`assert failed: $1')')')

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

h_define(`h_map_args',
`h_ifelse(`$#', `0', `h_fatal(`$0: too few arguments: $#')',
       `$#', `1', `',
       `$#', `2', `$1(`$2')`'',
       `_h_foreach(`$1(', `)', $@)')')

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
h_define(`h_reverse',
`h_ifelse(`$#', `0', `', `$#', `1', ``$1'',
       `$0(h_shift($@)), `$1'')')
h_define(`h_unquote', `$*')
h_define(`h_dquote', ``$@'')
h_define(`h_do',
`h_ifelse(`$#', 0, `',
         `$#', 1, `$1`'',
          `$1`'$0(h_shift($@))')')

h_define(`h_car', ``$1'')
h_define(`h_cdr',
`h_ifelse(`$#', 0, `h_warn(`$0: cannot be called without arguments')',
          `$#', 1, `',
          `h_dquote(h_shift($@))')')

h_define(`h_foreach_sep',
`h_pushdef(`Sep', `h_define(`Sep', `$2')')dnl
_$0(`$1', `$3', `$4')dnl
h_popdef(`Sep')')

h_define(`h_newline', `
$1')

h_define(`_h_foreach_sep',
`h_ifelse(`$2', `', `',
`h_pushdef(`$1', `h_car($2)')dnl
Sep`'$3`'dnl
h_popdef(`$1')$0(`$1', h_cdr($2), `$3')')')

h_define(`h_ifval',
`h_ifelse(`$1', `', `$3', `$2')')

h_define(`h_count', `$#')

h_define(`h_curry', `$1(h_shift($@,)_$0')
h_define(`_h_curry',             ``$1')')

h_define(`upcase', `h_translit(`$*', `a-z', `A-Z')')
h_define(`downcase', `h_translit(`$*', `A-Z', `a-z')')
h_define(`_arg1', `$1')
h_define(`_to_alt', `h_changequote(`<<[', `]>>')')
h_define(`_from_alt', `h_changequote(<<[`]>>, <<[']>>)')
h_define(`_upcase_alt', `h_translit(<<[$*]>>, <<[a-z]>>, <<[A-Z]>>)')
h_define(`_downcase_alt', `h_translit(<<[$*]>>, <<[A-Z]>>, <<[a-z]>>)')
h_define(`_capitalize_alt',
  `h_regexp(<<[$1]>>, <<[^\(\w\)\(\w*\)]>>,
    <<[_upcase_alt(<<[<<[\1]>>]>>)_downcase_alt(<<[<<[\2]>>]>>)]>>)')
h_define(`capitalize',
  `_arg1(_to_alt()h_patsubst(<<[<<[$*]>>]>>, <<[\w+]>>,
    _from_alt()`]>>_$0_alt(<<[\&]>>)<<['_to_alt())_from_alt())')
h_define(`h_cmd',
`h_esyscmd($1)dnl
h_ifelse(h_sysval, `0', `', `h_fatal(command "`$1'" failed)')')
h_define(`h_backtick', h_changequote([,
 ])[h_changequote([,])`h_changequote(`,')]h_changequote(`,')))
h_define(`h_qsingle', h_changequote([,
 ])[h_changequote([,])'h_changequote(`,')]h_changequote(`,')))
divert`'dnl
