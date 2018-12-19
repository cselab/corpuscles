plot \
'<sort ../../juelicher_Vr0642/sphere/eng_vol/geom.dat' u ($3/$1/2):($5/4.0/pi) w lp t 'a=0.64',\
'../sphere/eng_vol/geom.dat' u ($3/$1/2):($5/4.0/pi) w lp t 'a=1.4',\
x w l lc -1