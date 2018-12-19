reset
Es=8*pi#bending energy of a sphere

set term pdfcairo enhanced color fontscale 0.7
set output "final_bc_eb_Da.pdf"
#set size 0.7, 0.7

#set title "Model B: bending energy vs reduced volume, N_v=1280"


set lmargin 6
set bmargin 3.5
set ylabel "E/(8{/Symbol pk}_b)" offset 3, 0
set xlabel "{/Symbol D}a"

set multiplot

set xtics 0.1
set ytics 0.5

xl=1
xh=1.6
yl=1.5
yh=3
set xrange[xl:xh]
set yrange[yl:yh]

set key spacing 1.5
set key width 1
set key samplen 1

set key left top

set label 'v=0.7' at 1.12, 1.62
set label '0.65' at 1.18, 1.81
set label '0.6' at 1.22, 2.02
set label '0.55' at 1.28, 2.25
set label '0.5' at 1.35, 2.54

psize=1

plot \
'../../../data/2005_Ziherl_Svetina_EurophysLett/fig2_a.dat' u 1:2 w l lc -1 t '',\
'../../../data/2005_Ziherl_Svetina_EurophysLett/fig2_b.dat' u 1:2 w l lc -1 t '',\
'../../../data/2005_Ziherl_Svetina_EurophysLett/fig2_c.dat' u 1:2 w l lc -1 t '',\
'../../../data/2005_Ziherl_Svetina_EurophysLett/fig2_d.dat' u 1:2 w l lc -1 t '',\
'../../../data/2005_Ziherl_Svetina_EurophysLett/fig2_e.dat' u 1:2 w l lc -1 t '',\
'../juelicher_Vr069/sphere/eng_vol/final.dat' u 6:($5/Es/$1) w p lc 1 pt 4 ps psize t 'scheme B: N_t=1280', \
'../juelicher_Vr069/sphere/eng_vol_cont/final.dat' u 6:($5/Es/$1) w p lc 1 pt 4 ps psize t '', \
'../gompper_xin_Vr069/sphere/eng_vol/final.dat' u 6:($5/Es/$1) w p lc 2 pt 6 ps psize t 'scheme C: N_t=1280',\
'../meyer_xin_Vr069/sphere/eng_vol/final.dat' u 6:($5/Es/$1) w p lc 3 pt 8 ps psize t 'scheme D: N_t=1280',\
'../juelicher_Vr0642/sphere/eng_vol/final.dat' u 6:($5/Es/$1) w p lc 1 pt 4 ps psize t '', \
'../juelicher_Vr0642/sphere/eng_vol_cont/final.dat' u 6:($5/Es/$1) w p lc 1 pt 4 ps psize t '', \
'../gompper_xin_Vr0642/sphere/eng_vol/final.dat' u 6:($5/Es/$1) w p lc 2 pt 6 ps psize t '', \
'../gompper_xin_Vr0642/sphere/eng_vol_cont/final.dat' u 6:($5/Es/$1) w p lc 2 pt 6 ps psize t '', \
'../meyer_xin_Vr0642/sphere/eng_vol/final.dat' u 6:($5/Es/$1) w p lc 3 pt 8 ps psize t '', \
'../meyer_xin_Vr0642/sphere/eng_vol_cont/final.dat' u 6:($5/Es/$1) w p lc 3 pt 8 ps psize t '', \
'../juelicher_Vr059/sphere/eng_vol/final.dat' u 6:($5/Es/$1) w p lc 1 pt 4 ps psize t '', \
'../juelicher_Vr059/sphere/eng_vol_cont/final.dat' u 6:($5/Es/$1) w p lc 1 pt 4 ps psize t '', \
'../juelicher_Vr055_Kv2/sphere/eng_vol/final.dat' u 6:($5/Es/$1) w p lc 1 pt 4 ps psize t '', \
'../juelicher_Vr055_Kv2/sphere/eng_vol_cont/final.dat' u 6:($5/Es/$1) w p lc 1 pt 4 ps psize t '', \
'../juelicher_Vr05_Kv2/sphere/eng_vol/final.dat' u 6:($5/Es/$1) w p lc 1 pt 4 ps psize t '', \
'../juelicher_Vr05_Kv2/sphere/eng_vol_Kv/final.dat' u 6:($5/Es/$1) w p lc 1 pt 4 ps psize t ''

#set key at 1.45, 1.9
set key right bottom
#set size 0.7, 0.7

psize=0.6
plot \
'../../bc_Nt5120/juelicher_Vr069/sphere/eng_vol/final.dat' u 6:($5/Es/$1) w p lc 1 pt 5 ps psize t 'scheme B: N_t=5120', \
'../../bc_Nt5120/juelicher_Vr069/sphere/eng_vol_cont/final.dat' u 6:($5/Es/$1) w p lc 1 pt 5 ps psize t '', \
'../../bc_Nt5120/gompper_xin_Vr069/sphere/eng_vol/final.dat' u 6:($5/Es/$1) w p lc 2 pt 7 ps psize t 'scheme C: N_t=5120',\
'../../bc_Nt5120/meyer_xin_Vr069/sphere/eng_vol/final.dat' u 6:($5/Es/$1) w p lc 3 pt 9 ps psize t 'scheme D: N_t=5120',\
'../../bc_Nt5120/juelicher_Vr0642/sphere/eng_vol/final.dat' u 6:($5/Es/$1) w p lc 1 pt 5 ps psize t '', \
'../../bc_Nt5120/gompper_xin_Vr0642/sphere/eng_vol/final.dat' u 6:($5/Es/$1) w p lc 2 pt 7 ps psize t '',\
'../../bc_Nt5120/meyer_xin_Vr0642/sphere/eng_vol/final.dat' u 6:($5/Es/$1) w p lc 3 pt 9 ps psize t '',\
'../../bc_Nt5120/juelicher_Vr059/sphere/eng_vol/final.dat' u 6:($5/Es/$1) w p lc 1 pt 5 ps psize t '', \
'../../bc_Nt5120/juelicher_Vr059/sphere/eng_vol_cont/final.dat' u 6:($5/Es/$1) w p lc 1 pt 5 ps psize t '', \
'../../bc_Nt5120/juelicher_Vr055_Kv2/sphere/eng_vol/final.dat' u 6:($5/Es/$1) w p lc 1 pt 5 ps psize t '', \
'../../bc_Nt5120/juelicher_Vr055_Kv2/sphere/eng_vol_cont/final.dat' u 6:($5/Es/$1) w p lc 1 pt 5 ps psize t '', \
'../../bc_Nt5120/juelicher_Vr055_Kv2/sphere/eng_vol_cont_Kv/final.dat' u 6:($5/Es/$1) w p lc 1 pt 5 ps psize t '', \
'../../bc_Nt5120/juelicher_Vr05_Kv5/sphere/eng_vol/final.dat' u 6:($5/Es/$1) w p lc 1 pt 5 ps psize t ''


unset multiplot

set term x11
replot

reset