reset

set term postscript enhanced color

#set size 0.7, 0.7
set output 'normal.eps'

sc=0.2;

set xlabel 'x'
set ylabel 'z'

splot 'g.dat' \
u 1:2:3:($12*sc):($13*sc):($14*sc) w vec t 'normal'


reset

set term x11

replot
