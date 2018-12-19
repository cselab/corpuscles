set xrange [1.08:1.25]
set yrange [1.08:1.25]

plot \
'../eng_vol/geom.dat' u 1:($3/4.0/pi) w lp t 'bc',\
x w l lc -1