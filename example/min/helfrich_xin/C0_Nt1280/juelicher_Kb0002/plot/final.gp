reset
Kb=0.002
Es=8*pi*Kb#bending energy of a sphere

set size 0.7, 0.7
set term postscript eps enhanced color
set output "final_eb_vr_juelicher.eps"

set title "Model A: bending energy vs reduced volume"
set ylabel "E_b/(8{/Symbol pk}_b)"
set xlabel "V_r"

set key spacing 1.5
plot [0.5:0.7][1:2.5]\
'../prolate/eng_vol/prolate_final.dat' u 1:($2/Es) w lp pt 4 t 'init. config. V_r: prolate', \
'../oblate/eng_vol/oblate_final.dat' u 1:($2/Es) w lp pt 6 t 'init. config. V_r: oblate',\
'../sphere/eng_vol/sphere_final.dat' u 1:($2/Es) w lp pt 8 t 'init. config.: sphere'


set term x11
replot
reset