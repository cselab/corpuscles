reset

set term postscript enhanced color

#set size 0.7, 0.7
set output 'fH2.eps'

sc=0.05;

set xlabel 'x'
set ylabel 'z'

splot 'test_fH2.dat' \
u 1:2:3:($7*sc):($8*sc):($9*sc) w vec t 'fH2 or -lbH'


reset

set term x11

replot
