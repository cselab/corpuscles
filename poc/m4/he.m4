divert(-1)
changequote()
changequote(`, ')

define(`h_define',   defn(`define'))
define(`h_defn',     defn(`defn'))
define(`h_undefine', defn(`undefine'))
define(`h_errprint', defn(`errprint'))

h_undefine(`define')
h_undefine(`defn')
h_undefine(`undefine')
h_undefine(`errprint')

h_define(`h_location',
`__file__:__line__')

h_define(`h_errprintn',
`h_errprint(`$1
')')

h_define(`h_warning',
`h_errprint(h_location`: warning: $1')')

divert`'dnl
