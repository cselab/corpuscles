reset
Es=8*pi#bending energy of a sphere

set size 0.7, 0.7
set term postscript eps enhanced color
set output "final_eb_vr_juelicher_init.eps"

#set title "Model A: bending energy vs reduced volume, fv, N_v=5120"
set ylabel "E_b/(8{/Symbol pk}_b)"
set xlabel "V_r"

#set multiplot

set xtics 0.1
set ytics 0.5

xl=0.2
xh=1
yl=1
yh=2.8
set xrange[xl:xh]
set yrange[yl:yh]

set key spacing 1
set key width 1
set key samplen 1



set style line 1 lc -1 dt 2
set arrow from xl, 2 to 0.59, 2 nohead ls 1
set arrow from 0.59, yl to 0.59, 2 nohead ls 1
set arrow from 0.65, yl to 0.65, 1.8 nohead ls 1
unset style line

set label '0.59' at 0.54, 1.05
set label '0.65' at 0.65, 1.05
psize=1

plot \
'../prolate/eng_vol/prolate_final.dat' u 2:($3/Es/$1) w lp pt 4 ps psize t 'init. config.: prolate', \
'../oblate/eng_vol/oblate_final.dat' u 2:($3/Es/$1) w lp pt 6 ps psize t 'init. config.:  oblate',\
'../sphere/eng_vol/sphere_final.dat' u 2:($3/Es/$1) w lp pt 8 ps psize t 'init. config.: sphere'

set term x11

replot
reset