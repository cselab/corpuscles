reset

#set title "bending energy"
set xlabel "iterations/100"
set ylabel "energy"

#set logscale y

pre="../sphere/eng_vol/Da"
suf=".eng"

set yrange [1:1.5]
#set yrange [0.2:1.2]

c=2

plot for [i=0:0] \
Da=1+i*0.1 \
sprintf("%s%.0f%s", pre, Da, suf) u ($2+Da) w lp t sprintf("Da1=%.0f", Da)

replot for [i=0:0] \
Da=1.05+i*0.05 \
sprintf("%s%.2f%s", pre, Da, suf) u ($2+Da) w lp t sprintf("Da1=%.2f", Da)

replot for [i=1:4] \
Da=1.1+i*0.02 \
sprintf("%s%.2f%s", pre, Da, suf) u ($2+Da) w lp t sprintf("Da1=%.2f", Da)

replot for [i=0:0] \
Da=1.2+i*0.01 \
sprintf("%s%.1f%s", pre, Da, suf) u ($2+Da) w lp t sprintf("Da1=%.1f", Da)

replot for [i=1:1] \
Da=1.2+i*0.02 \
sprintf("%s%.2f%s", pre, Da, suf) u ($2+Da) w lp t sprintf("Da1=%.2f", Da)


#replot for [i=0:7] \
#Da=0.2+i*0.1 \
#sprintf("%s%.1f%s", pre, Da, suf) u ($2+Da) w lp t sprintf("Da1=%.1f", Da)

reset