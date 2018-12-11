divert(-1)
changequote()
changequote([, ])

define([m4_foreach],
[ifelse([$2], [], [],
       [pushdef([$1])_$0([define([$1],], [)$3], [],
  $2)popdef([$1])])])

define([_m4_foreach],
[ifelse([$#], [3], [],
       [$1[$4]$2[]$0([$1], [$2], m4_shift3($@))])])

define([m4_shift3], [shift(shift(shift($@)))])
divert[]dnl
m4_foreach(Var, [a,c,d,preved], [Var[[i]]])dnl

