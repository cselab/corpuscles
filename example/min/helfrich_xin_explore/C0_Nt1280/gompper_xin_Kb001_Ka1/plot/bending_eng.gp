reset

#set title "bending energy"
set xlabel "iterations/100"
set ylabel "energy"

#set logscale y

#pre="../prolate/eng_vol/Vr"
#pre="../oblate/eng_vol/Vr"
pre="../sphere/eng_vol/Vr"
suf=".eng"

#set xrange [0:200]

plot for [i=1:2] \
Vr=0.2+i*0.1 \
sprintf("%s%.1f%s", pre, Vr, suf) u ($2+Vr) w lp t sprintf("Vr=%.1f", Vr)

replot for [i=0:2] \
Vr=0.25+i*0.1 \
sprintf("%s%.2f%s", pre, Vr, suf) u ($2+Vr) w lp t sprintf("Vr=%.2f", Vr)

reset