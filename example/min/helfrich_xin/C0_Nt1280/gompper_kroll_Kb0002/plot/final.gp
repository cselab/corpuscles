reset
Es=8*pi#bending energy of a sphere

#set size 0.7, 0.7
set term postscript eps enhanced color
set output "final_eb_vr_gompper.eps"

set title "Model A: bending energy vs reduced volume, fv, N_v=1280"
set ylabel "E_b/(8{/Symbol pk}_b)"
set xlabel "V_r"

set key spacing 1.5
plot [0.2:1][1:3.5]\
'../prolate/eng_vol/prolate_final.dat' u 2:($3/Es/$1) w lp pt 4 t 'K_b=0.002 init. config. V_r: prolate', \
'../oblate/eng_vol/oblate_final.dat' u 2:($3/Es/$1) w lp pt 6 t 'K_b=0.002 init. config. V_r: oblate',\
'../sphere/eng_vol/sphere_final.dat' u 2:($3/Es/$1) w lp pt 8 t 'K_b=0.002 init. config.: sphere',\
2 w l lc -1 t ''

set term x11
replot
reset