reset

#set title "bending energy"
set xlabel "iterations/100"
set ylabel "energy"

set logscale y

pre="../sphere/eng_vol/Da"
suf="_restart.eng"

c=2

#plot for [i=0:0] \
#Da=0.3+i*0.1 \
#sprintf("%s%.1f%s", pre, Da, suf) u c w lp t sprintf("Da1=%.1f", Da)


plot for [i=1:4] \
Da=0.2+i*0.02 \
sprintf("%s%.2f%s", pre, Da, suf) u c w lp t sprintf("Da1=%.2f", Da)

reset