set term push
set term pdfcairo enhanced color fontscale 0.7
set size sq

o = "ad.pdf"
set output o
print o

set macro
c(x) = column(x)

f = 'sprintf("Nt_%d/dt_%g/mu_%g/all", Nt, dt, mu)'
t = 'sprintf("Nt = %d dt = %g mu = %g", Nt, dt, mu)'

pt = 'sprintf("N_t = %d {/Symbol D}t = %g", Nt, dt)'

set key spacing 1.5
set key width 1
set key samplen 6
set key right bottom

set xlabel 'time'
set ylabel '{/Symbol D}a'

kb = 0.001
Da0 = 4*pi
freq = 10000

i=0
set style line 1 lw 2 lc 2 dt 2
set style line 2 lw 2 lc 4 dt 4

p = 'i=i+1, @f u (($0-1)*dt*freq):(c("h1")/Da0) w l ls i t @pt'

WX = 800.0
WY = 600.0

xl = -270
xh = 4000

yl = 0.5
yh = 1.5

lx = xh - xl
ly = yh - yl

dx = 1
dy = dx*ly/lx # * WX/WY

set xrange [xl:xh]
set yrange [yl:yh]

set xtics 1000
set ytics 0.2

png = 'f binary filetype=png dx=dx dy=dy center=(x, y) with rgbalpha t ""'


plot  i=0, \
      mu = 10, \
      Nt=1280,  dt=0.001, @p,\
      Nt=5120,  dt=0.001, @p, \
      f="image/00000000.png", x = 0,    y = 0.8, @png, \
      f="image/00000250.png", x = 2500, y = 1.1, @png
