reset

Vr="0.7"

fnp="prolate_Vr".Vr.".eng"
fno="oblate_Vr".Vr.".eng"
fns="sphere_Vr".Vr.".eng"

set title "bending energy: "."Vr=".Vr
set xlabel "iterations/100"
set ylabel "energy"

set logscale y

plot \
fnp u 2 w lp t 'prolate: bending eng',\
fno u 2 w lp t 'oblate: bending eng',\
fns u 2 w lp t 'sphere: bending eng'


reset