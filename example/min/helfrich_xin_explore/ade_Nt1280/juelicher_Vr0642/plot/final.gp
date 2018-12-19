reset
Es=8*pi#bending energy of a sphere

set size 0.7, 0.7
set term pdfcairo enhanced color fontscale 0.7
set output "juelicher_ade_eb_da1.pdf"


#set title "Model B: bending energy vs reduced volume, N_v=1280"


set lmargin 6
set bmargin 3.5
set ylabel "E/(8{/Symbol pk}_b)" offset 2.5,3
set xlabel "100{/Symbol D}a_0"

set xtics 0.05
set ytics 0.1

xl=0.1
xh=0.3
yl=0
yh=0.5
set xrange[xl:xh]
set yrange[yl:yh]

set key spacing 1.5
set key width 1
set key samplen 2

set key right top

psize=1

plot [][0.15:0.25]\
'../sphere/eng_vol/final.dat' u 4:($3/Es/$1) w p lc 1 pt 4 ps psize t 'juelicher 1',\
'../sphere/eng_vol_restart/final.dat' u 4:($3/Es/$1) w p lc 2 pt 6 ps psize t 'juelicher restart',\
'../sphere/eng_vol_long/final.dat' u 4:($3/Es/$1) w p lc 3 pt 8 ps psize t 'juelicher long',\
0.2 w l lc -1 t ''


set term x11
replot

reset
