reset

#set title "bending energy"
set xlabel "iterations/100"
set ylabel "energy"

#set logscale y

set yrange[0:1.1]
#pre="../prolate/eng_vol/Vr"
#pre="../oblate/eng_vol/Vr"
pre="../sphere/eng_vol/Vr"
suf=".vol"

plot for [i=0:3] \
Vr=0.5+i*0.1 \
sprintf("%s%.1f%s", pre, Vr, suf) u 3 w lp t sprintf("Vr=%.1f", Vr)

replot for [i=1:4] \
Vr=0.5+i*0.02 \
sprintf("%s%.2f%s", pre, Vr, suf) u 3 w lp t sprintf("Vr=%.2f", Vr)

replot for [i=1:4] \
Vr=0.6+i*0.02 \
sprintf("%s%.2f%s", pre, Vr, suf) u 3 w lp t sprintf("Vr=%.2f", Vr)

replot for [i=1:4] \
Vr=0.7+i*0.02 \
sprintf("%s%.2f%s", pre, Vr, suf) u 3 w lp t sprintf("Vr=%.2f", Vr)

reset