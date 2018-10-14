Plot finite difference derivative of energy vs force (step is 1e-6).

$ ./main gompper 1e-6 10 < ../../../data/rbc/laplace/1.off > g
$ ./main meyer   1e-6 10 < ../../../data/rbc/laplace/1.off > m
$ ./main cahnman 1e-6 10 < ../../../data/rbc/laplace/1.off > c

gnuplot> plot "m", "g", "c"
