set term push
set term pdfcairo enhanced color fontscale 0.7
o = "energy.pdf"
set output o
print o

set macro
c(x) = column(x)

f = 'sprintf("Nt_%d/dt_%g/mu_%g/all", Nt, dt, mu)'
t = 'sprintf("Nt = %d dt = %g mu = %g", Nt, dt, mu)'

pt = 'sprintf("local: N_t = %d {/Symbol D}t = %g", Nt, dt)'
qt = 'sprintf("non-local: N_t = %d {/Symbol D}t = %g", Nt, dt)'

set key spacing 1.5
set key width 1
set key samplen 6
set key right center

set xlabel 'time'
set ylabel 'E/(8{/Symbol p k}_b)'

kb = 0.001
eb0 = 8*pi*kb
freq = 10000

i=0
set style line 1 lw 2 lc 1 dt 1
set style line 2 lw 2 lc 2 dt 2
set style line 3 lw 2 lc 3 dt 3
set style line 4 lw 2 lc 4 dt 4

p = 'i=i+1, @f u (($0-1)*dt*freq):(c("eb_bend")/eb0) w l ls i t @pt'
q = 'i=i+1, @f u (($0-1)*dt*freq):(c("eb_ad")/eb0) w l ls i t @qt'


set xrange[0:18000]
set yrange[0:2.5]
set xtics 3000
set ytics 0.5

plot  i=0, \
      mu = 10, \
      Nt=1280,  dt=0.005, @p, @q,\
      Nt=5120,  dt=0.001, @p, @q

      
