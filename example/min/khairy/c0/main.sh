set -eu

. he.util

rV=0.56 Ka=0.1 Kga=2 Kv=1 Ke=0 Kb=0.05 C0=`ae "3*$rV^(1/3)"` Kad=0 DAD0=0

end=999999
off=../../../../data/sph/laplace/0.off

./main juelicher_xin $rV   $Ka $Kga $Kv  $Ke      $Kb $C0 $Kad $DAD0          $end < $off
