divert(-1)

define(`Head',
`define(`$0i', index($1, ;))dnl
ifelse($0i, -1, $1, substr($1, 0, $0i))')

define(`Tail',
`define(`$0i', index($1, ;))dnl
define(`$0n', len($1))dnl
ifelse($0i, -1, ,`substr($1, incr($0i), eval($0n-$0i))')')

define(`App',
`ifelse($2,,,
`$1(Head($2))
App($1, Tail($2))')dnl
')
divert`'dnl
