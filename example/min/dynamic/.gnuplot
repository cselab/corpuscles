set macro
c(x) = column(x)

f = 'sprintf("Nt_%d/dt_%g/mu_%g/all", Nt, dt, mu)'
t = 'sprintf("Nt = %d dt = %g mu = %g", Nt, dt, mu)'

kb=0.001
eb0 = 4*pi*kb
freq = 10000

p = '@f u ($0*dt*freq/mu):(c("h1")/eb0) w l lw 3 t @t'

plot  \
      mu = 10, Nt=1280, dt=0.001, @p, \
               Nt=5120, dt=0.001, @p
      
      
