VAL=--tool=callgrind co.run ./main '<' `co.path`/rbc/laplace/2.off '>' normal.out.txt
time ./main < `co.path`/rbc/laplace/2.off > normal.out.txt

real	0m1.485s
