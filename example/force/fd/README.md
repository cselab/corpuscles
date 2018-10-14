Plot finite difference derivative of energy vs force (step is 1e-6).

Takes some time

$ ./main gompper 1e-6 1 < ../../../data/rbc/laplace/1.off > g
$ ./main meyer   1e-6 1 < ../../../data/rbc/laplace/1.off > m

gnuplot> plot "m", "g"
