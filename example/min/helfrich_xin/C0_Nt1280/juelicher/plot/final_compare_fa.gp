reset
Es=8*pi#bending energy of a sphere

set size 0.7, 0.7
set term postscript eps enhanced color
set output "final_eb_vr_juelicher_compare_fa.eps"

set title "Model A: bending energy vs reduced volume"
set ylabel "E_b/(8{/Symbol pk}_b)"
set xlabel "V_r"

set key spacing 1.5
plot [0.3:1][1:3.5]\
'../prolate/eng_vol/prolate_final.dat' u 2:($3/Es/$1) w lp pt 4 t 'fv init. config. V_r: prolate', \
'../oblate/eng_vol/oblate_final.dat' u 2:($3/Es/$1) w lp pt 6 t 'fv init. config. V_r: oblate',\
'../sphere/eng_vol/sphere_final.dat' u 2:($3/Es/$1) w lp pt 8 t 'fv init. config.: sphere',\
'../../../../helfrich_xin_fa/C0_Nv1280/juelicher_Kb0002/prolate/eng_vol/prolate_final.dat' u 2:($3/Es/$1) w lp pt 10 t 'fa  init. config. V_r: prolate', \
'../../../../helfrich_xin_fa/C0_Nv1280/juelicher_Kb0002/oblate/eng_vol/oblate_final.dat' u 2:($3/Es/$1) w lp pt 12 t 'fa init. config. V_r: oblate',\
'../../../../helfrich_xin_fa/C0_Nv1280/juelicher_Kb0002/sphere/eng_vol/sphere_final.dat' u 2:($3/Es/$1) w lp pt 14 t 'fa init. config.: sphere'


set term x11
replot
reset