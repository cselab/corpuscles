reset
Es=8*pi#bending energy of a sphere

set size 0.7, 0.7
set term pdfcairo enhanced color fontscale 0.7
set output "final_eb_vr_C24_compare_init.pdf"

#set title "Model B: bending energy vs reduced volume, N_v=1280"


set lmargin 6
set bmargin 3.5
set ylabel "E/(8{/Symbol pk}_b)" offset 3, 0
set xlabel "v"

set multiplot

set xtics 0.05
set ytics 0.05

xl=0.52
xh=0.8
yl=0.02
yh=0.22
set xrange[xl:xh]
set yrange[yl:yh]

set key spacing 1.5
set key width 1
set key samplen 1

set key center top

set label '0.59' at 0.535, 1.1
set label '0.65' at 0.655, 1.1
psize=1

plot \
'../../../data/1991_Seifert_et_al_PhysRevA/fig11_a.dat' u 1:2 w l lc -1 t '',\
'../../../data/1991_Seifert_et_al_PhysRevA/fig11_b.dat' u 1:2 w l lc -1 t '',\
'../../../data/1991_Seifert_et_al_PhysRevA/fig11_c.dat' u 1:2 w l lc -1 t '',\
'../../../data/1991_Seifert_et_al_PhysRevA/fig11_d.dat' u 1:2 w l lc -1 t '',\
'../juelicher_Kb001/prolate/eng_vol/final.dat' u 2:($3/Es/$1) w p lc 1 pt 4 ps psize t 'model B: prolate', \
'../gompper_xin_Kb001/prolate/eng_vol/final_combine.dat' u 2:($3/Es/$1) w p lc 2 pt 6 ps psize t 'model C: prolate',\
'../meyer_et_al_Kb001/prolate/eng_vol/final.dat' u 2:($3/Es/$1) w p lc 3 pt 8 ps psize t 'model D: prolate'

set key at 0.56, 0.07
set size 0.7, 0.7

plot \
'../juelicher_Kb001/sphere/eng_vol/final.dat' u 2:($3/Es/$1) w p lc 1 pt 5 ps psize t 'model B: sphere', \
'../gompper_xin_Kb001/sphere/eng_vol/final_combine.dat' u 2:($3/Es/$1) w p lc 2 pt 7 ps psize t 'model C: sphere',\
'../meyer_et_al_Kb001/sphere/eng_vol/final.dat' u 2:($3/Es/$1) w p lc 3 pt 9 ps psize t 'model D: sphere'

unset multiplot

set term x11
replot

reset
