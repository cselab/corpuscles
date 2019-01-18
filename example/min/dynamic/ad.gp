set term push
set term pdfcairo enhanced color fontscale 0.7
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


set xrange[0:4000]
set yrange[0.5:1.5]
set xtics 1000
set ytics 0.2

plot  i=0, \
      mu = 10, \
      Nt=1280,  dt=0.001, @p,\
      Nt=5120,  dt=0.001, @p

      
