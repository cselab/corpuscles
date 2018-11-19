set -eu

rV=0.642 Kb=0.1 C0=3 Kad=0
end=7500
off=../../../../data/sph/laplace/0.off
DAD0=0

./main juelicher $rV   40 40 2 0       $Kb $C0 $Kad $DAD0          $end < $off > q
