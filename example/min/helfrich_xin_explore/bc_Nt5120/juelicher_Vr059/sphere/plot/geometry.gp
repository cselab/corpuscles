set xrange [0.98:1.45]
set yrange [0.98:1.45]

plot \
'../eng_vol/geom.dat' u 1:($3/4.0/pi) w lp t 'bc',\
'../eng_vol_cont/geom.dat' u 1:($3/4.0/pi) w lp t 'bc cont',\
x w l lc -1