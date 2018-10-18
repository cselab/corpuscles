make
off=../../../data/rbc/laplace/2.off
./main meyer     1 0 0 0  <  $off > m
./main juelicher 1 0 0 0  <  $off > j
./main canham    1 0 0 0  <  $off > c

gnuplot <<!
set term png
set output "i.png"
plot [][:0.2] \
     "m" u 'r':'fm' t "m", \
     "c" u 'r':'fm' t "c", \
     "j" u 'r':'fm' t "j"
!
