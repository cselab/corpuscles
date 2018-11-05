reset

Vr="0.65"

fnp="prolate_Vr".Vr.".eng"
fno="oblate_Vr".Vr.".eng"
fns="sphere_Vr".Vr.".eng"

set title "kinetic energy: "."Vr=".Vr
set xlabel "iterations/100"
set ylabel "energy"

set logscale y

plot \
fnp u 3 w lp t 'prolate: kinetic eng',\
fno u 3 w lp t 'oblate: kinetic eng',\
fns u 3 w lp t 'sphere: kinetic eng'


reset