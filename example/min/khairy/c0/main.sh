set -eu

rV=0.56 Ka=0.1 Kga=2 Kv=1 Kb=0.1 C0=2.48 Kad=0 Ke=0 DAD0=0
end=750000
off=../../../../data/sph/laplace/0.off

./main juelicher_xin $rV   $Ka $Kga $Kv  $Ke      $Kb $C0 $Kad $DAD0          $end < $off
