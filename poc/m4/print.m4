divert(-1)
changequote()
changequote([, ])

define([m4_shift3], [shift(shift(shift($@)))])

define([m4_foreach],
[ifelse([$2], [], [],
       [pushdef([$1])_$0([define([$1],], [)$3], [],
  $2)popdef([$1])])])

define([_m4_foreach],
[ifelse([$#], [3], [],
       [$1[$4]$2[]$0([$1], [$2], m4_shift3($@))])])

define([_m4_apply],
[ifelse([$2], [], [], [$1($2)[]])])

define([m4_map],
[_m4_foreach([_m4_apply([$1],], [)], [], $2)])

define([m4_unquote], [$*])

define([m4_map_sep],
[pushdef([m4_Sep], [define([m4_Sep], defn([m4_unquote]))])]dnl
[_m4_foreach([_m4_apply([m4_Sep([$2])[]$1],], [)], [], $3)popdef([m4_Sep])])

divert[]dnl

m4_foreach(Var, [a,b,c], [Var[[i]]]
)dnl

m4_map_sep([preved], [, ], [a,b,c,f(x,y)])
