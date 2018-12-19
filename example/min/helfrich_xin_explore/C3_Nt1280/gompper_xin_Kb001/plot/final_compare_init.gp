reset
Es=8*pi#bending energy of a sphere

set size 0.7, 0.7
set term postscript eps enhanced color
set output "final_eb_vr_gompper_C3_compare_init.eps"

#set title "Model B: bending energy vs reduced volume, N_v=1280"


set lmargin 6
set bmargin 3.5
set ylabel "E/(8{/Symbol pk}_b)" offset 3, 0
set xlabel "V_r"

set xtics 0.1
set ytics 0.05

xl=0.5
xh=0.8
yl=0
yh=0.3
set xrange[xl:xh]
set yrange[yl:yh]

set key spacing 1
set key width 1
set key samplen 1

set key center top


set label '0.59' at 0.535, 1.1
set label '0.65' at 0.655, 1.1
psize=1

plot \
'../../../../data/1991_Seifert_et_al_PhysRevA/fig13_a.dat' u 1:2 w l lc -1 t 'theory',\
'../../../../data/1991_Seifert_et_al_PhysRevA/fig13_b.dat' u 1:2 w l lc -1 t '',\
'../../../../data/1991_Seifert_et_al_PhysRevA/fig13_c.dat' u 1:2 w l lc -1 t '',\
'../../../../data/1991_Seifert_et_al_PhysRevA/fig13_d.dat' u 1:2 w l lc -1 t '',\
'../prolate/eng_vol/final.dat' u 2:($3/Es/$1) w p lc 1 pt 4 ps psize t 'init. config.: prolate', \
'../oblate/eng_vol/final.dat' u 2:($3/Es/$1) w p lc 2 pt 6 ps psize t 'init. config.:  oblate',\
'../sphere/eng_vol/final.dat' u 2:($3/Es/$1) w p lc 3 pt 8 ps psize t 'init. config.: sphere'

set term x11
replot

reset
