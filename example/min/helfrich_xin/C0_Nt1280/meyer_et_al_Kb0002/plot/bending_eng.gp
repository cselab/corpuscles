reset

#set title "reduced volume"
set xlabel "iterations/100"
set ylabel "energy"

#set logscale y

pre="../prolate/eng_vol/prolate_Vr"
pre="../oblate/eng_vol/oblate_Vr"
#pre="../sphere/eng_vol/sphere_Vr"
suf=".eng"

plot for [i=0:2] \
Vr=0.2+i*0.1 \
sprintf("%s%.1f%s", pre, Vr, suf) u ($2+Vr) w lp t sprintf("Vr=%.1f", Vr)

replot for [i=0:2] \
Vr=0.25+i*0.1 \
sprintf("%s%.2f%s", pre, Vr, suf) u ($2+Vr) w lp t sprintf("Vr=%.2f", Vr)


#replot for [i=0:4] \
#Vr=0.55+i*0.01 \
#sprintf("%s%.2f%s", pre, Vr, suf) u ($2+Vr) w lp t sprintf("Vr=%.2f", Vr)

#replot for [i=0:6] \
#Vr=0.61+i*0.01 \
#sprintf("%s%.2f%s", pre, Vr, suf) u ($2+Vr) w lp t sprintf("Vr=%.2f", Vr)

reset