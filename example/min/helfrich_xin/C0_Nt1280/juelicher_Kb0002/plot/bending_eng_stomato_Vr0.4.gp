reset

#set title "bending energy"
set xlabel "iterations/100"
set ylabel "energy"

#set logscale y

pre="../stomato_Vr0.4"
suf1="stomato_Vr"
suf2=".eng"

plot for [i=0:10] \
Vr=0.45+i*0.02 \
sprintf("%s/%.2f/eng_vol/%s%.2f%s", pre, Vr, suf1, Vr, suf2) u ($2+Vr) w lp t sprintf("Vr=%.2f", Vr)

reset