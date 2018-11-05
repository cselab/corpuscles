reset

Vr="0.7"

fnp="prolate_Vr".Vr.".eng"
fno="oblate_Vr".Vr.".eng"
fns="sphere_Vr".Vr.".eng"

set title "total energy: "."Vr=".Vr
set xlabel "iterations/100"
set ylabel "energy"

set logscale y

plot \
fnp u 1 w lp t 'prolate: total eng',\
fno u 1 w lp t 'oblate: total eng',\
fns u 1 w lp t 'sphere: total eng'


reset