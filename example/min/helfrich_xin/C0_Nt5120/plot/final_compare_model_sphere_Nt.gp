reset
Es=8*pi#bending energy of a sphere

set size 0.7, 0.7
set term postscript eps enhanced color
set output "final_eb_vr_model_sphere_Nt.eps"

set multiplot

set lmargin 6.5
set bmargin 3.5
set ylabel "E/(8{/Symbol pk}_b)" offset 2.5, 0.3
set xlabel "v"


set xtics 0.05
set ytics 0.25

xl=0.51
xh=0.7
yl=1.5
yh=2.2
set xrange[xl:xh]
set yrange[yl:yh]

set key spacing 1.5
set key width 1
set key samplen 2

e1=1.83
e2=2

set style line 1 lc -1 dt 3 lw 2
set arrow from xl, e1 to 0.65, e1 nohead ls 1
set arrow from xl, e2 to 0.59, e2 nohead ls 1
set arrow from 0.65, yl to 0.65, e1 nohead ls 1
set arrow from 0.59, yl to 0.59, e2 nohead ls 1
unset style line

set label '0.59' at 0.59+0.002, yl+0.05
#set label '0.65' at 0.65+0.002, yl+0.05

set label '1.83' at xl-0.016, e1

psize=1.5
#'../../C0_Nt320/juelicher_Kb0002/sphere/eng_vol/sphere_final.dat' u 2:($3/Es/$1) w p lc 1 pt 3 ps psize t 'model B: N_t=320', \

plot \
'../../../data/1991_Seifert_et_al_PhysRevA/fig8_b.dat' u 1:2 w l lw 2 lc -1 dt 2 t 'theory: oblate',\
'../../C0_Nt1280/juelicher_Kb0002/sphere/eng_vol/sphere_final.dat' u 2:($3/Es/$1) w p lc 1 pt 4 ps psize t 'model B: N_t=1280', \
'../../C0_Nt1280/gompper_kroll_Kb0002/sphere/eng_vol/sphere_final.dat' u 2:($3/Es/$1) w p lc 2 pt 6 ps psize t 'model C: N_t=1280', \
'../../C0_Nt1280/meyer_et_al_Kb0002/sphere/eng_vol/sphere_final.dat' u 2:($3/Es/$1) w p lc 3 pt 8 ps psize t 'model D: N_t=1280'


set key at 0.59, 1.68
set size 0.7, 0.7

psize=1.5
plot \
'../juelicher_Kb0001/sphere/eng_vol/sphere_final.dat' u 2:($3/Es/$1) w p lc 1 pt 5 ps psize t 'model B: N_t=5120', \
'../gompper_kroll_Kb0001/sphere/eng_vol/sphere_final.dat' u 2:($3/Es/$1) w p lc 2 pt 7 ps psize t 'model C: N_t=5120',\
'../meyer_et_al_Kb0001/sphere/eng_vol/sphere_final.dat' u 2:($3/Es/$1) w p lc 3 pt 9 ps psize t 'model D: N_t=5120'

unset multiplot

set term x11

replot
reset