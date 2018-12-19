reset

set term postscript enhanced color

#set size 0.7, 0.7
set output 'lbx.eps'

sc=0.03;

set xlabel 'x'
set ylabel 'z'

splot 'g.dat' \
u 1:2:3:($15*sc):($16*sc):($17*sc) w vec t 'lbx'


reset

set term x11

replot
