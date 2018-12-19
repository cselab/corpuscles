reset

#set title "bending energy"
set xlabel "iterations/100"
set ylabel "energy"

#set logscale y

pre="../stomato_Vr0.45_juelicher"
suf1="stomato_Vr"
suf2=".eng"

plot for [i=0:1] \
Vr=0.5+i*0.1 \
sprintf("%s/%.1f/eng_vol/%s%.1f%s", pre, Vr, suf1, Vr, suf2) u ($2+Vr) w lp t sprintf("Vr=%.1f", Vr)

replot for [i=0:1] \
Vr=0.48+i*0.01 \
sprintf("%s/%.2f/eng_vol/%s%.2f%s", pre, Vr, suf1, Vr, suf2) u ($2+Vr) w lp t sprintf("Vr=%.2f", Vr)

replot for [i=0:8] \
Vr=0.51+i*0.01 \
sprintf("%s/%.2f/eng_vol/%s%.2f%s", pre, Vr, suf1, Vr, suf2) u ($2+Vr) w lp t sprintf("Vr=%.2f", Vr)

replot for [i=0:4] \
Vr=0.61+i*0.01 \
sprintf("%s/%.2f/eng_vol/%s%.2f%s", pre, Vr, suf1, Vr, suf2) u ($2+Vr) w lp t sprintf("Vr=%.2f", Vr)

reset