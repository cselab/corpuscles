set xrange [0.98:1.6]
set yrange [0.98:1.6]

#Vr="08"
#Vr="07"
Vr="069"
#Vr="065"
#Vr="0642"
#Vr="06"
#Vr="06_Kv2"
#Vr="059"
#Vr="055"
#Vr="05"
#Vr="045"
#Vr="031"
#Vr="031_Kv2"

set key left top
plot \
"../juelicher_Vr".Vr."/sphere/eng_vol/geom.dat" u 1:($3/4.0/pi) w lp lc 1 pt 4 t 'juelicher',\
"../gompper_xin_Vr".Vr."/sphere/eng_vol/geom.dat" u 1:($3/4.0/pi) w lp lc 2 pt 6 t 'gompper',\
"../meyer_xin_Vr".Vr."/sphere/eng_vol/geom.dat" u 1:($3/4.0/pi) w lp lc 3 pt 8 t 'meyer',\
x w l lc -1