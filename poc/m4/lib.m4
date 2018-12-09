divert(-1)

define(`Head',
`define(`$0i', `index($1, ;)')dnl
ifelse($0i,-1,$1,`substr($1, 0, $0i)')')

define(`Tail',
`define(`$0i', `index($1, ;)')dnl
define(`$0n', `len($1)')dnl
ifelse($0i,-1,,`substr($1, incr($0i), eval($0n-$0i))')')

define(`App',
`ifelse($2,,,
`$1(Head(`$2'))`'dnl
App(`$1', Tail($2))')')

define(`AppLambda',
`define(`$0f', `$1')`'dnl
App(`$0f', $2)')

define(`Splice',`translit($1,;,`,')')

divert`'dnl
