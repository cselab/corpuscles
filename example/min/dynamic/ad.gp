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
#set key left top

set xlabel 'dimensionless time'
set ylabel '{/Symbol D}a'

kb = 0.001
Da0 = 4*pi
freq = 10000

i=0
set style line 1 lw 2 lc 2 dt 2
set style line 2 lw 2 lc 4 dt 4

p = 'i=i+1, @f u (($0-1)*dt*freq/mu):(c("h1")/Da0) w l ls i t @pt'

WX = 800.0
WY = 600.0

shift=600
xl = 0-shift
xh = 12000+shift

yl = 0.5
yh = 1.5

lx = xh - xl
ly = yh - yl

dx = 2
dy = dx*ly/lx

set xrange [xl:xh]
set yrange [yl:yh]

#set logscale x
set xtics 2.0e3
set ytics 0.2

png = 'f binary filetype=png dx=dx dy=dy center=(x, y) with rgbalpha t ""'


plot  i=0, \
      mu = 1, \
      Nt=1280,  dt=0.005, @p, \
      f="image/00000000.png", x = 0,    y = 0.54, @png, \
      f="image/00000250.png", x = 250+shift,  y = 0.7, @png,
      
      #f="image/00000500.png", x = 500,  y = 1.1, @png,  \
      #f="image/00001000.png", x = 1000, y = 1.1, @png, \
      #f="image/00002000.png", x = 1000, y = 1.1, @png
