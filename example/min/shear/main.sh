set -eu

. he.util
make
rV=0.64 Ka=0.1 Kga=2 Kv=1 Ke=0 Kb=0.1 C0=0 Kad=0 DAD0=0

end=999999
off=../../../data/sph/laplace/0.off

./main kantor $rV   $Ka $Kga $Kv  $Ke      $Kb $C0 $Kad $DAD0          $end < $off
