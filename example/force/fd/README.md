Plot finite difference derivatives of energy vs force (step is 1e-6)
for every 10 points.

$ ./main gompper 1e-6 10 < ../../../data/rbc/laplace/1.off > g
$ ./main meyer   1e-6 10 < ../../../data/rbc/laplace/1.off > m

gnuplot> plot "m", "g", "c"
