reset
Es=8*pi#bending energy of a sphere

set term pdfcairo enhanced color fontscale 0.7
set output "final_ade_eb_Da0.pdf"

#set title "Model B: bending energy vs reduced volume, N_v=1280"


set lmargin 6
set bmargin 3.5
set ylabel "E/(8{/Symbol pk}_b)" offset 3, 0
set xlabel "100{/Symbol D}a_0"

set multiplot

set xtics 0.05
set ytics 0.5

xl=0.09
xh=0.31
yl=1
yh=3
set xrange[xl:xh]
set yrange[yl:yh]

set key spacing 1.5
set key width 1
set key samplen 1

set key left top

psize=1

plot \
'../juelicher_Vr0642/sphere/eng_vol/final_combine.dat' u 4:($3/Es/$1) w p lc 1 pt 4 ps psize t 'model B: N_t=1280', \
'../juelicher_Vr0642/sphere/eng_vol_restart/final_combine.dat' u 4:($3/Es/$1) w p lc 1 pt 4 ps psize t '', \
'../gompper_xin_Vr0642/sphere/eng_vol/final_combine.dat' u 4:($3/Es/$1) w p lc 2 pt 6 ps psize t 'model C: N_t=1280',\
'../gompper_xin_Vr0642/sphere/eng_vol_restart/final_combine.dat' u 4:($3/Es/$1) w p lc 2 pt 6 ps psize t '',\
'../meyer_et_al_Vr0642/sphere/eng_vol/final_combine.dat' u 4:($3/Es/$1) w p lc 3 pt 8 ps psize t 'model D: N_t=1280',\
'../meyer_et_al_Vr0642/sphere/eng_vol_long/final_combine.dat' u 4:($3/Es/$1) w p lc 3 pt 8 ps psize t '',\
'../meyer_et_al_Vr0642/sphere/eng_vol_long_restart/final_combine.dat' u 4:($3/Es/$1) w p lc 3 pt 8 ps psize t ''

#set key at 1.45, 1.9
set key right bottom

psize=1
plot \
'../../ade_Nt5120/juelicher_Vr0642/sphere/eng_vol/final_combine.dat' u 4:($3/Es/$1) w p lc 1 pt 5 ps psize t 'model B: N_t=5120', \
'../../ade_Nt5120/juelicher_Vr0642/sphere/eng_vol_restart/final_combine.dat' u 4:($3/Es/$1) w p lc 1 pt 5 ps psize t '', \
'../../ade_Nt5120/gompper_xin_Vr0642/sphere/eng_vol/final_combine.dat' u 4:($3/Es/$1) w p lc 2 pt 7 ps psize t 'model C: N_t=5120',\
'../../ade_Nt5120/meyer_et_al_Vr0642/sphere/eng_vol/final_combine.dat' u 4:($3/Es/$1) w p lc 3 pt 9 ps psize t 'model D: N_t=5120',\
'../../ade_Nt5120/meyer_et_al_Vr0642/sphere/eng_vol_long/final_combine.dat' u 4:($3/Es/$1) w p lc 3 pt 9 ps psize t ''

unset multiplot

set term x11
replot

reset
