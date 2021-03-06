divert(-1)

changequote()
changequote(<[[,]]>)

define(<[[mh_define]]>,   defn(<[[define]]>))
define(<[[mh_defn]]>,     defn(<[[defn]]>))
define(<[[mh_undefine]]>, defn(<[[undefine]]>))
mh_undefine(<[[define]]>)
mh_undefine(<[[defn]]>)
mh_undefine(<[[undefine]]>)

mh_define(<[[mh_copy]]>,
<[[mh_define(<[[$2]]>, mh_defn(<[[$1]]>))]]>)

mh_define(<[[mh_rename]]>,
<[[mh_copy(<[[$1]]>, <[[$2]]>)mh_undefine(<[[$1]]>)]]>)

mh_define(<[[mh_rename_mh]]>,
<[[mh_rename(<[[$1]]>, <[[mh_$1]]>)]]>)

mh_rename_mh(<[[builtin]]>)
mh_rename_mh(<[[changecom]]>)
mh_rename_mh(<[[changequote]]>)
mh_rename_mh(<[[debugfile]]>)
mh_rename_mh(<[[debugmode]]>)
mh_rename_mh(<[[decr]]>)
mh_rename_mh(<[[divnum]]>)
mh_rename_mh(<[[dumpdef]]>)
mh_rename_mh(<[[errprint]]>)
mh_rename_mh(<[[esycmd]]>)
mh_rename_mh(<[[eval]]>)
mh_rename_mh(<[[format]]>)
mh_rename_mh(<[[ifelse]]>)
mh_rename_mh(<[[include]]>)
mh_rename_mh(<[[incr]]>)
mh_rename_mh(<[[index]]>)
mh_rename_mh(<[[indir]]>)
mh_rename_mh(<[[len]]>)
mh_rename_mh(<[[maketemp]]>)
mh_rename_mh(<[[patsubst]]>)
mh_rename_mh(<[[popdef]]>)
mh_rename_mh(<[[pushdef]]>)
mh_rename_mh(<[[regexp]]>)
mh_rename_mh(<[[shift]]>)
mh_rename_mh(<[[substr]]>)
mh_rename_mh(<[[syscmd]]>)
mh_rename_mh(<[[sysval]]>)
mh_rename_mh(<[[traceoff]]>)
mh_rename_mh(<[[traceon]]>)
mh_rename_mh(<[[translit]]>)

mh_copy(<[[m4exit]]>, <[[mh_exit]]>)

mh_define(<[[mh_location]]>,
<[[__file__:__line__]]>)

mh_define(<[[mh_errprintn]]>,
<[[mh_errprint(<[[$1
]]>)]]>)

mh_define(<[[mh_warn]]>,
<[[mh_errprint(mh_location<[[: warning: $1]]>)]]>)

mh_define(<[[mh_fatal]]>,
<[[mh_errprintn(mh_location<[[: error: $1: $2]]>)mh_exit(1)]]>)

mh_define(<[[mh_assert]]>,
<[[mh_ifelse(mh_eval(<[[$1]]>), 0,
       <[[mh_fatal(<[[assert failed: $1]]>, <[[$2]]>)]]>)]]>)

divert<[[]]>dnl
