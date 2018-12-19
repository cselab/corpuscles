reset

#set title "bending energy"
set xlabel "iterations/100"
set ylabel "energy"

#set logscale y

#pre="../prolate/eng_vol/Vr"
pre="../oblate/eng_vol/Vr"
#pre="../sphere/eng_vol/Vr"
suf=".vol"

plot for [i=0:1] \
Vr=0.9-i*0.1 \
sprintf("%s%.1f%s", pre, Vr, suf) u 3 w lp t sprintf("Vr=%.1f", Vr)

replot for [i=0:2] \
Vr=0.95-i*0.1 \
sprintf("%s%.2f%s", pre, Vr, suf) u 3 w lp t sprintf("Vr=%.2f", Vr)

#replot for [i=0:11] \
#Vr=0.73-i*0.02 \
#sprintf("%s%.2f%s", pre, Vr, suf) u 3 w lp t sprintf("Vr=%.2f", Vr)

#replot for [i=0:3] \
#Vr=0.5-i*0.1 \
#sprintf("%s%.1f%s", pre, Vr, suf) u 3 w lp t sprintf("Vr=%.1f", Vr)

#replot for [i=0:2] \
#Vr=0.45-i*0.1 \
#sprintf("%s%.2f%s", pre, Vr, suf) u 3 w lp t sprintf("Vr=%.2f", Vr)

reset