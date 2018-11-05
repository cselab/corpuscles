reset

set title "bending energy vs reduced volume"
set ylabel "bending energy"
set xlabel "V_r"

plot \
'prolate_final.dat' u 1:2 w lp pt 4 t 'prolate', \
'oblate_final.dat' u 1:2 w lp pt 6 t 'oblate',\
'sphere_final.dat' u 1:2 w lp pt 8 t 'sphere'


reset