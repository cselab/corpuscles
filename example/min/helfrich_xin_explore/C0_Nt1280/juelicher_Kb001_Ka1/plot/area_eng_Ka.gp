reset

#set title "bending energy"
set xlabel "iterations/100"
set ylabel "energy"

set logscale y

pre1="../"

pre2="prolate/eng_vol/Vr"
pre2="oblate/eng_vol/Vr"
pre2="sphere/eng_vol/Vr"
suf=".eng"

ka1="juelicher_Kb01/"
ka2="juelicher_Kb001/"
ka3="juelicher_Kb001_Ka1/"

Vr="0.3"

set title "Vr=".Vr.pre2

#sprintf("%s%s%s%s%s%s", pre1, pre1, ka1, pre2, Vr, suf) u ($3) w lp t sprintf("%s", ka1),
plot \
sprintf("%s%s%s%s%s%s", pre1, pre1, ka2, pre2, Vr, suf) u ($3) w lp t sprintf("%s", ka2),\
sprintf("%s%s%s%s%s%s", pre1, pre1, ka3, pre2, Vr, suf) u ($3) w lp t sprintf("%s", ka3)

reset