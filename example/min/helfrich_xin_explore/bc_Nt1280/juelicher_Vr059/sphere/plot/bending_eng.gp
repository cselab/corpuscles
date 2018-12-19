reset

#set title "bending energy"
set xlabel "iterations/100"
set ylabel "energy"

#set logscale y

set yrange [1:1.7]

pre="../eng_vol/Da"
suf=".eng"

c=2

plot for [i=0:3] \
Da=1.1+i*0.1 \
sprintf("%s%.1f%s", pre, Da, suf) u ($2+Da) w lp t sprintf("Da1=%.1f", Da)

replot for [i=1:4] \
Da=1.1+i*0.02 \
sprintf("%s%.2f%s", pre, Da, suf) u ($2+Da) w lp t sprintf("Da1=%.2f", Da)

replot for [i=1:1] \
Da=1.2+i*0.02 \
sprintf("%s%.2f%s", pre, Da, suf) u ($2+Da) w lp t sprintf("Da1=%.2f", Da)

replot for [i=0:1] \
Da=1.25+i*0.1 \
sprintf("%s%.2f%s", pre, Da, suf) u ($2+Da) w lp t sprintf("Da1=%.2f", Da)

reset