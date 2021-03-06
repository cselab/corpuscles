reset

#set title "bending energy"
set xlabel "iterations/100"
set ylabel "energy"

#set logscale y

pre="../eng_vol/Da"
suf=".eng"

set yrange [1:1.8]
#set yrange [0.2:1.2]

c=2

plot for [i=0:0] \
Da=1+i*0.1 \
sprintf("%s%.0f%s", pre, Da, suf) u ($2+Da) w lp t sprintf("Da1=%.0f", Da)

replot for [i=0:4] \
Da=1.05+i*0.1 \
sprintf("%s%.2f%s", pre, Da, suf) u ($2+Da) w lp t sprintf("Da1=%.2f", Da)

replot for [i=0:4] \
Da=1.1+i*0.1 \
sprintf("%s%.1f%s", pre, Da, suf) u ($2+Da) w lp t sprintf("Da1=%.1f", Da)

reset