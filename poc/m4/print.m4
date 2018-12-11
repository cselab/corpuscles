divert(-1)
changequote()
changequote([, ])

define([shift3], [shift(shift(shift($@)))])
define([shift2], [shift(shift($@))])

define([foreach],
[ifelse([$2], [], [],
       [pushdef([$1])_$0([define([$1],], [)$3], [],
  $2)popdef([$1])])])

define([_foreach],
[ifelse([$#], [3], [],
       [$1[$4]$2[]$0([$1], [$2], shift3($@))])])

define([_apply],
[ifelse([$2], [], [], [$1($2)[]])])

define([map],
[_foreach([_apply([$1],], [)], [], $2)])

define([unquote], [$*])

define([map_sep],
[pushdef([Sep], [define([Sep], defn([unquote]))])]dnl
[_foreach([_apply([Sep([$2])[]$1],], [)], [], $3)popdef([Sep])])

define([join],
[ifelse([$#], [1], [],
       [$#], [2], [[$2]],
       [ifelse([$2], [], [], [[$2]_])$0([$1], shift2($@))])])
define([_join],
[ifelse([$#$2], [2], [],
       [ifelse([$2], [], [], [[$1$2]])$0([$1], shift2($@))])])

divert[]dnl

foreach(Var, [a,b,c], [Var[[i]]]
)dnl

map_sep([preved], [, ], [a,b,c])dnl

map([preved], [a,b,c])dnl

join([, ], [a,b,c])

define([preved], [$1[[i]]])
map_sep([preved], [, ], [a, b, c])
