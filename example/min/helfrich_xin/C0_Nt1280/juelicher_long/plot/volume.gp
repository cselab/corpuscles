reset
Vr="0.55"

fnp="prolate_Vr".Vr.".vol"
fno="oblate_Vr".Vr.".vol"
fns="sphere_Vr".Vr.".vol"

set title "total energy: "."Vr=".Vr
set xlabel "iterations/100"
set ylabel "energy"

c=3
plot \
fnp u c w lp t 'prolate',\
fno u c w lp t 'oblate',\
fns u c w lp t 'sphere'

reset