reset
Es=8*pi#bending energy of a sphere

set size 0.7, 0.7
set term postscript eps enhanced color
set output "final_eb_Da_juelicher.eps"


#set title "Model B: bending energy vs reduced volume, N_v=1280"



set xtics 0.1
set ytics 0.5

xl=0.2
xh=1.25
yl=1
yh=5
set xrange[xl:xh]
set yrange[yl:yh]

set key spacing 1.5
set key width 1
set key samplen 2

set key right top

psize=1

plot \
'<sort -k 4 ../sphere/eng_vol/final.dat' u 4:($3/Es/$1) w lp lc 3 pt 8 ps psize t 'sphere'



set term x11
replot

reset