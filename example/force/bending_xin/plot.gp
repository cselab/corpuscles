scl=0.1;
c(x)=column(x);

splot [-1:1][-1:1][-0.4:0.4] \
'test_g.dat' u 'x':'y':'z' w p pt 6, \
'test_g.dat' u 'x':'y':'z':(c('Fx')*scl):(c('Fy')*scl):(c('Fz')*scl) w vector lc 3 t 'g',\
'test_m.dat' u 'x':'y':'z':(c('Fx')*scl):(c('Fy')*scl):(c('Fz')*scl) w vector lc 4 t 'm'