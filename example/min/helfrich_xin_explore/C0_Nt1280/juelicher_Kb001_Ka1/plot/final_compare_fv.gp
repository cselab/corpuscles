reset
Es=8*pi#bending energy of a sphere

set size 0.7, 0.7
set term pdfcairo enhanced color fontscale 0.7
set output "final_eb_vr_juelicher_compare_fv.pdf"

#set title "Model B: bending energy vs reduced volume, N_v=1280"


set lmargin 6
set bmargin 3.5
set ylabel "E/(8{/Symbol pk}_b)" offset 3, 0
set xlabel "V_r"

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

set key right top

e1=1.83
e2=2

set style line 1 lc -1 dt 2
set arrow from xl, e2 to 0.59, e2 nohead ls 1
set arrow from 0.59, yl to 0.59, e2 nohead ls 1
set arrow from 0.65, yl to 0.65, e1 nohead ls 1
unset style line

set label '0.59' at 0.535, 1.1
set label '0.65' at 0.655, 1.1
psize=1

plot \
'<sort ../prolate/eng_vol/final.dat' u 2:($3/Es/$1) w lp lc 1 pt 4 ps psize t 'init. config.: prolate', \
'<sort ../oblate/eng_vol/final.dat' u 2:($3/Es/$1) w lp lc 2 pt 6 ps psize t 'init. config.:  oblate',\
'<sort ../sphere/eng_vol/final.dat' u 2:($3/Es/$1) w lp lc 3 pt 8 ps psize t 'init. config.: sphere',\
'../../../../helfrich_xin/C0_Nt1280/juelicher_Kb0002/prolate/eng_vol/prolate_final.dat' u 2:($3/Es/$1) w lp lc 4 pt 10 ps psize t 'fv init. config.: prolate',\
'../../../../helfrich_xin/C0_Nt1280/juelicher_Kb0002/oblate/eng_vol/oblate_final.dat' u 2:($3/Es/$1) w lp lc 5 pt 12 ps psize t 'fv init. config.: oblate',\
'../../../../helfrich_xin/C0_Nt1280/juelicher_Kb0002/sphere/eng_vol/sphere_final.dat' u 2:($3/Es/$1) w lp lc 6 pt 14 ps psize t 'fv init. config.: sphere'

set term x11
replot

reset
