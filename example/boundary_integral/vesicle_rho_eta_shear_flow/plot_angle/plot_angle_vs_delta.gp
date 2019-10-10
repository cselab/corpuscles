reset

set term postscript eps enhanced color 
set output "angle_vs_delta.eps"

#set size 0.7,0.7
xmin=0.0
xmax=1.5
ymin=0
ymax=0.25

set xrange[xmin:xmax]
set yrange[ymin:ymax]

set key right top
set key spacing 1.5
set key width 1
set key samplen 1

set lmargin 6
set bmargin 2

set xlabel '{/Symbol D} ' offset 0, 1
set ylabel '{/Symbol q}/{/Symbol p}' offset 3, 0
psize1=1.5
psize2=1.5

width=1
set xtics 0.5
set ytics 0.05
plot \
"../reference/zhao2011_sim1.dat" u 1:2 w l lc -1 lt 1 lw width t '{/Symbol l}=1.0: Zhao2009 Sim.',\
"../reference/zhao2011_sim2.dat" u 1:2 w l lc 1 lt 2 lw width t '{/Symbol l}=2.7: Zhao2009 Sim.',\
"../reference/zhao2011_sim3.dat" u 1:2 w l lc 2 lt 3 lw width t '{/Symbol l}=5.6: Zhao2009 Sim.',\
"../min_Nt320/post_angle/lam1.0_av.dat" u (4.0*pi*($1**(-2.0/3.0)-1)):($3/pi):($4/pi) w e lc 3 pt 6 ps psize2 t '{/Symbol l}=1.0: N_t=320',\
"../min_Nt320/post_angle/lam2.7_av.dat" u (4.0*pi*($1**(-2.0/3.0)-1)):($3/pi):($4/pi) w e lc 3 pt 4 ps psize2 t '{/Symbol l}=2.7: N_t=320',\
"../min_Nt320/post_angle/lam5.6_av.dat" u (4.0*pi*($1**(-2.0/3.0)-1)):($3/pi):($4/pi) w e lc 3 pt 8 ps psize2 t '{/Symbol l}=5.6: N_t=320',\
"../min_Nt1280/post_angle_transit/lam1.0_av.dat" u (4.0*pi*($1**(-2.0/3.0)-1)):($3/pi):($4/pi) w e lc 4 pt 7 ps psize2 t '{/Symbol l}=1.0: N_t=1280',\
"../min_Nt1280/post_angle_transit/lam2.7_av.dat" u (4.0*pi*($1**(-2.0/3.0)-1)):($3/pi):($4/pi) w e lc 4 pt 5 ps psize2 t '{/Symbol l}=2.7: N_t=1280',\
"../min_Nt1280/post_angle_transit/lam5.6_av.dat" u (4.0*pi*($1**(-2.0/3.0)-1)):($3/pi):($4/pi) w e lc 4 pt 9 ps psize2 t '{/Symbol l}=5.6: N_t=1280',\
"../min_Nt5120/post_angle_transit/lam1.0_av.dat" u (4.0*pi*($1**(-2.0/3.0)-1)):($3/pi):($4/pi) w e lc 5 pt 10 ps psize2 t '{/Symbol l}=1.0: N_t=5120',\
"../min_Nt5120/post_angle_transit/lam2.7_av.dat" u (4.0*pi*($1**(-2.0/3.0)-1)):($3/pi):($4/pi) w e lc 5 pt 12 ps psize2 t '{/Symbol l}=2.7: N_t=5120',\
"../min_Nt5120/post_angle_transit/lam5.6_av.dat" u (4.0*pi*($1**(-2.0/3.0)-1)):($3/pi):($4/pi) w e lc 5 pt 14 ps psize2 t '{/Symbol l}=5.6: N_t=5120'

set term qt
replot

reset
