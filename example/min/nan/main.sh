set -eu

. he.util
make
rV=0.45 Ka=0.1 Kga=2 Kv=1 Ke=0 Kb=0.1 C0=`ae "3*$rV^(1/3)"` Kad=0 DAD0=0

end=999999
off=`he.path`/sph/laplace/0.off

./main meyer $rV   $Ka $Kga $Kv  $Ke      $Kb $C0 $Kad $DAD0          $end < $off
