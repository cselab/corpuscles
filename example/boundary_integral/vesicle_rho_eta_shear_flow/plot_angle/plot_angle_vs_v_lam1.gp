reset

set term postscript eps enhanced color 
set output "angle_vs_v_lam1.eps"

#set size 0.7,0.7
xmin=0.5
xmax=1
ymin=0
ymax=0.25

set xrange[xmin:xmax]
set yrange[ymin:ymax]

set key left top
set key spacing 1.5
set key width 1
set key samplen 1

set lmargin 6
set bmargin 2

set xlabel 'v' offset 0, 1
set ylabel '{/Symbol q}/{/Symbol p}' offset 3, 0
psize1=1.5
psize2=1.5

set xtics 0.05
set ytics 0.05
plot \
"../reference/angle_kraus1996_theory.dat" u 1:2 w l lc -1 t '{/Symbol l}=1.0: Kraus1996 theory',\
"../reference/angle_kraus1996_sim.dat" u 1:2 w p lc -1 pt 1 ps psize1 t '{/Symbol l}=1.0: Kraus1996 sim. N_t=670',\
"../reference/angle_kantsler2005_exp1.dat" u 1:2 w p lc -1 pt 6 ps psize1 t '{/Symbol l}=1.0: Kantsler2005 exp.',\
"../reference/angle_zhao2009_sim1.dat" u 1:2 w p lc -1 pt 7 ps psize1 t '{/Symbol l}=1.0: Zhao2009 Sim.',\
"../min_Nt320/post_angle/lam1.0_av.dat" u 1:($3/pi):($5/pi) w p lc 3 pt 6 ps psize2 t '{/Symbol l}=1.0: N_t=320',\
"../min_Nt1280/post_angle_transit/lam1.0_av.dat" u 1:($3/pi):($5/pi) w p lc 4 pt 7 ps psize2 t '{/Symbol l}=1.0: N_t=1280'

set term qt
replot

reset
