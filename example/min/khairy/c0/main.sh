set -eu

rV=0.56 Kb=0.00k1 C0=2.47 Kad=0 Ke=0
end=750000
off=../../../../data/sph/laplace/0.off
DAD0=0

./main juelicher_xin $rV   40 40 2  $Ke      $Kb $C0 $Kad $DAD0          $end < $off
