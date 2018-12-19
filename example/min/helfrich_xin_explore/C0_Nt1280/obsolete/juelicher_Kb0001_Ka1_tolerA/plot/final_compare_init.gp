reset
Es=8*pi#bending energy of a sphere

set size 0.7, 0.7
set term postscript eps enhanced color
set output "final_eb_vr_juelicher_compare_init.eps"


#set title "Model B: bending energy vs reduced volume, N_v=1280"


set lmargin 6
set bmargin 3.5
set ylabel "E/(8{/Symbol pk}_b)" offset 3, 2
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

set key spacing 1.5
set key width 1
set key samplen 2

set key right top

e1=1.83
e2=2

set style line 1 lc -1 dt 3
set style line 2 lc -1 dt 4
set arrow from xl, e1 to 0.65, e1 nohead ls 1
set arrow from xl, e2 to 0.66, e2 nohead ls 2
set arrow from 0.65, yl to 0.65, e1 nohead ls 1
set arrow from 0.59, yl to 0.59, e2 nohead ls 1
unset style line

set label '1.83' at 0.135, 1.83
set label '0.59' at 0.535, 1.1
set label '0.65' at 0.655, 1.1
psize=1

plot \
'../../../../data/1991_Seifert_et_al_PhysRevA/fig8_a.dat' u 1:2 w l lc -1 dt 1 t 'theory:   prolate',\
'../../../../data/1991_Seifert_et_al_PhysRevA/fig8_b.dat' u 1:2 w l lc -1 dt 2 t 'oblate',\
'../../../../data/1991_Seifert_et_al_PhysRevA/fig8_c.dat' u 1:2 w l lc -1 dt 4 t 'stomato',\
'<sort ../prolate/eng_vol/final.dat' u 2:($3/Es/$1) w p lc 1 pt 4 ps psize t 'init. config.: prolate', \
'<sort ../oblate/eng_vol/final.dat' u 2:($3/Es/$1) w p lc 2 pt 6 ps psize t 'oblate',\
'<sort ../sphere/eng_vol/final.dat' u 2:($3/Es/$1) w p lc 3 pt 8 ps psize t 'sphere'


#set key at 0.54, 1.25
#set size 0.7, 0.7

#plot \
#'../stomato_Vr0.4/eng_vol/final.dat' u 2:($3/Es/$1) w p lc 6 pt 11 ps psize t 'init. conf#ig.: stomato 1',\
#'../stomato_Vr0.45/eng_vol/final.dat' u 2:($3/Es/$1) w p lc 7 pt 13 ps psize t 'init. config.: stomato 2'

#unset multiplot

set term x11
replot

reset
