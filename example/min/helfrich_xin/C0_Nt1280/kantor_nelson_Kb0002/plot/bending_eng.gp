reset

#set title "reduced volume"
set xlabel "iterations/100"
set ylabel "energy"

#set logscale y

pre="../prolate/eng_vol/prolate_Vr"
#pre="../oblate/eng_vol/oblate_Vr"
#pre="../sphere/eng_vol/sphere_Vr"
suf=".eng"

#plot for [i=0:4] \
#Vr=0.95-i*0.1 \
#sprintf("%s%.2f%s", pre, Vr, suf) u 2 w lp t sprintf("Vr=%.2f", Vr)

#replot for [i=0:3] \
#Vr=0.9-i*0.1 \
#sprintf("%s%.1f%s", pre, Vr, suf) u 2 w lp t sprintf("Vr=%.1f", Vr)


plot for [i=0:6] \
Vr=0.67-i*0.01 \
sprintf("%s%.2f%s", pre, Vr, suf) u 2 w lp t sprintf("Vr=%.2f", Vr)

#replot for [i=0:4] \
#Vr=0.55+i*0.01 \
#sprintf("%s%.2f%s", pre, Vr, suf) u 2 w lp t sprintf("Vr=%.2f", Vr)


reset