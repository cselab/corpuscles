set -eu

. he.util
make
rV=0.63 Ka=10 Kga=20 Kv=1 Ke=0.1 Kb=0.01   C0=0 Kad=0 DAD0=0

end=999999
off=`he.path`/rbc/laplace/0.off

./main kantor $rV   $Ka $Kga $Kv  $Ke      $Kb $C0 $Kad $DAD0          $end < $off
