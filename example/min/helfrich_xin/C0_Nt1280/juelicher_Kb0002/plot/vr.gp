reset

#set title "reduced volume"
set xlabel "iterations/100"
set ylabel "energy"

#set logscale y

pre="../prolate/eng_vol/prolate_Vr"
pre="../oblate/eng_vol/oblate_Vr"
pre="../sphere/eng_vol/sphere_Vr"
suf=".vol"

plot for [i=0:7] \
Vr=0.2+i*0.1 \
sprintf("%s%.1f%s", pre, Vr, suf) u 3 w lp t sprintf("Vr=%.1f", Vr)

replot for [i=0:7] \
Vr=0.25+i*0.1 \
sprintf("%s%.2f%s", pre, Vr, suf) u 3 w lp t sprintf("Vr=%.2f", Vr)

reset