reset

#set title "bending energy"
set xlabel "iterations/100"
set ylabel "energy"

#set logscale y

pre="../sphere/eng_vol/Da"
pre2="../sphere/eng_vol_long/Da"
suf=".vol"

c=3

#plot for [i=0:2] \
#Da=0.1+i*0.1 \
#sprintf("%s%.1f%s", pre, Da, suf) u c w lp t sprintf("Da1=%.1f", Da)

#plot for [i=1:4] \
#Da=0.1+i*0.02 \
#sprintf("%s%.2f%s", pre, Da, suf) u c w lp t sprintf("Da1=%.2f", Da)

plot for [i=1:4] \
Da=0.2+i*0.01 \
sprintf("%s%.2f%s", pre2, Da, suf) u c w lp t sprintf("Da1=%.2f", Da)

replot for [i=0:1] \
Da=0.26+i*0.02 \
sprintf("%s%.2f%s", pre2, Da, suf) u c w lp t sprintf("Da1=%.2f", Da)

replot for [i=0:0] \
Da=0.3+i*0.1 \
sprintf("%s%.1f%s", pre2, Da, suf) u c w lp t sprintf("Da1=%.1f", Da)

reset