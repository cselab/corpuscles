reset

Vr="0.64"

fnp="../prolate/eng_vol/prolate_Vr".Vr.".eng"
fno="../oblate/eng_vol/oblate_Vr".Vr.".eng"
fns="../sphere/eng_vol/sphere_Vr".Vr.".eng"

set title "bending energy: "."Vr=".Vr
set xlabel "iterations/100"
set ylabel "energy"

#set logscale y

plot \
fnp u 2 w lp t 'prolate: bending eng',\
fno u 2 w lp t 'oblate: bending eng',\
fns u 2 w lp t 'sphere: bending eng'


reset